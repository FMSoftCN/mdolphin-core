// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MDOLPHIN_DOM_SERIALIZER_H__
#define MDOLPHIN_DOM_SERIALIZER_H__
         
#include <string>

#include "Color.h"
#include "EditAction.h"
#include "DragImage.h"
#include "RenderLayer.h"
#include "TextGranularity.h"
#include "VisiblePosition.h"
#include <wtf/unicode/Unicode.h>
#include <wtf/Forward.h>
#include <wtf/Vector.h>


namespace WebCore {
class Document;
class Element;
class Node;
class TextEncoding;
}

namespace WebCore {

struct SavableResourcesResult {
        // vector which contains all savable links of sub resource.
        Vector<KURL> resources_list;
        // vector which contains corresponding all referral links of sub resource,
        // it matched with links one by one.
        //Vector<KURL>* referrers_list;
        
        Vector<String> local_file;
        // vector which contains all savable links of main frame and sub frames.
        //std::vector<KURL>* frames_list;
};

enum  replaceNodeType {imageType, cssType, jscriptType,frameType, iframeType,  hrefType};

// Get html data by serializing all frames of current page with lists
// which contain all resource links that have local copy.
// contain all saved auxiliary files included all sub frames and resources.
// This function will find out all frames and serialize them to HTML data.
// We have a data buffer to temporary saving generated html data. We will
// sequentially call WebViewDelegate::SendSerializedHtmlData once the data
// buffer is full. See comments of WebViewDelegate::SendSerializedHtmlData
// for getting more information.
class DomSerializer {
 public:
  // Do serialization action. Return false means no available frame has been
  // serialized, otherwise return true.
  bool SerializeDom();
  // The parameter specifies which frame need to be serialized.
  // The parameter recursive_serialization specifies whether we need to
  // serialize all sub frames of the specified frame or not.
  // The parameter delegate specifies the pointer of interface
  // DomSerializerDelegate provide sink interface which can receive the
  // individual chunks of data to be saved.
  // The parameter links contain original URLs of all saved links.
  // The parameter local_paths contain corresponding local file paths of all
  // saved links, which matched with vector:links one by one.
  // The parameter local_directory_name is relative path of directory which
  // contain all saved auxiliary files included all sub frames and resources.
  DomSerializer(Frame* webframe, bool recursive_serialization,const char* savedName);
#if 1
  // Generate the META for charset declaration.
 static String GenerateMetaCharsetDeclaration( const String& charset);

  // Generate the MOTW declaration.
  static String GenerateMarkOfTheWebDeclaration(const KURL& url);
 
  // Generate the default base tag declaration.
  static String GenerateBaseTagDeclaration( const String& base_target);

  int isExistSameResource(const String & strdata , bool type);
  void getNewResourceName(String & origResourceName);
#endif
 private:
  // Specified frame which need to be serialized;
  Frame* m_mainframe;
  // This hash_map is used to map resource URL of original link to its local
  // file path.
  typedef HashMap<String, String> LinkLocalPathMap;
  // local_links_ include all pair of local resource path and corresponding
  // original link.
  LinkLocalPathMap local_links_;
  // Pointer of DomSerializerDelegate
  //DomSerializerDelegate* delegate_;
  // Data buffer for saving result of serialized DOM data.
  //String data_buffer_;
   Vector<UChar> data_buffer_;
  // Passing true to recursive_serialization_ indicates we will serialize not
  // only the specified frame but also all sub-frames in the specific frame.
  // Otherwise we only serialize the specified frame excluded all sub-frames.
  bool recursive_serialization_;
  // Flag indicates whether we have collected all frames which need to be
  // serialized or not;
  bool frames_collected_;
  // Local directory name of all local resource files.
  String  local_directory_name_;
  String  local_subresource_directory_name_;
  String  local_htmlfile_name_;

  // Vector for saving all frames which need to be serialized.
  Vector<Frame *> frames_;
 struct SavableResourcesResult subresourceslist;

  struct SerializeDomParam {
    // Frame URL of current processing document presented by KURL
    const KURL& current_frame_kurl;
    // Current using text encoding object.
    const WebCore::TextEncoding& text_encoding;

    // Document object of current frame.
    WebCore::Document* doc;
    // Local directory name of all local resource files.
    const String& directory_name;

    // Flag indicates current doc is html document or not. It's a cache value
    // of Document.isHTMLDocument().
    bool is_html_document;
    // Flag which indicate whether we have met document type declaration.
    bool has_doctype;
    // Flag which indicate whether will process meta issue.
    bool has_checked_meta;
    // This meta element need to be skipped when serializing DOM.
    const WebCore::Element* skip_meta_element;
    // Flag indicates we are in script or style tag.
    bool is_in_script_or_style_tag;
    // Flag indicates whether we have written xml document declaration.
    // It is only used in xml document
    bool has_doc_declaration;
    // Flag indicates whether we have added additional contents before end tag.
    // This flag will be re-assigned in each call of function
    // PostActionAfterSerializeOpenTag and it could be changed in function
    // PreActionBeforeSerializeEndTag if the function adds new contents into
    // serialization stream.
    bool has_added_contents_before_end;
    // Constructor.
    SerializeDomParam(
        const KURL& current_frame_kurl,
        const WebCore::TextEncoding& text_encoding,
        WebCore::Document* doc,
        const String& directory_name);

  // private:
  //  DISALLOW_EVIL_CONSTRUCTORS(SerializeDomParam);
  };

  // Collect all target frames which need to be serialized.
  void CollectTargetFrames();
  
  // Before we begin serializing open tag of a element, we give the target
  // element a chance to do some work prior to add some additional data.
  WTF::String PreActionBeforeSerializeOpenTag(
      const WebCore::Element* element,
      SerializeDomParam* param,
      bool* need_skip);

  // After we finish serializing open tag of a element, we give the target
  // element a chance to do some post work to add some additional data.
  WTF::String PostActionAfterSerializeOpenTag(
      const WebCore::Element* element,
      SerializeDomParam* param);
  
  // Before we begin serializing end tag of a element, we give the target
  // element a chance to do some work prior to add some additional data.
  WTF::String PreActionBeforeSerializeEndTag( const WebCore::Element* element, 
                                                SerializeDomParam* param, bool* need_skip);

  // After we finish serializing end tag of a element, we give the target
  // element a chance to do some post work to add some additional data.
  WTF::String PostActionAfterSerializeEndTag( const WebCore::Element* element, SerializeDomParam* param);
  // Save generated html content to data buffer.
  void SaveHtmlContentToBuffer(const WTF::String& result, SerializeDomParam* param);
  // Serialize open tag of an specified element.
  void OpenTagToString(const WebCore::Element* element, SerializeDomParam* param);
 
  // Serialize end tag of an specified element.
  void EndTagToString(const WebCore::Element* element,
                      SerializeDomParam* param);
  
  // Build content for a specified node
  void BuildContentForNode(const WebCore::Node* node, SerializeDomParam* param);

  void StartDownloadSubresourceFromURL(const KURL &url, const String & file);
  String m_savedName;
};

}  // namespace webkit_glue

#endif  // WEBKIT_GLUE_DOM_SERIALIZER_H__
