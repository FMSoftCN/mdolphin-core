/*
** $Id: FrameLoaderClientMg.cpp 1474 2008-09-23 01:43:15Z yangyan $
**
** FrameLoaderClientMg.cpp : Frame loader client implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-21
*/
#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "mdolphin_errcode.h"
#include "Frame.h"
#include "FrameView.h"
#include "FrameTree.h"
#include "FrameLoader.h"
#include "CString.h"
#include "FrameLoaderClientMg.h"
#include "FrameNetworkingContextMg.h"
#include "PlatformString.h"
#include "ResourceRequest.h"
#include "ResourceError.h"
#include "DocumentLoader.h"
#include "HTMLFrameElement.h"
#include "HTMLFrameOwnerElement.h"
#include "HTMLNames.h"
#include "MIMETypeRegistry.h"
#include "ResourceResponse.h"
#include "Page.h"
#include "Chrome.h"
#include "ChromeClientMg.h"
#include "ProgressTracker.h"
#include "HistoryItem.h"
#include "LocalizedStrings.h"
#include <wtf/RefPtr.h>
#include "HTTPParsers.h"
#include "Settings.h"
#include "MDolphinEncoding.h"
#include "MIMETypeMg.h"
#include "ScrollbarThemeMg.h"
#include "NotImplemented.h"
#if OS(UNIX)
#include <sys/utsname.h>
#endif

#if ENABLE(PLUGIN)
#include "HTMLPlugInElement.h" 
#include "PluginView.h"
#include "PluginDatabase.h"
//#include "PluginInfoStore.h"
#endif 

#include "MDWebView.h"
#include "MDWebFrame.h"
#include "IMDWebFrameLoadDelegate.h"
#include "IMDWebDownloadDelegate.h"
#include "IMDWebUIDelegate.h"
#include "MDWebDownload.h"
#include "MDResourceRequest.h"
#include "MDResourceResponse.h"
#include "MDDefaultWebPolicyDelegate.h"
#include "MDolphinDelegates.h"
#include "MDNativeBindingManager.h"
#include "MDCommonFunc.h"
#include "AuthenticationChallenge.h"
#include "Credential.h"
#include "CredentialStorage.h"
#include "ProtectionSpace.h"

namespace WebCore {

FrameLoaderClientMg::FrameLoaderClientMg()
    : m_webFrame(0)
    , m_firstData(false)
    , m_loadFinished(false)
{
}

FrameLoaderClientMg::~FrameLoaderClientMg()
{
}

void FrameLoaderClientMg::frameLoaderDestroyed()
{
    m_webFrame->destroyWebFrame();
    m_webFrame = 0;
    delete this;
}

bool FrameLoaderClientMg::hasWebView() const
{
    return true;
}

void FrameLoaderClientMg::setWebFrame (MDWebFrame *webFrame)
{
    m_webFrame = webFrame;
    m_webFrame->addRef();
}

void FrameLoaderClientMg::dispatchDidReceiveResponse(DocumentLoader* loader, unsigned long identifier, const ResourceResponse& response)
{
    m_firstData = true;
   
#if 0 //make sure the bug does not exist
    if ((!loader->isLoadingSubresources()) && (loader->frame() == core(m_webFrame)->page()->mainFrame())) {
        /* bug 2019: after post a form, location is redirected to www.newsmth.org/frames.html */
        if (equalIgnoringCase(loader->request().httpMethod(), "POST"))
            loader->request().setHTTPMethod("GET");
    }
#endif
    if (!m_loadFinished) 
    {
        MDWebView* webView = m_webFrame->topLevelView();
        webView->frameLoadDelegate()->didReceiveResponse(webView, m_webFrame, identifier, (MDHResponse)&response);
    }
}

void FrameLoaderClientMg::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction policyFunction, const NavigationAction& action, 
        const ResourceRequest& req, PassRefPtr<FormState>, const String& frameName)
{
    if (!policyFunction || !core(m_webFrame))
        return;

    MDWebView* webView = m_webFrame->topLevelView();
    IMDWebPolicyDelegate *policyDelegate = webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = MDDefaultWebPolicyDelegate::sharedInstance();

    MDEWebPolicyDecisionResult result;
    result = policyDelegate->decidePolicyForNewWindowAction(webView, (MDENavigationType)action.type(), (MDHRequest)&req, 
            m_webFrame, frameName.utf8().data());
    
    (core(m_webFrame)->loader()->policyChecker()->*policyFunction)((PolicyAction)result);
}

void FrameLoaderClientMg::dispatchDecidePolicyForNavigationAction(FramePolicyFunction policyFunction, const NavigationAction& action,
        const ResourceRequest& req, PassRefPtr<FormState>)
{
    if (!policyFunction || !core(m_webFrame))
        return;

    MDWebView* webView = m_webFrame->topLevelView();
    IMDWebPolicyDelegate *policyDelegate = webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = MDDefaultWebPolicyDelegate::sharedInstance();

    MDEWebPolicyDecisionResult result;
    result = policyDelegate->decidePolicyForNavigationAction(webView, (MDENavigationType)action.type(), (MDHRequest)&req, m_webFrame);
    
    (core(m_webFrame)->loader()->policyChecker()->*policyFunction)((PolicyAction)result);
}

void FrameLoaderClientMg::dispatchWillSubmitForm(FramePolicyFunction policyFunction, PassRefPtr<FormState>)
{
    if (!policyFunction || !core(m_webFrame))
        return;
    (core(m_webFrame)->loader()->policyChecker()->*policyFunction)(PolicyUse);
}

PassRefPtr<DocumentLoader> FrameLoaderClientMg::createDocumentLoader(const ResourceRequest& request, const SubstituteData& substituteData)
{
    RefPtr<DocumentLoader> loader = DocumentLoader::create(request, substituteData);
    if (substituteData.isValid())
        loader->setDeferMainResourceDataLoad(false);
    return loader.release();
}

static String agentOS()
{
#if OS(UNIX)
    struct utsname name;
    if (uname(&name) != -1)
        return String::format("%s %s", name.sysname, name.machine);
    else
        return "Unknown";
#elif OS(ECOS)
    return "ECOS";
#elif CPU(PPC)
    return "PPC";
#else
    return "Unknown";
#endif
}

static String composeUserAgent()
{
    // This is a liberal interpretation of http://www.mozilla.org/build/revised-user-agent-strings.html
    // See also http://developer.apple.com/internet/safari/faq.html#anchor2

    static String ua;
    if (ua.length() > 0) {
        return ua;
    }

    String platform;
    String os;
    String lang;
    String mdolphin;
    String webkit;
    String chrome;
    String safria;

    platform = "miniGUI/";
    platform += String::number(MINIGUI_MAJOR_VERSION);
    platform += ".x;";

    os = agentOS() + ";";
    lang += String("en-US"); // Localization information FIXME: not implement Use "en" for now
    //lang = defaultLanguage() + ";";
    mdolphin = "mDolphin/";
    mdolphin +=  String::number(MDOLPHIN_MAJOR_VERSION);
    mdolphin+=".";
    mdolphin +=  String::number(MDOLPHIN_MINOR_VERSION);

    webkit = "AppleWebKit/";
    webkit +=  String::number(WEBKIT_MAJOR_VERSION);
    webkit+=".";
    webkit +=  String::number(WEBKIT_MINOR_VERSION);

    chrome = "chrome/10.0";
    safria = "Safria/";
    safria +=  String::number(WEBKIT_MAJOR_VERSION);
    safria+=".";
    safria +=  String::number(WEBKIT_MINOR_VERSION);
    
    // Product
    ua += "Mozilla/5.0";

    ua += " (";
    // Comment
    ua += platform; // Platform
    ua += " "; // Platform
    ua += "U; ";  // Security
    ua += os; // OS-or-CPU
    ua += " "; 
    ua += lang; // Localization information
    ua += "; X11)";
    ua += " "; 
    ua += webkit;
    ua += " "; 
    ua += "(KHTML, like Gecko)";
    ua += " "; 
    ua += mdolphin;
    ua += " "; 
    ua += chrome;
    ua += " ";
    ua += safria;

    return ua;
}

String FrameLoaderClientMg::userAgent(const KURL& url)
{
    static String UA;

#ifdef ENABLE_CUSTOM_USERAGENT
    if (Control::MDCB_CUSTOM_USERAGENT)
        return String(Control::MDCB_CUSTOM_USERAGENT(url.string().utf8().data()));
    else {
        if (UA.isEmpty())
            UA = composeUserAgent();
        return UA;
    }
#else
    UA = composeUserAgent();
    return UA;
#endif
}

bool FrameLoaderClientMg::canShowMIMEType(const String& MIMEType) const
{
    if (MIMETypeRegistry::isSupportedImageMIMEType(MIMEType))
        return true;

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(MIMEType))
        return true;

#if ENABLE(PLUGIN)
    if (core(m_webFrame) && core(m_webFrame)->settings()  && core(m_webFrame)->settings()->arePluginsEnabled()
        && PluginDatabase::installedPlugins()->isMIMETypeRegistered(MIMEType))
        return true;
#endif

    return false;
}

void FrameLoaderClientMg::committedLoad(DocumentLoader* loader, const char* data, int length)
{
    //FIXME:lijiangwei
    loader->commitData(data, length);
#if 0
    //TODO: please check if  enable pluginView  

    FrameLoader *fl = loader->frameLoader();
    if (m_firstData) {
        String encoding = loader->overrideEncoding();
        bool userChosen = !encoding.isNull();
        if (encoding.isEmpty())
            encoding = loader->response().textEncodingName();
        fl->writer()->setEncoding(encoding, userChosen);
        m_firstData = false;
    }
#endif
#if 0 //lijiangwei
    fl->addData(data, length);
#endif
}

void FrameLoaderClientMg::willChangeTitle(DocumentLoader* loader)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}

void FrameLoaderClientMg::dispatchDidReceiveTitle(const WebCore::StringWithDirection& title)
{
    MDWebView* webView = m_webFrame->topLevelView();
    char* str = copyStringToUtf8Char(title.string());
    webView->frameLoadDelegate()->didReceiveTitle(webView, m_webFrame, str);
    freeUtf8Char(str);
}

void FrameLoaderClientMg::dispatchDecidePolicyForResponse(FramePolicyFunction policyFunction, const ResourceResponse& response, 
        const ResourceRequest& resourceRequest)
{
    if (!policyFunction || !core(m_webFrame))
        return;

    MDWebView* webView = m_webFrame->topLevelView();
    IMDWebPolicyDelegate *policyDelegate = webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = MDDefaultWebPolicyDelegate::sharedInstance();

    String mimeType = response.mimeType();
    MDEWebPolicyDecisionResult result;
    result = policyDelegate->decidePolicyForMIMEType(webView, mimeType.utf8().data(), (MDHRequest)&resourceRequest, m_webFrame);
    
    (core(m_webFrame)->loader()->policyChecker()->*policyFunction)((PolicyAction)result);
}

bool FrameLoaderClientMg::shouldStopLoadingForHistoryItem(HistoryItem* item) const
{
    return true;
}

void FrameLoaderClientMg::didRunInsecureContent(SecurityOrigin*, const KURL&)
{
    notImplemented();
}

void FrameLoaderClientMg::didChangeTitle(DocumentLoader* l)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}

void FrameLoaderClientMg::setTitle(const StringWithDirection& title, const KURL& url)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}

void FrameLoaderClientMg::didSaveToPageCache()
{
}

void FrameLoaderClientMg::didRestoreFromPageCache()
{
}

void FrameLoaderClientMg::transitionToCommittedForNewPage()
{
    ASSERT(core(m_webFrame));                                                                                                          
       
    HWND hwnd = NULL;
    if (core(m_webFrame))                            
        hwnd = core(m_webFrame)->page()->chrome()->platformPageClient();

    if (!hwnd)
        return;

    RECT rc;
    GetClientRect(hwnd, &rc);
    IntSize size(RECTW(rc), RECTH(rc));
    // FIXME: This value should be gotten from m_webView->IsTransparent();     
    // but transitionToCommittedForNewPage() can be called while m_webView is            
    // still being initialized.     
    bool transparent = false;                  
    Color backgroundColor = transparent ? WebCore::Color::transparent : WebCore::Color::white;    
                                                                                                     
    if (core(m_webFrame)) {                            
        MDScrobarWidth *customScrollbarWidth = ScrollbarThemeMg::scrollbarCustomWidth();
        if (customScrollbarWidth->wRegular < 0)
            core(m_webFrame)->createView(size, backgroundColor, transparent, IntSize(), 
                                false, ScrollbarAlwaysOff ,true,  ScrollbarAlwaysOff, true); 
        else
            core(m_webFrame)->createView(size, backgroundColor, transparent, IntSize(), false); 
    }
}

PassRefPtr<Frame> FrameLoaderClientMg::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                   const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{

    MDWebFrame* webFrame = MDWebFrame::createInstance();
    FrameLoaderClientMg *client = new FrameLoaderClientMg();
    RefPtr<Frame> frame = webFrame->init(m_webFrame->topLevelView(), ownerElement, client);

    client->setWebFrame(webFrame);
    webFrame->release();
    //WTF::PassRefPtr<WebCore::Frame> frame = Frame::create(core(m_webFrame)->page(), ownerElement, client);

    //WTF::PassRefPtr<WebCore::FrameView> frameView = FrameView::create(frame.get());
    //frame->setView(frameView.releaseRef());
    //frameView->setPlatformWidget(core(m_webFrame)->view()->platformWidget());
    //frameView->deref();
    //frame->init();

    //RefPtr<Frame> childFrame(adoptRef(frame.get()));
    RefPtr<Frame> childFrame=frame;

    //childFrame->view()->setScrollbarsMode(modeScrolling);
#if 0
    if (marginWidth != -1)
        childFrame->view()->setMarginWidth(marginWidth);
    if (marginHeight != -1)
        childFrame->view()->setMarginHeight(marginHeight);
#endif

    core(m_webFrame)->tree()->appendChild(childFrame);
    childFrame->tree()->setName(name);
    childFrame->init();


    // The creation of the frame may have run arbitrary JavaScript that removed it from the page already.
    if (!childFrame->page())
        return 0;

    childFrame->loader()->loadURLIntoChildFrame(url, referrer, childFrame.get());

#if 0
    FrameLoadType childLoadType;// = FrameLoadTypeRedirectWithLockedHistory;
    //childFrame->loader()->load(url, referrer, childLoadType, String(), 0, 0);
#endif

    if (!childFrame->tree()->parent())
        return 0;

    return childFrame.release();
}
#if 0
PassRefPtr<Frame> FrameLoaderClientMg::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
            const String& referrer, ScrollbarMode modeScrolling, int marginWidth, int marginHeight)
{
    FrameLoaderClientMg *client = new FrameLoaderClientMg;
    Frame* frame;// = new Frame(core(m_webFrame)->page(), ownerElement, client);
    client->setFrame(frame);
    FrameView* frameView; // = new FrameView(frame);
    frame->setView(frameView);
    //frameView->setContainingWindow(HWND_NULL);
    frameView->deref();
    frame->init();

    RefPtr<Frame> childFrame(adoptRef(frame));

    //childFrame->view()->setScrollbarsMode(modeScrolling);
    if (marginWidth != -1)
        childFrame->view()->setMarginWidth(marginWidth);
    if (marginHeight != -1)
        childFrame->view()->setMarginHeight(marginHeight);

    core(m_webFrame)->tree()->appendChild(childFrame);
    childFrame->tree()->setName(name);
    FrameLoadType childLoadType;// = FrameLoadTypeRedirectWithLockedHistory;
    //childFrame->loader()->load(url, referrer, childLoadType, String(), 0, 0);

    if (!childFrame->tree()->parent())
        return 0;

    return childFrame.release();
}
#endif

PassRefPtr<Widget> FrameLoaderClientMg::createPlugin(const IntSize& pluginSize, HTMLPlugInElement* ownerElement, const KURL& URL, 
                                            const Vector<String>& paramNames, const Vector<String>& paramValues,
		                                    const String& mimeType, bool loadManually)
{

#if ENABLE(PLUGIN)
    LOG(Plugins, "FrameLoaderClientMg::createPluginView");
    RefPtr<PluginView> pluginView = PluginView::create(core(m_webFrame), pluginSize, ownerElement, URL, paramNames, paramValues, mimeType, loadManually);

    //if (pluginView->status() == PluginStatusLoadedSuccessfully)
        return pluginView;
#else
    return NULL;
#endif //ENABLE(PLUGIN)
}

void FrameLoaderClientMg::forceLayout()
{
    core(m_webFrame)->view()->setNeedsLayout();
    core(m_webFrame)->view()->layout();
}

void FrameLoaderClientMg::postProgressStartedNotification()
{
	if (core(m_webFrame)) { 
#if 0
		MDWebView * webView = m_webFrame->topLevelView();
		if (webView)
			webView->setLoadingStatus(true, 0);
#endif
        MDWebView* webView = m_webFrame->topLevelView();
        webView->frameLoadDelegate()->didProgressStarted(webView, m_webFrame);
        m_loadFinished = false;
#ifdef _MD_ENABLE_LOADSPLASH
        HWND hWnd = webView->viewWindow();
        HDC hdc = GetClientDC(hWnd);
        webView->drawLoadSplash(hdc);
        ReleaseDC(hdc);
#endif
	}
}

void FrameLoaderClientMg::postProgressEstimateChangedNotification()
{
	if (core(m_webFrame) && core(m_webFrame)->page()) {
		int percent = (int)(m_webFrame->topLevelView()->estimatedProgress()*100);

#if 0
		MDWebView * webView = m_webFrame->topLevelView();
		if (webView)
			webView->setLoadingStatus(true, percent);
#endif
        MDWebView* webView = m_webFrame->topLevelView();
        webView->frameLoadDelegate()->didProgressEstimateChanged(webView, m_webFrame, percent);        
#ifdef _MD_ENABLE_LOADSPLASH
        HWND hWnd = webView->viewWindow();
        HDC hdc = GetClientDC(hWnd);
        webView->drawLoadSplash(hdc);
        ReleaseDC(hdc);
#endif
	}
}

void FrameLoaderClientMg::postProgressFinishedNotification()
{
	if (core(m_webFrame)) {
		MDWebView * webView = m_webFrame->topLevelView();
		if (webView)
			webView->loadFinished();
        m_loadFinished = true;
        webView->frameLoadDelegate()->didProgressFinished(webView, m_webFrame); 
	}
}

bool FrameLoaderClientMg::shouldGoToHistoryItem(HistoryItem* item) const
{
    if(item) 
        return true;

    return false;
}

void FrameLoaderClientMg::assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const ResourceRequest& request)
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->identifierForInitialRequest(webView, m_webFrame
                                        ,(MDHRequest)&request, identifier); 
    //nothing to do
}

void FrameLoaderClientMg::frameLoadCompleted()
{
    notImplemented();
    //core(m_webFrame)->loader()->setPreviousHistoryItem(0);
}

void FrameLoaderClientMg::setMainFrameDocumentReady(bool)
{
    //nothing to do
}

Frame* FrameLoaderClientMg::dispatchCreatePage(const NavigationAction&)
{
    IMDWebView* webView = m_webFrame->webView();
    if (!webView)
        return NULL;

    MDWindowFeatureInfo  featureInfo;
    MDFrameLoaderInfo frameLoaderInfo;
    IMDWebUIDelegate *uiDelegate = webView->uiDelegate();

    frameLoaderInfo.url = "about:blank"; 
    memset(&featureInfo, 0, sizeof(featureInfo));

    if (uiDelegate) {
            featureInfo.x = 0; 
            featureInfo.y = 0;
            featureInfo.width = -1; //normal size
            featureInfo.height = -1; //normal size
            featureInfo.toolBarVisible = 1;
            featureInfo.locationBarVisible = 1;
            featureInfo.statusBarVisible = 1;
            featureInfo.scrollbarsVisible = 1;
            featureInfo.menuBarVisible = 1;
            featureInfo.resizeable = 1;
            featureInfo.fullScreen = 0;
            featureInfo.dialog = 0;

        HWND newHwnd = uiDelegate->createWebViewWithRequest(webView, &frameLoaderInfo, &featureInfo);
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(newHwnd);
        if (webView){
            return core(webView->topLevelFrame());
       }  else
            return NULL;
    }
    else
        return NULL;
}

void FrameLoaderClientMg::dispatchDidFailProvisionalLoad(const ResourceError& err)
{
#if !ENABLE(FAST_MODE)
    dispatchDidFailLoad(err);
#endif
}

void FrameLoaderClientMg::dispatchDidFailLoad(const ResourceError& err)
{
#if !ENABLE(FAST_MODE)
    // For bug 5468: core if there is MessageBox.
    // It is WebKit re-entry problem.
    // We use SendNotifyMessage to not block the message.
    MDWebView* webView = m_webFrame->topLevelView();
    HWND mdolphin_hwnd = webView->viewWindow();
	//because in Asynchronous mode ,the err object may be deconstruct，
	//so ,we must deep copy a error . 
	ResourceError* deepCopyError=new ResourceError;
	*deepCopyError=err.copy();
    SendNotifyMessage(mdolphin_hwnd, MSG_MDCORE, (WPARAM)deepCopyError, (LPARAM)"load-error");

#if 0
    // FIXME: No need this?
    // We should do something more like GTK.
    String content = String::format("<html><body>%s</body></html>", err.localizedDescription().utf8().data());
    m_webFrame->loadData(content.utf8().data(), content.utf8().length(), 0, 0, 0, err.failingURL().utf8().data());
#endif
#endif
}

void FrameLoaderClientMg::dispatchDidFirstLayout()
{
    //TODO: we need reset global view values at there
}

ObjectContentType FrameLoaderClientMg::objectContentType(const KURL& url, const String& _mimeType, bool shouldPreferPlugInsForImages)
{
    //FIXME: Why should NOT we use defaultObjectContentType?
    //Maybe it is better.
    if (url.isEmpty())
        return ObjectContentNone;

    String mimeType = _mimeType;
    if (! mimeType.length())
        mimeType = MIMETypeRegistry::getMIMETypeForPath(url.string());

    if (! mimeType.length())
        return ObjectContentFrame;

    if (MIMETypeRegistry::isSupportedImageMIMEType(mimeType))
        return ObjectContentImage;

#if ENABLE(PLUGIN)
    if (PluginDatabase::installedPlugins()->isMIMETypeRegistered(mimeType))
        return ObjectContentNetscapePlugin;
#endif

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(mimeType))
        return ObjectContentFrame;

    if (url.protocol() == "about")
        return ObjectContentFrame;

    return ObjectContentNone;
}

bool FrameLoaderClientMg::canHandleRequest(const ResourceRequest& req) const
{
    return true;
    //if(isSupportedInternalScheme(req.url().protocol().latin1()))
    //    return true;
    //return false;
}

void FrameLoaderClientMg::makeRepresentation(DocumentLoader*)
{
    notImplemented();
}

void FrameLoaderClientMg::forceLayoutForNonHTML()
{
    notImplemented();
}

void FrameLoaderClientMg::setCopiesOnScroll()
{
    notImplemented();
}

void FrameLoaderClientMg::detachedFromParent2()
{
    notImplemented();
}

void FrameLoaderClientMg::detachedFromParent3()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchWillSendRequest(DocumentLoader*, unsigned long identifier, ResourceRequest& res, 
                                                                                     const ResourceResponse& redirectResponse)
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidReceiveAuthenticationChallenge(DocumentLoader* docLoader, unsigned long identifier, 
        const AuthenticationChallenge& challenge)
{
    LOG(Loading, "FrameLoaderClientMg - dispatchDidReceiveAuthenticationChallenge %p", &challenge);
    char user[NAME_MAX] = {0};
    char pass[NAME_MAX] = {0};
    bool isOk = false;

    IMDWebView* webView = m_webFrame->webView();
    IMDWebUIDelegate *uiDelegate = webView->uiDelegate();
    ASSERT(uiDelegate);
    
    String strTitle = challenge.failureResponse().httpHeaderField("WWW-Authenticate");
    int pos = strTitle.find('=');
    if (pos != -1) 
        strTitle = strTitle.substring(pos+1, strTitle.length()-pos-1);
    if (strTitle.isNull())
    {
        strTitle = challenge.failureResponse().url();
    }
    isOk = uiDelegate->receivedAuthentication(webView, strTitle.utf8().data(), user, NAME_MAX, pass, NAME_MAX);
    CredentialPersistence persistence = CredentialPersistenceNone;
    if (isOk) {
        persistence = CredentialPersistenceForSession;
    }
    Credential cred(String(user), String(pass), persistence);
    const ProtectionSpace protectionSpace = challenge.protectionSpace();
    //CredentialStorage::set(cred, protectionSpace, docLoader->url());
    //FIXME:lijiangwei
    CredentialStorage::set(cred, protectionSpace, challenge.failureResponse().url());
}

void FrameLoaderClientMg::dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long identifier, 
                                                                                      const AuthenticationChallenge&)
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidReceiveContentLength(DocumentLoader*, unsigned long identifier, int lengthReceived)
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidFinishLoading(DocumentLoader*, unsigned long identifier)
{
    LOG(Loading, "FrameLoaderClientMg - dispatchDidFinishLoading ");
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidFailLoading(DocumentLoader* loader, unsigned long identifier, const ResourceError&)
{
    LOG(Loading, "FrameLoaderClientMg - dispatchDidFailLoading ");

#if 0
    if (m_firstData) {
        FrameLoader *fl = loader->frameLoader();
        fl->writer()->setEncoding(loader->response().textEncodingName(), false);
        m_firstData = false;
    }
#endif
}

bool FrameLoaderClientMg::dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&,
                                                                                  const ResourceResponse&, int length)
{
    notImplemented();
    return false;
}

void FrameLoaderClientMg::dispatchDidHandleOnloadEvents()
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didHandleOnloadEventsForFrame(webView, m_webFrame);
}

void FrameLoaderClientMg::dispatchDidReceiveServerRedirectForProvisionalLoad()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidCancelClientRedirect()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchWillPerformClientRedirect(const KURL&, double, double)
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidChangeLocationWithinPage()
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didChangeLocationWithinPageForFrame(webView, m_webFrame);
}

void FrameLoaderClientMg::dispatchWillClose()
{
    LOG(Loading, "FrameLoaderClientMg - dispatchWillClose  ");
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidReceiveIcon()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidStartProvisionalLoad()
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didStartProvisionalLoadForFrame(webView, m_webFrame);
}

void FrameLoaderClientMg::dispatchDidCommitLoad()
{
    if (core(m_webFrame)->tree()->parent() || !m_webFrame)
        return;
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didCommitLoadForFrame(webView, m_webFrame);
}

void FrameLoaderClientMg::dispatchDidFinishDocumentLoad()
{
    if (core(m_webFrame)->tree()->parent() || !m_webFrame)
        return;
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didFinishDocumentLoad(webView, m_webFrame); 
}

void FrameLoaderClientMg::dispatchDidFinishLoad()
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didFinishLoadForFrame(webView,m_webFrame);
}


void FrameLoaderClientMg::dispatchShow()
{
    notImplemented();
}

void FrameLoaderClientMg::cancelPolicyCheck()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchUnableToImplementPolicy(const ResourceError&)
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidLoadMainResource(DocumentLoader*)
{
    notImplemented();
}

void FrameLoaderClientMg::revertToProvisionalState(DocumentLoader*)
{
    notImplemented();
}

void FrameLoaderClientMg::clearUnarchivingState(DocumentLoader*)
{
    notImplemented();
}

void FrameLoaderClientMg::setMainDocumentError(DocumentLoader* loader, const ResourceError& error)
{
#if 0
    //TODO: please check if  enable pluginView  
    if (m_firstData) {
        loader->frameLoader()->writer()->setEncoding(loader->response().textEncodingName(), false);
        m_firstData = false;
    }
#endif
}

void FrameLoaderClientMg::startDownload(const ResourceRequest& request)
{
    MDWebView* webView = m_webFrame->topLevelView();
    webView->downloadURL(request.url());
}

void FrameLoaderClientMg::finishedLoading(DocumentLoader* documentLoader)
{
    //TODO: please check if  enable pluginView  

    //FrameLoader* loader = documentLoader->frameLoader();
    //loader->writer()->setEncoding(documentLoader->response().textEncodingName(), false);
    documentLoader->writer()->setEncoding(documentLoader->response().textEncodingName(), false);
    m_firstData = false; 
}

void FrameLoaderClientMg::updateGlobalHistoryForStandardLoad(const KURL&)
{
    notImplemented();
}

void FrameLoaderClientMg::updateGlobalHistoryForReload(const KURL&)
{
    notImplemented();
}

ResourceError FrameLoaderClientMg::cancelledError(const ResourceRequest& request)
{
    return ResourceError("MgNetwork", MDEC_NET_OPERATION_CANCELED, request.url().prettyURL(),
            "the opperation is canceled");
}

ResourceError FrameLoaderClientMg::blockedError(const ResourceRequest& request)
{
    return ResourceError("MgNetWork", MDEC_NET_REQUEST_BLOCKED, request.url().prettyURL(),
            "Request blocked");
}

ResourceError FrameLoaderClientMg::cannotShowURLError(const ResourceRequest& request)
{
    return ResourceError("MgNetwork", MDEC_NET_URL_ERROR, request.url().prettyURL(),
            "Cannot Show URL");
}

ResourceError FrameLoaderClientMg::interruptForPolicyChangeError(const ResourceRequest& request)
{
    return ResourceError("MgNetwork", MDEC_NET_INTERRUPTED_FOR_POLICY_CHANGE, request.url().prettyURL(),
            "Interrupt for policy change");
}

ResourceError FrameLoaderClientMg::cannotShowMIMETypeError(const ResourceResponse& request)
{
    return ResourceError("MgNetwork", MDEC_NET_UNKNOWN_MIMETYPE, request.url().prettyURL(),
            "Unknown MIME type");
}

ResourceError FrameLoaderClientMg::fileDoesNotExistError(const ResourceResponse& request)
{
    return ResourceError("MgNetwork", MDEC_NET_FILE_DOESNOT_EXIST, request.url().prettyURL(),
            "File doest not exist");
}

bool FrameLoaderClientMg::shouldFallBack(const ResourceError&)
{
    notImplemented();
    return false;
}

void FrameLoaderClientMg::setDefersLoading(bool)
{
    notImplemented();
}

bool FrameLoaderClientMg::willUseArchive(ResourceLoader*, const ResourceRequest&, const KURL& originalURL) const
{
    notImplemented();
    return false;
}

bool FrameLoaderClientMg::isArchiveLoadPending(ResourceLoader*) const
{
    notImplemented();
    return false;
}

void FrameLoaderClientMg::cancelPendingArchiveLoad(ResourceLoader*)
{
    notImplemented();
}

void FrameLoaderClientMg::clearArchivedResources()
{
    notImplemented();
}

bool FrameLoaderClientMg::representationExistsForURLScheme(const String&) const
{
    notImplemented();
    return false;
}

String FrameLoaderClientMg::generatedMIMETypeForURLScheme(const String&) const
{
    notImplemented();
    return String();
}

void FrameLoaderClientMg::saveViewStateToItem(HistoryItem*)
{
    notImplemented();
}

void FrameLoaderClientMg::restoreViewState()
{
    notImplemented();
}

void FrameLoaderClientMg::provisionalLoadStarted()
{
    notImplemented();
}

void FrameLoaderClientMg::didFinishLoad()
{
    notImplemented();
}

void FrameLoaderClientMg::prepareForDataSourceReplacement()
{
    notImplemented();
}

void FrameLoaderClientMg::saveDocumentViewToCachedPage(CachedPage*)
{
    notImplemented();
}

bool FrameLoaderClientMg::canCachePage() const
{
    notImplemented();
    return false;
}

void FrameLoaderClientMg::download(ResourceHandle* handle, const ResourceRequest& request, const ResourceRequest&, 
    const ResourceResponse& response)
{
    //
    IMDWebDownloadDelegate* downloadDelegate = 0;
    MDWebView* webView;
    if (webView = m_webFrame->topLevelView()) {
        downloadDelegate = webView->downloadDelegate();
        if (!downloadDelegate) {
            LOG_ERROR("Failed to get downloadDelegate from WebView");
            downloadDelegate = 0; 
        }
    }
    MDWebDownload* download = MDWebDownload::createInstance(handle, request, response, downloadDelegate);
    //
    download->start();
    download->release();
}

void FrameLoaderClientMg::redirectDataToPlugin(Widget* pluginWidget)
{
    notImplemented();
}

//FIXME: add by lvlei
PassRefPtr<Widget> FrameLoaderClientMg::createJavaAppletWidget(const IntSize& pluginSize, 
                HTMLAppletElement* element, const KURL& baseURL,
                const Vector<String>& paramNames, const Vector<String>& paramValues)
{
#if ENABLE(PLUGIN)
    return createPlugin(pluginSize, (HTMLPlugInElement *)(element), baseURL, paramNames, paramValues, "application/x-java-applet", false);
#endif
}

String FrameLoaderClientMg::overrideMediaType() const
{
    notImplemented();
    return String();
}

void FrameLoaderClientMg::windowObjectCleared() const
{
    notImplemented();
}

bool FrameLoaderClientMg::shouldUseCredentialStorage(DocumentLoader*, unsigned long identifier)
{
    notImplemented();
    return false;
}

void FrameLoaderClientMg::dispatchDidPushStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidReplaceStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidPopStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidChangeIcons()
{
    notImplemented();
}

void FrameLoaderClientMg::dispatchDidFirstVisuallyNonEmptyLayout()
{
    notImplemented();
}

ResourceError FrameLoaderClientMg::pluginWillHandleLoadError(const ResourceResponse&)
{
    notImplemented();
    return ResourceError();
}

void FrameLoaderClientMg::didPerformFirstNavigation() const
{
    if (core(m_webFrame)->tree()->parent() || !m_webFrame)
        return;
    MDWebView* webView = m_webFrame->topLevelView();
    webView->frameLoadDelegate()->didPerformFirstNavigation(webView, m_webFrame); 
}

void FrameLoaderClientMg::dispatchDidClearWindowObjectInWorld(DOMWrapperWorld* world) 
{ 
    LOG(Loading, "FrameLoaderClientMg - dispatchDidClearWindowObjectInWorld %p, %p\n", world, mainThreadNormalWorld());
#if ENABLE(JSNATIVEBINDING)
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    instance->registerNativeJSObjectsToContext(core(m_webFrame)->script(), world);
#endif

    MDWebView* webView = m_webFrame->topLevelView();

    DOMWrapperWorld* nativeWorld = (world)? world : mainThreadNormalWorld();
    JSDOMWindow*    window =  (core(m_webFrame)->script()->globalObject(nativeWorld));
    //toJSDOMWindow
    JSContextRef context = reinterpret_cast<JSContextRef>(window->globalExec()); 
    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    webView->frameLoadDelegate()->didClearWindowObject(webView, m_webFrame
                        ,context, globalObject); 
}

void FrameLoaderClientMg::dispatchDidChangeCaret(const IntRect* rect)
{
    RECT caret;
    SetRect(&caret, rect->x(), rect->y(), rect->x()+rect->width(), rect->y()+rect->height());
    IMDWebView* webView = m_webFrame->webView();
    IMDWebUIDelegate *uiDelegate = webView->uiDelegate();
    
    uiDelegate->didChangeCaret(webView, &caret);
}

void  FrameLoaderClientMg::dispatchWillApplyCSSVisitedLink(const KURL &kurl)
{
    IMDWebView* webView = m_webFrame->webView();
    IMDWebCustomDelegate *customDelegate = webView->customDelegate();
    ASSERT(customDelegate);
    customDelegate->linkURL(kurl.string().utf8().data());
}

PassRefPtr<FrameNetworkingContext> FrameLoaderClientMg::createNetworkingContext()
{
    return FrameNetworkingContextMg::create(core(m_webFrame));
}


}
