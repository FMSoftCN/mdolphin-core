/*
 ** $Id$
 **
 ** MDWebFrame.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-07 
 */
#include "minigui.h"
#include "mdolphin.h"

#include "config.h"
#include "MDWebFrame.h"
#include "MDWebView.h"

#include "Frame.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "FrameLoader.h"
#include "DocumentLoader.h"
#include "Page.h"
#include "KURL.h"
#include "ResourceRequest.h"
#include "CredentialStorage.h"
#include "ProtectionSpace.h"
#include "Credential.h"
//WebCore header
#include "RenderTreeAsText.h"

#include "FrameLoaderClientMg.h"
#include "MDCommonFunc.h"

using namespace WebCore;
using namespace WTF;

// START_MDWEBFRAMEPRIVATE 
class MDWebFrame::MDWebFramePrivate {
public:
    MDWebFramePrivate() 
        : frame(0)
        , webView(0)
        , name(NULL)
    { 
    }

    ~MDWebFramePrivate() 
    { 
        if (name) 
            free(name);
    }

    WebCore::FrameView* frameView() { return frame ? frame->view() : 0; }

//START_MDWEBFRAMEPRIVATE_DATAMEMBER
    WebCore::Frame* frame;
    MDWebView* webView;
    char *name;
//END_MDWEBFRAMEPRIVATE_DATAMEMBER

};

//START_MDWEBFRAME_CONSTRUCTOR

MDWebFrame::MDWebFrame()
    : d(new MDWebFrame::MDWebFramePrivate)
{
}

MDWebFrame::~MDWebFrame()
{
    delete d;
}

PassRefPtr<Frame> MDWebFrame::init(MDWebView *view, HTMLFrameOwnerElement* ownerElement, FrameLoaderClientMg *client)
{
    ASSERT(view);
    RefPtr<Frame> frame = Frame::create(core(view), ownerElement, client);
    d->frame =  frame.get();
    d->webView = view;


    return frame.release();
}

void MDWebFrame::destroyWebFrame() 
{
    // The FrameLoader going away is equivalent to the Frame going away,
    // so we now need to clear our frame pointer.
    d->frame = 0;     
    this->release(); 
}
//END_MDWEBFRAME_CONSTRUCTOR
MDWebFrame* MDWebFrame::createInstance()
{
    MDWebFrame* instance = new MDWebFrame();
    return instance;
}

static void setCredentialByKURL(const KURL& kurl)
{
    if (!kurl.isValid())
    {
        return;
    }

    ProtectionSpaceServerType serverType; 
    ProtectionSpaceAuthenticationScheme authenticationSchemeconst; 
    if (kurl.protocolIs("http")) {
        serverType = ProtectionSpaceServerHTTP; 
        authenticationSchemeconst = ProtectionSpaceAuthenticationSchemeHTTPBasic;
    }
    else if (kurl.protocolIs("ftp")) {
        serverType = ProtectionSpaceServerFTP; 
        authenticationSchemeconst = ProtectionSpaceAuthenticationSchemeDefault;
    }

    ProtectionSpace protectionSpace(kurl.host(), kurl.port(), serverType, 
            "", authenticationSchemeconst);
    Credential cred(kurl.user(), kurl.pass(), CredentialPersistenceForSession);
    CredentialStorage::set(cred, protectionSpace, kurl);
}

//START_MDWEBFRAME_INTERFACE
bool MDWebFrame::loadURL(const char * sUrl)
{
    stopLoading();
    KURL kurl(ParsedURLString, sUrl);
    if (!kurl.isValid())
        return false;
    if (!kurl.user().isEmpty())
    {
        // save username and password into CredentialStorage
        setCredentialByKURL(kurl);
        // clear username and password in kurl
        kurl.setUser(String());
        kurl.setPass(String());
    }
    ResourceRequest request(kurl);
   // call MDWebView core func
    d->frame->loader()->load(request, false);
    return true;
}

void MDWebFrame::stopLoading(void)
{
    if (Frame* coreFrame = core(this))
        coreFrame->loader()->stopAllLoaders();
    topLevelView()->loadFinished();
}

void MDWebFrame::reload(void)
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return ;

    coreFrame->loader()->reload();
}

void MDWebFrame::loadData(const char* data, int len, const char *mimeType, 
                        const char* encoding, const char *baseURL, const char* unreachableURL)
{
    Frame* coreFrame = core(this);

    KURL baseKURL = baseURL ? KURL(KURL(), WTF::String::fromUTF8(baseURL)) : blankURL();

    ResourceRequest request(baseKURL);

    RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(data, len);
    SubstituteData substituteData(sharedBuffer.release(),
            mimeType ? String::fromUTF8(mimeType) : String::fromUTF8("text/html"),
            encoding ? String::fromUTF8(encoding) : String::fromUTF8("UTF-8"),
            KURL(KURL(), String::fromUTF8(unreachableURL)),
            KURL(KURL(), String::fromUTF8(unreachableURL)));

    coreFrame->loader()->load(request, substituteData, false); 
}

IMDWebView* MDWebFrame::webView() const
{
    return d->webView;
}

MDWebView*  MDWebFrame::topLevelView() const
{
    return d->webView;
}

char* MDWebFrame::url() const
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return NULL;

    return copyStringToUtf8Char(coreFrame->document()->url().string());
}

//END_MDWEBFRAME_INTERFACE


//START_MDWEBFRAME_HELPFUNC

Frame* core(MDWebFrame* webFrame)
{
    if (!webFrame)
        return 0;
    return webFrame->impl();
}

// This function is not in WebFrame.h because we don't want to advertise the ability to get a non-const Frame from a const WebFrame
Frame* core(const MDWebFrame* webFrame)
{
    if (!webFrame)
        return 0;
    return const_cast<MDWebFrame*>(webFrame)->impl();
}

MDWebFrame* kit(WebCore::Frame* frame)
{
    if (!frame)
        return 0;

    FrameLoader *loader = frame->loader();
    if (loader && loader->client())
        return  ((FrameLoaderClientMg *)loader->client())->mdWebFrame();

    return NULL;
}

Frame* MDWebFrame::impl()
{
    return d->frame;
}
//END_MDWEBFRAME_HELPFUNC

char* MDWebFrame::renderTreeAsExternalRepresentation()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return strdup("");

    FrameView* view = coreFrame->view();

    if (view && view->layoutPending())
        view->layout();

    return strdup(externalRepresentation(coreFrame).utf8().data());
}

const char* MDWebFrame::name() const
{
    Frame* coreFrame = core(this);
    if (!coreFrame) 
        return "";
    
    if (d->name)
        return d->name;

    String string = coreFrame->tree()->name();
    d->name = strdup(string.utf8().data());

    return d->name;
}

IMDWebFrame* MDWebFrame::findFrame(const char* name)
{
    if (!name)
        return NULL;

    Frame* coreFrame = core(this);
    if (!coreFrame)
        return NULL;

    String nameString = String::fromUTF8(name);
    return kit(coreFrame->tree()->find(AtomicString(nameString)));
}

IMDWebFrame* MDWebFrame::parentFrame(void)
{
    Frame* coreFrame = core(this);
    if (coreFrame) {
        return kit(coreFrame->tree()->parent());
    }

    return NULL;
}

bool MDWebFrame::childFrames(std::list<IMDWebFrame*>& children)
{
    Frame* coreFrame = core(this);
    if (!coreFrame) 
        return false;

    for (Frame* child = coreFrame->tree()->firstChild(); child;
            child = child->tree()->nextSibling()) {
        FrameLoader* loader = child->loader();
        WebCore::FrameLoaderClientMg* client = 
            static_cast<WebCore::FrameLoaderClientMg*>(loader->client());
        if (client)
            children.push_back(static_cast<IMDWebFrame*>(client->mdWebFrame())); 
    }

    return false;
}

void MDWebFrame::layout()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return;

    FrameView* view = coreFrame->view();
    if (!view)
        return;

    view->layout();
}

char* MDWebFrame::responseMIMEType()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return strdup("");

    WebCore::DocumentLoader* docLoader = coreFrame->loader()->documentLoader();
    String mimeType = docLoader->responseMIMEType();

    return strdup(mimeType.utf8().data());
}

char* MDWebFrame::innerText()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return strdup("");

    FrameView* view = coreFrame->view();

    if (view && view->layoutPending())
        view->layout();

    Element* documentElement = coreFrame->document()->documentElement();
    String string =  documentElement->innerText();
    return strdup(string.utf8().data());
}
