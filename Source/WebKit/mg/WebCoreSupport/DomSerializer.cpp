// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// How we handle the base tag better.
// Current status:
// At now the normal way we use to handling base tag is
// a) For those links which have corresponding local saved files, such as
// savable CSS, JavaScript files, they will be written to relative URLs which
// point to local saved file. Why those links can not be resolved as absolute
// file URLs, because if they are resolved as absolute URLs, after moving the
// file location from one directory to another directory, the file URLs will
// be dead links.
// b) For those links which have not corresponding local saved files, such as
// links in A, AREA tags, they will be resolved as absolute URLs.
// c) We comment all base tags when serialzing DOM for the page.
// FireFox also uses above way to handle base tag.
//
// Problem:
// This way can not handle the following situation:
// the base tag is written by JavaScript.
// For example. The page "www.yahoo.com" use
// "document.write('<base href="http://www.yahoo.com/"...');" to setup base URL
// of page when loading page. So when saving page as completed-HTML, we assume
// that we save "www.yahoo.com" to "c:\yahoo.htm". After then we load the saved
// completed-HTML page, then the JavaScript will insert a base tag
// <base href="http://www.yahoo.com/"...> to DOM, so all URLs which point to
// local saved resource files will be resolved as
// "http://www.yahoo.com/yahoo_files/...", which will cause all saved  resource
// files can not be loaded correctly. Also the page will be rendered ugly since
// all saved sub-resource files (such as CSS, JavaScript files) and sub-frame
// files can not be fetched.
// Now FireFox, IE and WebKit based Browser all have this problem.
//
// Solution:
// My solution is that we comment old base tag and write new base tag:
// <base href="." ...> after the previous commented base tag. In WebKit, it
// always uses the latest "href" attribute of base tag to set document's base
// URL. Based on this behavior, when we encounter a base tag, we comment it and
// write a new base tag <base href="."> after the previous commented base tag.
// The new added base tag can help engine to locate correct base URL for
// correctly loading local saved resource files. Also I think we need to inherit
// the base target value from document object when appending new base tag.
// If there are multiple base tags in original document, we will comment all old
// base tags and append new base tag after each old base tag because we do not
// know those old base tags are original content or added by JavaScript. If
// they are added by JavaScript, it means when loading saved page, the script(s)
// will still insert base tag(s) to DOM, so the new added base tag(s) can
// override the incorrect base URL and make sure we alway load correct local
// saved resource files.

#include "config.h"
#include "minigui.h"

#include "MDolphinDelegates.h"
#include "Document.h"
#include "HTMLDocument.h"
#include "HTMLElementFactory.h"
#include "HTMLFrameOwnerElement.h"
#include "HTMLHeadElement.h"
#include "HTMLImageLoader.h"
#include "HTMLInputElement.h"
#include "HTMLLinkElement.h"
#include "HTMLMapElement.h"
#include "HTMLNameCollection.h"
#include "HTMLNames.h"
#include "HTMLMetaElement.h"
#include "NamedNodeMap.h"
#include "DomSerializer.h"
#include "Frame.h"
#include "markup.h"
#include "TextEncoding.h"
#include "FrameLoader.h"
#include "CString.h"
#include "PlatformString.h"

#include "FrameLoader.h"
#include "ResourceHandle.h"
#include "FrameLoadRequest.h"
#include "ResourceRequest.h"
#include "ResourceHandleInternal.h"
#include "DownloadResourceClient.h"
#include "RenderImage.h"
#include "SharedBuffer.h"
#include "BitmapImage.h"
#include "CachedResource.h"
#include "CachedResourceClient.h"
#include "CachedScript.h"
#include "CachedCSSStyleSheet.h"
#include "CachedResourceLoader.h"

#if ENABLE(FILESYSTEM)
#include "FileSystemMg.h"
#endif

#include <errno.h>
#include "HTMLAllCollection.h"

namespace {

	// Default "mark of the web" declaration
	static const char* const kDefaultMarkOfTheWeb =
		"\n<!-- saved from url=(%04d)%s -->\n";

	// Default meat content for writing correct charset declaration.
	static const char* const kDefaultMetaContent =
		( "<META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");

	// Notation of start comment.
	static const char* const kStartCommentNotation = "<!-- ";

	// Notation of end comment.
	static const char* const kEndCommentNotation = " -->";

	// Default XML declaration.
	static const char* const kXMLDeclaration =
		"<?xml version=\"%ls\" encoding=\"%ls\"%ls?>\n";

	// Default base tag declaration
	static const char* const kBaseTagDeclaration =
		"<BASE href=\".\"%ls>";

	static const char* const kBaseTargetDeclaration =
		" target=\"%ls\"";

	// Maximum length of data buffer which is used to temporary save generated
	// html content data.
	static const int kHtmlContentBufferLength = 65535;


}  // namespace

namespace WebCore {

#define MAX_FILENAME 256

int findRevNotChar(const String & str, char ch)
{
	int pos;
	if (str.length() <= 0)
		return -1;

	pos = str.reverseFind(ch);
	if (pos <= 0)
		return -1;

	pos--;
	while ((pos>=0) && (str[pos] == ch)) {
		pos--;  
	}

	return pos;
}

static String getFilePost(const String & path)
{
	String str("");
	int findpos;
	int pos;

	if (path.length() <= 0) {
		//printf("%s:%d\n", __FILE__, __LINE__); 
		return str;
	}

	findpos = path.reverseFind('/'); 
	//int pos = findRevNotChar(path, '/');
	findpos = (findpos < 0)?0:(findpos+1);
	pos = findpos;

	if (pos == (int)path.length())
		return str;
	str = path.substring(pos, (path.length() - pos));
	pos = str.reverseFind('.'); 
	if (pos < 0) {
		//printf("%s:%d\n", __FILE__, __LINE__); 
		return str;
	}
	else
		return  str.substring(pos, (str.length() - pos));
}

static String getbaseFilenameAndNOPost(const String & path)
{
	String str;
	if (path.length() <= 0)
		return str;

	int findpos = path.reverseFind('/'); 
	findpos = (findpos < 0) ? 0 : (findpos + 1);
	unsigned int pos = (unsigned int) findpos;

	if (pos == path.length())
		return str;
	//int pos = findRevNotChar(path, '/');
	//if (pos < 0)
	//     pos = 0;

	String filename = path.substring(pos, path.length());
	pos = filename.reverseFind('.'); 
	pos = (pos > 0)?(pos) : (filename.length());
	return filename.substring(0, pos);
}

static String getFilebasePath(const String & filename)
{
	String str; 
	int pos = findRevNotChar(filename, '/');
	if (pos < 0)
		str = "./";
	else {
		str = filename.substring(0, pos+1);
	}

	return str;
}

static int SaveDatatoFile(String &strdata, const String & filename)
{

#if ENABLE(FILESYSTEM)

	char file[MAX_FILENAME]; 
	CString cstr;
	HFile fp;

	String path = getFilebasePath(filename);
	if (!fileExists(path)) {
		makeAllDirectories(path); 
	}
	cstr  =  filename.utf8();

	//printf("%s:%d orig-filename=%s\n", __FILE__, __LINE__, cstr.data());
	snprintf(file, sizeof(file)-1, "%s",
			cstr.data());
	fp = openFile(file, "a+");
	if (!fp) {

		return -1;
	}
	//String  strdata = String::adopt(data);
	writeFile((void *)strdata.utf8().data(),  1, strdata.utf8().length(),fp);
	cstr = strdata.utf8();
	//printf("%s:%d data=%s\n", __FILE__, __LINE__, cstr.data());
	closeFile(fp);
#endif

	return 0;
}

static int SaveDatatoFile(const char *strdata, int & len,  const String & filename)
{

#if ENABLE(FILESYSTEM)
	CString cstr;
	String path = getFilebasePath(filename);
	char file[MAX_FILENAME]; 
	int ret;
	HFile fp;

	if (!fileExists(path)) {
		makeAllDirectories(path); 
	}
	cstr =  filename.utf8();

	//printf("%s:%d orig-filename=%s\n", __FILE__, __LINE__, cstr.data());
	ret  = snprintf(file, sizeof(file)-1, "%s", cstr.data());
	ret = (MAX_FILENAME -1 < ret)? ret:(MAX_FILENAME -1);
	fp = openFile(file, "a+");
	if (!fp){
		printf("%s:%d open file %s error! \n", __FILE__, __LINE__, file); 
		return -1;
	}
	//String  strdata = String::adopt(data);
	writeFile((void *)strdata, 1, len, fp);
	//cstr = strdata.utf8();
	//printf("%s:%d data=%s\n", __FILE__, __LINE__, cstr.data());
	closeFile(fp);
#endif

	return 0;
}

static int SaveDatatoFile(Vector<UChar> &data, const String & filename)
{

#if ENABLE(FILESYSTEM)
	char file[MAX_FILENAME]; 
	CString cstr;
	String path = getFilebasePath(filename);
	int ret;

	if (!fileExists(path)) {
		makeAllDirectories(path); 
	}
	cstr =  filename.utf8();

	//printf("%s:%d orig-filename=%s\n", __FILE__, __LINE__, cstr.data());
	ret = snprintf(file, sizeof(file)-1, "%s", cstr.data());
	ret = (MAX_FILENAME -1 < ret)? ret:(MAX_FILENAME -1);
	//printf("%s:%d filename=%s\n",  __FILE__, __LINE__, file);
	HFile fp = openFile(file, "a+");
	if (!fp) {
		printf("%s:%d open file %s error! \n", __FILE__, __LINE__, file); 
		return -1;
	}
	String  strdata = String::adopt(data);
	cstr = strdata.utf8();
	ret = writeFile((void *)cstr.data(),  1, cstr.length(),fp);
	closeFile(fp);
#endif

	return 0;
}

// SerializeDomParam Constructor.
DomSerializer::SerializeDomParam::SerializeDomParam(
		const KURL& current_frame_kurl,
		const TextEncoding& text_encoding,
		Document* doc,
		const String& directory_name)
: current_frame_kurl(current_frame_kurl),
	text_encoding(text_encoding),
	doc(doc),
	directory_name(directory_name),
	has_doctype(false),
	has_checked_meta(false),
	skip_meta_element(NULL),
	is_in_script_or_style_tag(false),
	has_doc_declaration(false)
{
	// Cache the value since we check it lots of times.
	is_html_document = doc->isHTMLDocument();
}

// Static
String DomSerializer::GenerateMetaCharsetDeclaration(const  String& charset)
{
	//String str; 
	//CString cstr = charset.utf8();
	//String relstr = str.format((const char *)kDefaultMetaContent, "utf-8" );
	// printf("meta=%s\n", relstr.utf8().data());
	//return relstr;
	//return StringPrintf(kDefaultMetaContent, charset.c_str());
	String str(kDefaultMetaContent);
	return str;
}

// Static.
String DomSerializer::GenerateMarkOfTheWebDeclaration(const KURL& url)
{
	//  return StringPrintf((kDefaultMarkOfTheWeb), url.spec().size(), url.spec().c_str());
	//  return StringPrintf((kDefaultMarkOfTheWeb), url.spec().size(), url.spec().c_str());

	//str.format((const char *)kDefaultMarkOfTheWeb, url.string().length(), url.string().ascii());
	String str(kDefaultMarkOfTheWeb);
	return str;
}

// Static.
String DomSerializer::GenerateBaseTagDeclaration( const String& base_target)
{
	String str ; 
	str = "<BASE href=\".\"  "; // %ls>";
	if (base_target.length() > 0) {
		//" target=\"%ls\"";
		str += "target="; 
		str += "\"" ;
		str += base_target;
		str += "\"";
	}
	str += ">";

	return str;
}

String DomSerializer::PreActionBeforeSerializeOpenTag(
		const Element* element, SerializeDomParam* param,
		bool* need_skip) 
{
	String result;
	*need_skip = false;
	if (param->is_html_document) {
		// Skip the open tag of original META tag which declare charset since we
		// have overrided the META which have correct charset declaration after
		// serializing open tag of HEAD element.
		if (element->hasTagName(HTMLNames::metaTag)) {
			const HTMLMetaElement* meta =
				static_cast<const HTMLMetaElement*>(element);
			// Check whether the META tag has declared charset or not.
			String equiv = meta->httpEquiv();
			if (equalIgnoringCase(equiv, "content-type")) {
				String content = meta->content();
				if (content.length() && content.contains("charset", false)) {
					// Find META tag declared charset, we need to skip it when
					// serializing DOM.
					param->skip_meta_element = element;
					*need_skip = true;
				}
			}
		} 
		else if (element->hasTagName(HTMLNames::baseTag)) {
			result += String(kStartCommentNotation);
		}
	}
	return result;
}

String DomSerializer::PostActionAfterSerializeOpenTag(
		const Element* element, SerializeDomParam* param)
{
	String result;

	param->has_added_contents_before_end = false;
	if (!param->is_html_document)
		return result;
	// Check after processing the open tag of HEAD element
	if (!param->has_checked_meta && element->hasTagName(HTMLNames::headTag)) {
		param->has_checked_meta = true;
		// Check meta element. WebKit only pre-parse the first 512 bytes
		// of the document. If the whole <HEAD> is larger and meta is the
		// end of head part, then this kind of pages aren't decoded correctly
		// because of this issue. So when we serialize the DOM, we need to
		// make sure the meta will in first child of head tag.
		// See http://bugs.webkit.org/show_bug.cgi?id=16621.
		// First we generate new content for writing correct META element.
		String str_meta = GenerateMetaCharsetDeclaration(String(param->text_encoding.name()));
		result += (str_meta);
		param->has_added_contents_before_end = true;
		// Will search each META which has charset declaration, and skip them all
		// in PreActionBeforeSerializeOpenTag.
	} 
	else 
		if (element->hasTagName(HTMLNames::scriptTag) ||
				element->hasTagName(HTMLNames::styleTag)) {
			param->is_in_script_or_style_tag = true;
		}

	return result;
}

String DomSerializer::PreActionBeforeSerializeEndTag(
		const Element* element, SerializeDomParam* param,
		bool* need_skip)
{
	String result;
	*need_skip = false;
	if (!param->is_html_document)
		return result;
	// Skip the end tag of original META tag which declare charset.
	// Need not to check whether it's META tag since we guarantee
	// skip_meta_element is definitely META tag if it's not NULL.
	if (param->skip_meta_element == element) {
		*need_skip = true;
	} else if (element->hasTagName(HTMLNames::scriptTag) ||
			element->hasTagName(HTMLNames::styleTag)) {
		//DCHECK(param->is_in_script_or_style_tag);
		param->is_in_script_or_style_tag = false;
	}

	return result;
}

// After we finish serializing end tag of a element, we give the target
// element a chance to do some post work to add some additional data.
String DomSerializer::PostActionAfterSerializeEndTag(
		const Element* element, SerializeDomParam* param)
{
	String result;

	if (!param->is_html_document)
		return result;
	// Comment the BASE tag when serializing DOM.
	if (element->hasTagName(HTMLNames::baseTag)) {
		result += String((const char *)kEndCommentNotation);
		// Append a new base tag declaration.
		result += GenerateBaseTagDeclaration(param->doc->baseTarget());
	}
	return result;
}

void DomSerializer::SaveHtmlContentToBuffer(const String& result,
		SerializeDomParam* param)
{
	if (!result.length())
		return;
	if (result.length() + data_buffer_.size()  > data_buffer_.capacity()) {
		SaveDatatoFile(data_buffer_, param->directory_name);
		data_buffer_.clear();
		data_buffer_.reserveCapacity(kHtmlContentBufferLength);

	}

	data_buffer_.append(result.characters(), result.length());
}

const AtomicString* GetSubResourceLinkFromElement(const Element* element)
{
	const QualifiedName* attribute_name = NULL;
	if (element->hasTagName(HTMLNames::imgTag) ||
			element->hasTagName(HTMLNames::scriptTag) ||
			element->hasTagName(HTMLNames::iframeTag) ||
			element->hasTagName(HTMLNames::frameTag) ||
			element->hasTagName(HTMLNames::linkTag)) {
		// Get value.
		if (element->hasTagName(HTMLNames::linkTag)) {
			// If the link element is not linked to css, ignore it.
			const HTMLLinkElement* link =
				static_cast<const HTMLLinkElement*>(element);
			if (!link->sheet())
				return NULL;
			// TODO(jnd). Add support for extracting links of sub-resources which
			// are inside style-sheet such as @import,.string(), etc.
			// See bug: http://b/issue?id=1111667.
			attribute_name = &HTMLNames::hrefAttr;
		} else {
			attribute_name = &HTMLNames::srcAttr;
		}
	} else if (element->hasTagName(HTMLNames::inputTag)) {
		const HTMLInputElement* input =
			static_cast<const HTMLInputElement*>(element);
#if 0 //FIXME
		if (input->inputType() == HTMLInputElement::IMAGE) {
			attribute_name = &HTMLNames::srcAttr;
		}
#endif
	} else if (element->hasTagName(HTMLNames::bodyTag) ||
			element->hasTagName(HTMLNames::tableTag) ||
			element->hasTagName(HTMLNames::trTag) ||
			element->hasTagName(HTMLNames::tdTag)) {
		attribute_name = &HTMLNames::backgroundAttr;
	} else if (element->hasTagName(HTMLNames::blockquoteTag) ||
			element->hasTagName(HTMLNames::qTag) ||
			element->hasTagName(HTMLNames::delTag) ||
			element->hasTagName(HTMLNames::insTag)) {
		attribute_name = &HTMLNames::citeAttr;
	}
	if (!attribute_name)
		return NULL;
	const AtomicString* value =
		&element->getAttribute(*attribute_name);
	// If value has content and not start with "javascript:" then return it,
	// otherwise return NULL.
	if (value && !value->isEmpty() &&
			!value->startsWith("javascript:", false))
		return value;

	return NULL;
}

static Frame* GetFrameFromElement(const Element* element,
		bool* is_frame_element) 
{
	*is_frame_element = false;
	if (element->hasTagName(HTMLNames::iframeTag) ||
			element->hasTagName(HTMLNames::frameTag)) {
		*is_frame_element = true;
		if (element->isFrameOwnerElement()) {
			// Check whether this frame has content.
			const HTMLFrameOwnerElement* frame_element =
				static_cast<const HTMLFrameOwnerElement*>(element);
			Frame* content_frame = frame_element->contentFrame();
			return content_frame ? content_frame : NULL;
		}
	}
	return NULL;
}
bool ElementHasLegalLinkAttribute(const Element* element,
		const QualifiedName& attr_name, replaceNodeType & replacetype)
{
	if (attr_name == HTMLNames::srcAttr) {
		// Check src attribute.
		if (element->hasTagName(HTMLNames::imgTag)) {
			replacetype = imageType;
			return true;
		}
		if ( element->hasTagName(HTMLNames::scriptTag)) {
			replacetype = jscriptType;
			return true;
		}
		if (element->hasTagName(HTMLNames::iframeTag)) {
			replacetype = iframeType;
			return true;
		}
		if( element->hasTagName(HTMLNames::frameTag)) {
			replacetype = frameType;
			return true;

		}
		if (element->hasTagName(HTMLNames::inputTag)) {
			const HTMLInputElement* input =
				static_cast<const HTMLInputElement*>(element);
#if 0 //FIXME
			if (input->inputType() == HTMLInputElement::IMAGE) {
				replacetype = imageType;
				return true;
			}
#endif
		}
	}
	else if (attr_name == HTMLNames::hrefAttr) {
		// Check href attribute.
		if (element->hasTagName(HTMLNames::linkTag)) {
			replacetype = cssType;
			return true;
		}
		if (element->hasTagName(HTMLNames::aTag) || 
				element->hasTagName(HTMLNames::areaTag)){
			replacetype = hrefType;
			return true;
		}
	} 
	else if (attr_name == HTMLNames::backgroundAttr) {
		if (element->hasTagName(HTMLNames::bodyTag) ||
				element->hasTagName(HTMLNames::tableTag) ||
				element->hasTagName(HTMLNames::trTag) ||
				element->hasTagName(HTMLNames::tdTag)) {
			replacetype = imageType; 
			return true;
		}
	} 
	return false;
}


void DomSerializer::OpenTagToString(const Element* element,
		SerializeDomParam* param) 
{
#if 1
	bool need_skip = false;
	const  NamedNodeMap*  attrMap = element->attributes(true);
	unsigned numAttrs;
	unsigned i ;
	bool need_replace_link ;
	String filePost;
	bool isalsodownlaod = false;
	int index;
	String strreplacename;
	String savefilename;
	bool is_frame_element;
	Frame* web_frame = NULL;

	// Do pre action for open tag.
	String result = PreActionBeforeSerializeOpenTag(element, param, &need_skip);
	if (need_skip )
		return;
	// Add open tag
	result += "<" + element->nodeName();

	// Go through all attributes and serialize them.
	if (attrMap) {
		numAttrs = attrMap->length();
		for (i = 0; i < numAttrs; i++) {
			result += " ";
			// Add attribute pair
			const Attribute *attribute = attrMap->attributeItem(i);
			result += attribute->name().toString();
			result += "=\"";
			if (!attribute->value().isEmpty()) {
				// Check whether we need to replace some resource links
				// with local resource paths.
				const QualifiedName& attr_name = attribute->name();
				// Check whether need to change the attribute which has link
				//bool need_replace_link;
				replaceNodeType replacetype;
				need_replace_link = ElementHasLegalLinkAttribute(element, attr_name, replacetype);
				//if (need_replace_link  and not save html-only 
				if(need_replace_link){
					// First, get the absolute link
					const String& attr_value = attribute->value();
					// For links start with "javascript:", we do not change it.
					if (attr_value.startsWith("javascript:", false)) {
						result += attr_value;
					} 
					else {
						//result += attr_value;
						//String str_value = param->doc->completeURL(attr_value);
						KURL url(param->doc->completeURL(attr_value)); 
						String tmpfilename = url.lastPathComponent();
						if (element->hasTagName(HTMLNames::iframeTag) || 
								element->hasTagName(HTMLNames::frameTag)) {
							//unique check 
							filePost = getFilePost(tmpfilename);
							if (!(filePost.lower() == ".html") && !(filePost.lower() == ".htm") ) {
								tmpfilename.append(".html");
							}
						}
						isalsodownlaod = false;
						index = isExistSameResource(url.string(), true);
						if (index < 0 ) {
							getNewResourceName(tmpfilename);
							subresourceslist.resources_list.append(url);
							subresourceslist.local_file.append(tmpfilename);
						}
						else {
							isalsodownlaod = true;
							tmpfilename = subresourceslist.local_file[index]; 
						}

						//default  replace
						strreplacename = "./" +  local_subresource_directory_name_ + "/"+ tmpfilename;
						savefilename = local_directory_name_+ "/" + local_subresource_directory_name_ + "/"+ tmpfilename; 
						//result += strreplacename;
						switch (replacetype) {
							case imageType: 
								{
									int len;
									CachedImage* cachedImage =  param->doc->cachedResourceLoader()->requestImage(url.string()); 
									ASSERT(cachedImage);
									if (cachedImage->status() != CachedResource::Cached) {
										if (!isalsodownlaod) {
											StartDownloadSubresourceFromURL(url, savefilename);
										}
										//return ;
									}
									else {
										Image* image = cachedImage->image();
										ASSERT(image);
										//printf("image size=%d\n", image->data()->size()); 
										len = image->data()->size();
										const char *data =  image->data()->data();
										if (!isalsodownlaod)
											SaveDatatoFile(data, len,  savefilename);
									}
								}
								break;

							case cssType:
								{
									String charset;
									CachedCSSStyleSheet *cachecsssheet =  param->doc->cachedResourceLoader()->requestUserCSSStyleSheet(url.string(), charset); 

									if (cachecsssheet->status() != CachedResource::Cached) {
										if (!isalsodownlaod)
											StartDownloadSubresourceFromURL(url, savefilename);
									}
									else {
										String strcsssheet = cachecsssheet->sheetText();
										if (!isalsodownlaod)
											SaveDatatoFile(strcsssheet,  savefilename);
									}
								}
								break;
							case jscriptType:
								{
									String charset;
									CachedScript *cacheScirpt = param->doc->cachedResourceLoader()->requestScript(url.string(), charset); 
									if (cacheScirpt->status() != CachedResource::Cached) {
										if (!isalsodownlaod)
											StartDownloadSubresourceFromURL(url, savefilename);
										//return ;
									}
									else {
										String strScript = cacheScirpt->script();
										if (!isalsodownlaod)
											SaveDatatoFile(strScript,  savefilename);
									}
								}
								break;

							case frameType:
							case iframeType:
								{
									// iframe 中的url被重定向， 被重定向的url地址恰好又属于当前网页中的另一个iframe的情况
									// 这时本iframe中的url不替换
									web_frame = GetFrameFromElement( element, &is_frame_element);
									if ((is_frame_element && web_frame)) {
										unsigned int j;
										for (j = 0; j < frames_.size(); j++ ) {
											if ( web_frame->loader()->baseURL() == url.string())
												break;
										}

										if (j == frames_.size())
											strreplacename = url.string();
									}
									else {
										strreplacename = url.string();
									}
								}
								break;

							case hrefType:
								{
									strreplacename = url.string();
								}
								break;

						}
						result += strreplacename;
					}
				} 

				else {
					result += attribute->value();
				}
			}
			result += "\"";
		}
	}

	// Do post action for open tag.
	String added_contents = PostActionAfterSerializeOpenTag(element, param);
	// Complete the open tag for element when it has child/children.
	if (element->hasChildNodes() || param->has_added_contents_before_end)
		result += ">";
	// Append the added contents generate in  post action of open tag.
	if (added_contents.length() > 0)
		result += added_contents;
	// Save the result to data buffer.
	SaveHtmlContentToBuffer(result, param);
#endif
}

// Serialize end tag of an specified element.
void DomSerializer::EndTagToString(const Element* element,
		SerializeDomParam* param)
{
	bool need_skip;
	// Do pre action for end tag.
	String result = PreActionBeforeSerializeEndTag(element,
			param,
			&need_skip);
	if (need_skip)
		return;
	// Write end tag when element has child/children.
	if (element->hasChildNodes() || param->has_added_contents_before_end) {
		result += "</";
		result += element->nodeName();
		result += ">";
	} 
	else {
		// Check whether we have to write end tag for empty element.
		if (param->is_html_document) {
			result += ">";
			const HTMLElement* html_element =
				static_cast<const HTMLElement*>(element);
			if (!html_element->ieForbidsInsertHTML()) {
				// We need to write end tag when it is required.
				result += "</";
				result += element->nodeName();
				result += ">";
			}
		} 
		else { // For xml base document.
			result += " />";
		}
	}
	// Do post action for end tag.
	result += PostActionAfterSerializeEndTag(element, param);
	// Save the result to data buffer.
	SaveHtmlContentToBuffer(result, param);
}

void DomSerializer::BuildContentForNode(const Node* node,
		SerializeDomParam* param)
{
	switch (node->nodeType()) {
		case Node::ELEMENT_NODE:
		   	{
				// Process open tag of element.
				OpenTagToString(static_cast<const Element*>(node), param);
				// Walk through the children nodes and process it.
				for (const Node *child = node->firstChild(); child != NULL;
						child = child->nextSibling())
					BuildContentForNode(child, param);
				// Process end tag of element.
				EndTagToString(static_cast<const Element*>(node), param);
				break;
			}
		case Node::TEXT_NODE:
		   	{
				SaveHtmlContentToBuffer(createMarkup(node), param);
				break;
			}
		case Node::ATTRIBUTE_NODE:
		case Node::DOCUMENT_NODE:
		case Node::DOCUMENT_FRAGMENT_NODE:
			{
				// Should not exist.
				//DCHECK(false);
				break;
			}
			// Document type node can be in DOM?
		case Node::DOCUMENT_TYPE_NODE:
			param->has_doctype = true;
			break;
		default:
			{
				// For other type node, call default action.
				SaveHtmlContentToBuffer(createMarkup(node), param);
				break;
			}
	}
}

DomSerializer::DomSerializer(Frame* webframe, bool recursive_serialization,const char* utf8name)
: recursive_serialization_(recursive_serialization),frames_collected_(false)
{
	// Must specify available webframe.
	m_mainframe = static_cast<Frame*>(webframe);
	// Init data buffer.
	data_buffer_.reserveCapacity(kHtmlContentBufferLength);

	m_savedName=String::fromUTF8(utf8name,strlen(utf8name));
}


void DomSerializer::CollectTargetFrames()
{
	if(frames_collected_ || !m_mainframe)
		return;
	frames_collected_ = true;
	frames_.append(m_mainframe);

	if (!recursive_serialization_)
		return;

	// Collect all frames inside the specified frame.
	for (int i = 0; i < static_cast<int>(frames_.size()); ++i) {
		Frame* current_frame = frames_[i];

		Document* current_doc =  current_frame->document();

		RefPtr<HTMLCollection> all = current_doc->all();
		for (Node* node = all->firstItem(); node != NULL; node = all->nextItem()) {

			if (!node->isHTMLElement())
				continue;
			Element* element = static_cast<Element*>(node);
			// Check frame tag and iframe tag.
			bool is_frame_element;
			Frame* web_frame = GetFrameFromElement(element, &is_frame_element);
			if (is_frame_element && web_frame) {
				//unique check 
				unsigned int i; 
				for (i=0; i< frames_.size(); i++) {
					if(frames_[i] == web_frame )  
						break;
				}
				if (i == frames_.size())
					frames_.append(web_frame);
			}
		}
	}


}

//void DomSerializer::StartDownloadSubresource()
void DomSerializer::StartDownloadSubresourceFromURL(const KURL &url, const String & file)
{
	//printf("%s:%d url=%s, file=%s\n", __FILE__, __LINE__, String(url.string()).utf8().data(), file.utf8().data());
	ResourceRequest request(url, "");
	RefPtr<ResourceHandle> handle = ResourceHandle::create(0, request, new DownloadResourceClient(file), false, false);
	return;
}

//unique check
//type :true url 
//type :false  filename 
int DomSerializer::isExistSameResource(const String & strdata, bool type )
{
	unsigned int i;

	if (type){
		for (i=0; i<subresourceslist.resources_list.size(); i++ ) {
			if (strdata == subresourceslist.resources_list[i].string())  
				return i;
		}
	}
	else {
		for (i=0; i<subresourceslist.local_file.size(); i++ ) {
			if (strdata == subresourceslist.local_file[i])  
				return i;
		}
	}
	return -1;
}

void  DomSerializer::getNewResourceName( String & origResourceName)
{
	if (isExistSameResource(origResourceName, false) < 0)
		return ;
	int i = 0;    
	char post[] = "001";
	String basename = getbaseFilenameAndNOPost(origResourceName);
	String namepost = getFilePost(origResourceName);
	do {
		i++;
		snprintf(post, 4, "%03d", i);
		origResourceName = basename + "_" + post +  namepost;
	}while(isExistSameResource(origResourceName, false) >= 0);

}

bool DomSerializer::SerializeDom()
{
	KURL main_page_kurl(m_mainframe->loader()->baseURL());
	String fullpath(m_savedName); 
	local_directory_name_ = getFilebasePath(fullpath);
	// save html-only
	if (!recursive_serialization_) {
		StartDownloadSubresourceFromURL(main_page_kurl, fullpath);
		return true;
	}

	if (!frames_collected_)
		CollectTargetFrames();
	bool did_serialization = false;

	String tmpSubresource_dirctory = getbaseFilenameAndNOPost(fullpath); 
	local_subresource_directory_name_ = tmpSubresource_dirctory +  "_files";
	String subresourcepath = local_directory_name_+ "/" + local_subresource_directory_name_;
	if (!fileExists(subresourcepath) ) 
		makeAllDirectories(subresourcepath); 

	// Go through all frames for serializing DOM for whole page, include
	// sub-frames.
	for (int i = 0; i < static_cast<int>(frames_.size()); ++i) {
		// Get current serializing frame.
		Frame* current_frame = frames_[i];
		// Get current using document.
		Document* current_doc = current_frame->document();

		did_serialization = true;
		// Get target encoding for current document.
		//String encoding = current_frame->loader()->encoding();
		String encoding("UTF-8");
		// Create the text encoding object with target encoding.
		TextEncoding text_encoding(encoding);
		// Construct serialize parameter for late processing document.
		const KURL& current_frame_kurl = KURL(current_frame->loader()->baseURL());
		//printf("frame url=%s\n", current_frame_kurl.string().ascii()); 
		String filename = m_savedName;
		if (filename.length() <= 0)
			filename = "mdolphin_local_frame.html";
		String filePost = getFilePost(filename);

		if (!(filePost.lower() == ".html") && !(filePost.lower() == ".htm")) {
			filename.append(".html");
		}
#if 0
		int index = isExistSameResource(current_frame_kurl.string(), true);
		if (index < 0 ) {
			getNewResourceName(filename);
			subresourceslist.resources_list.append(current_frame_kurl);
			subresourceslist.local_file.append(filename);
		}
		else {
			filename = subresourceslist.local_file[index]; 
		}
		if (current_frame != m_mainframe) {
			//subresourceslist.resources_list.append(current_frame_kurl);
			//subresourceslist.local_file.append(filename);
			filename = local_directory_name_+ "/" + local_subresource_directory_name_ + "/" + filename;
		}
		else {
			filename = fullpath;
		}

		// 清空文件
		CString cstr =  filename.utf8();
		char file[MAX_FILENAME]; 
		int ret = snprintf(file, sizeof(file)-1, "%s", cstr.data());
		ret = (MAX_FILENAME -1 < ret)? ret:(MAX_FILENAME -1);
		file[ret]='\n';
#endif

		SerializeDomParam param(
				current_frame_kurl,
				UTF8Encoding(),
				current_doc,
				filename);

		// Process current document.
		Element* root_element = current_doc->documentElement();
		if (root_element)
			BuildContentForNode(root_element, &param);

		// check whehter there is a same filename ? 

		//subresourceslist.resources_list.append(url);
		//subresourceslist.local_file.append(savefilename);
		SaveDatatoFile(data_buffer_, filename);
		data_buffer_.clear();
		data_buffer_.reserveCapacity(kHtmlContentBufferLength);
	}

	//StartDownloadSubresource();
	return true;
}

}  // namespace webkit_glue
