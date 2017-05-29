/*
** $Id: ChromeClientMg.cpp 1160 2008-04-11 01:49:17Z whzeng $
**
** ChromeClientMg.cpp: Chrome Client implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-5
*/
#include "minigui.h"
#include "mdolphin.h"

#include "config.h"
#include "ChromeClientMg.h"

#include "Frame.h"
#include "Page.h"
#include "FrameView.h"
#include "FrameLoader.h"
#include "CString.h"
#include "FrameLoadRequest.h"
#include "MDWebView.h"
#include "HitTestResult.h"
#include "NotImplemented.h"
#include "Chrome.h"
#include "PopupMenuMg.h"
#include "SearchPopupMenuMg.h"

#include "MDolphinDelegates.h"
#include "MDCommonFunc.h"
#include <wtf/Vector.h>

#if ENABLE(DATABASE)
#include "DatabaseTracker.h"
#endif

namespace WebCore
{
    
ChromeClientMg::ChromeClientMg(MDWebView* webView)
    :m_webView(webView)
    ,m_lastHoverElement(0)
    ,m_lastTooltipId(0)
    ,m_focused(false)
{
    // default window features.
#if 0
    RECT rcClient;
    GetClientRect(m_webView->viewWindow(), &rcClient);
    m_features.width = RECTW(rcClient);
    m_features.height = RECTH(rcClient);

    m_features.x = 0.0f;
    m_features.xSet = false;
    m_features.y = 0.0f;
    m_features.ySet = false;
    m_features.widthSet = false;

    m_features.heightSet = false;
    m_features.menuBarVisible = true;
    m_features.statusBarVisible = true;
    m_features.toolBarVisible = true;
    m_features.locationBarVisible = true;
    m_features.scrollbarsVisible = true;
    m_features.resizable = true;
    m_features.fullscreen = false;
    m_features.dialog = false;
#endif
}

ChromeClientMg::~ChromeClientMg()
{
    
}

void ChromeClientMg::focusedNodeChanged(Node*)
{
    //if (!m_focused) {
    m_focused = true;
    SetFocus(m_webView->viewWindow());
    //}
}

void ChromeClientMg::runJavaScriptAlert(Frame* frame, const String& message)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->runJavaScriptAlertPanelWithMessage(m_webView, message.utf8().data());
}

bool ChromeClientMg::runJavaScriptConfirm(Frame* frame, const String& message)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    return uiDelegate->runJavaScriptConfirmPanelWithMessage(m_webView, message.utf8().data());
}

bool ChromeClientMg::runJavaScriptPrompt(Frame* frame, const String& message, const String& defaultValue, String& result)
{
    char * ret = NULL;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret =  uiDelegate->runJavaScriptTextInputPanelWithPrompt(m_webView, 
            message.utf8().data(), (char*)defaultValue.utf8().data());

    if (ret){
        result = ret;
        return true;
    } 
    else
        return false; 
}

bool ChromeClientMg::tabsToLinks() const
{
    return true;
}

Page* ChromeClientMg::createWindow(Frame* frame, const FrameLoadRequest& request, const WindowFeatures& features, const NavigationAction&)
{
    MDWindowFeatureInfo  featureInfo;
    MDFrameLoaderInfo frameLoaderInfo;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();

    frameLoaderInfo.url = copyStringToUtf8Char(request.resourceRequest().url().string());
    memset(&featureInfo, 0, sizeof(featureInfo));

    ASSERT(uiDelegate); 
    {
        if (features.xSet) 
            featureInfo.x = (int)ceil(features.x); 

        if (features.ySet)
            featureInfo.y = (int)ceil(features.y);

        if (features.widthSet)
            featureInfo.width = (int)ceil(features.width);
        else
            featureInfo.width = -1; //normal size

        if (features.heightSet)
            featureInfo.height = (int)ceil(features.height);
        else
            featureInfo.height = -1; //normal size

        if (features.toolBarVisible)
            featureInfo.toolBarVisible = 1;

        if (features.locationBarVisible)
            featureInfo.locationBarVisible = 1;

        if (features.statusBarVisible)
            featureInfo.statusBarVisible = 1;

        if (features.scrollbarsVisible)
            featureInfo.scrollbarsVisible = 1;

        if (features.menuBarVisible)
            featureInfo.menuBarVisible = 1;

        if (features.resizable)
            featureInfo.resizeable = 1;

        if (features.fullscreen)
            featureInfo.fullScreen = 1;

        if (features.dialog)
            featureInfo.dialog = 1;

        HWND newHwnd = uiDelegate->createWebViewWithRequest(m_webView, &frameLoaderInfo, &featureInfo);
        freeUtf8Char((void*)frameLoaderInfo.url);

        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(newHwnd);
        if (webView) {
            Page * page = webView->page();
			if(!request.isEmpty())
				page->mainFrame()->loader()->load(request.resourceRequest(),false);
            return page;
        }else
            return NULL;
    }

}

void ChromeClientMg::show()
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->webViewShow(m_webView);
}

bool ChromeClientMg::canRunModal()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->canRunModal(m_webView);
    return ret;
}

void ChromeClientMg::closeWindowSoon()
{

    //m_webView->setGroupName(0);
    m_webView->stopLoading();
    //m_webView->closeWindowSoon();

    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->webViewClose(m_webView);
}

void ChromeClientMg::setToolbarsVisible(bool visible)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setBarVisible(m_webView, MD_TOOLBAR, visible); 
}

bool ChromeClientMg::toolbarsVisible()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->isBarVisible(m_webView, MD_TOOLBAR);
    return ret;
}

void ChromeClientMg::setStatusbarVisible(bool visible)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setBarVisible(m_webView, MD_STATUSBAR, visible); 
}

bool ChromeClientMg::statusbarVisible()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->isBarVisible(m_webView, MD_STATUSBAR);
    return ret;
}

void ChromeClientMg::setScrollbarsVisible(bool visible)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setBarVisible(m_webView, MD_SCROLLBAR, visible); 
}

bool ChromeClientMg::scrollbarsVisible()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->isBarVisible(m_webView, MD_STATUSBAR);
    return ret;
}

void ChromeClientMg::setMenubarVisible(bool visible)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setBarVisible(m_webView, MD_MENUBAR, visible); 
}

bool ChromeClientMg::menubarVisible()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->isBarVisible(m_webView, MD_MENUBAR);
    return ret;
}

void ChromeClientMg::setResizable(bool resizeable)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setResizeable(m_webView, resizeable);
}

FloatRect ChromeClientMg::windowRect()
{
    BOOL ret = FALSE;
    RECT rc ;

    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->webViewFrame(m_webView, &rc);

    if (ret)
        return FloatRect(rc.left, rc.right, RECTW(rc), RECTH(rc));
    else 
        return FloatRect();
}

void ChromeClientMg::mouseDidMoveOverElement(const HitTestResult& result, unsigned modifierFlags)
{
    Element *link = result.URLElement();
    // check to see if it is the same as last
    if (link != m_lastHoverElement) {
        m_lastHoverElement = link;

        IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
        ASSERT(uiDelegate);
        uiDelegate->mouseDidMoveOverElement(m_webView, result.absoluteLinkURL().prettyURL().utf8().data()); 
    }
}

bool ChromeClientMg::paintCustomScrollCorner(GraphicsContext* context, const FloatRect& rect)
{
    if (context->paintingDisabled())
        return false;

    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    if (!uiDelegate)
        return false;

    RECT webRect = enclosingIntRect(rect);
    HDC hdc = *(context->platformContext());
    if (!hdc)
        return false;

    BOOL ret = uiDelegate->paintCustomScrollCorner(m_webView, hdc, webRect);
    return ret ? true : false;
}

void ChromeClientMg::scrollbarsModeDidChange() const
{
    notImplemented();
}

void ChromeClientMg::setToolTip(const WTF::String& text, WebCore::TextDirection direction)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);

    unsigned int ns = 0;
    const char *pText = NULL;
    BOOL bShow = !text.isEmpty();
    CString ctext = text.utf8();
    if (bShow)
        pText = ctext.data();

    BOOL canSetTip = (bShow && ((ns = text.impl()->hash()) != m_lastTooltipId)) || (!bShow);
    
    if (canSetTip) {
            uiDelegate->setToolTip(m_webView, pText, (int) direction); 
        /*
        if (bShow)
            uiDelegate->setToolTip(m_webView, text.utf8().data(), (int) direction); 
        else
            uiDelegate->setToolTip(m_webView, NULL, (int) direction); 
        */

        m_lastTooltipId = ns;
    }
}

#if 0
void ChromeClientMg::setToolTip(const String& str, const IntPoint & mouse_point)
{
    unsigned int ns = 0;
    BOOL bShow = !str.isEmpty();

    BOOL canSetTip = (bShow && ((ns = str.impl()->hash()) != m_lastTooltipId)) || (!bShow);
    if (canSetTip && Control::MDCB_SET_TOOLTIP) {
        Control::MDCB_SET_TOOLTIP(m_webView->viewWindow(), mouse_point.x(), mouse_point.y(), str.utf8().data(), bShow); 
        m_lastTooltipId = ns;
    }
}
#endif

void ChromeClientMg::focus()
{
    //TODO: check it ? 
    //if (m_focused)
    //    return;
    m_focused = true;
    SetFocus(m_webView->viewWindow());

    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->webViewFocus(m_webView, true);
}


void ChromeClientMg::unfocus()
{
    if (m_focused) {
        m_focused = false;
        SendMessage(m_webView->viewWindow(), MSG_KILLFOCUS, 0, 0); //FIXME: this is very badness to send message, because minigui have not KillFocus 
    }
}

FloatRect ChromeClientMg::pageRect()
{
    RECT rc;
    GetWindowRect(m_webView->viewWindow(), &rc);
    return FloatRect(rc.left, rc.right, RECTW(rc), RECTH(rc));
}

//not implement.
float ChromeClientMg::scaleFactor()
{
    notImplemented();
    return 1.0f;
}

bool ChromeClientMg::canTakeFocus(FocusDirection direction)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    BOOL bForward = (direction == FocusDirectionForward) ? TRUE : FALSE;
    BOOL ret = FALSE;
    ASSERT(uiDelegate);
    ret = uiDelegate->canTakeFocus(m_webView, bForward);

    return ret;
}

void ChromeClientMg::takeFocus(FocusDirection direction)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    BOOL bForward = (direction == FocusDirectionForward) ? TRUE : FALSE;
    ASSERT(uiDelegate);
    uiDelegate->takeFocus(m_webView, bForward);
}


void ChromeClientMg::runModal()
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->runModal(m_webView);
}

void ChromeClientMg::setWindowRect(const FloatRect& rect)
{
    RECT rc = (IntRect)rect;

    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setFrame(m_webView, &rc);
}

void ChromeClientMg::invalidateWindow(const IntRect& windowRect, bool immediate)
{
    ASSERT(core(m_webView->topLevelFrame()));
    m_webView->repaint(windowRect, false /*contentChanged*/, immediate, false /*repaintContentOnly*/);
}

void ChromeClientMg::invalidateContentsAndWindow(const IntRect& windowRect, bool immediate)
{
    ASSERT(core(m_webView->topLevelFrame()));
    m_webView->repaint(windowRect, true /*contentChanged*/, immediate /*immediate*/, false /*repaintContentOnly*/);
}

void ChromeClientMg::invalidateContentsForSlowScroll(const IntRect& windowRect, bool immediate)
{
    ASSERT(core(m_webView->topLevelFrame()));
    m_webView->repaint(windowRect, true /*contentChanged*/, immediate, true /*repaintContentOnly*/);
}

void ChromeClientMg::scroll(const IntSize& delta, const IntRect& scrollViewRect, const IntRect& clipRect)
{
    ASSERT(core(m_webView->topLevelFrame()));
    m_webView->scrollBackingStore(core(m_webView->topLevelFrame())->view(), 
            delta.width(), delta.height(), scrollViewRect, clipRect);
}

IntPoint ChromeClientMg::screenToWindow(const IntPoint& point) const
{
    POINT result = point;
    ScreenToWindow(m_webView->viewWindow(), &(result.x), &(result.y));
    return result;
}

IntRect ChromeClientMg::windowToScreen(const IntRect& rect) const
{
    POINT topLeft = {0, 0};
    IntRect result = rect;
    WindowToScreen(m_webView->viewWindow(), &(topLeft.x), &(topLeft.y));
    result.move(topLeft.x, topLeft.y);

    return result;
}

PlatformPageClient ChromeClientMg::platformPageClient() const
{
    return m_webView->viewWindow();
}

void ChromeClientMg::contentsSizeChanged(Frame*, const IntSize&) const
{
    notImplemented();
}

void ChromeClientMg::scrollRectIntoView(const IntRect&, const ScrollView*) const
{
    notImplemented();
}

void ChromeClientMg::addMessageToConsole(MessageSource source, MessageType type, MessageLevel level, const String& message, unsigned int lineNumber, 
        const String& sourceID)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->webViewAddMessageToConsole(m_webView, message.utf8().data(), 
            lineNumber, sourceID.utf8().data(), level, type);
}

bool ChromeClientMg::canRunBeforeUnloadConfirmPanel()
{
    notImplemented();
    return false;
}

bool ChromeClientMg::runBeforeUnloadConfirmPanel(const String& message, Frame* frame)
{
    notImplemented();
    return false;
}

void ChromeClientMg::setStatusbarText(const String& text)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    uiDelegate->setStatusText(m_webView, text.utf8().data());
}

bool ChromeClientMg::shouldInterruptJavaScript()
{
    bool ret = false;
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);
    ret = uiDelegate->shouldInterruptJavaScript(m_webView);
    return ret;
}

KeyboardUIMode ChromeClientMg::keyboardUIMode()
{
    bool tabsToLinks = true;

    return tabsToLinks ? KeyboardAccessTabsToLinks : KeyboardAccessDefault;
}

IntRect ChromeClientMg::windowResizerRect() const
{
    notImplemented();
    return IntRect();
}

void ChromeClientMg::print(Frame* frame)
{
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    IMDWebFrame *webFrame = kit(frame);
    ASSERT(uiDelegate);
    uiDelegate->printFrame(m_webView, webFrame);
}

void ChromeClientMg::requestGeolocationPermissionForFrame(Frame*, Geolocation*)
{
    notImplemented();
}

void ChromeClientMg::cancelGeolocationPermissionRequestForFrame(Frame*, Geolocation*)
{
    notImplemented();
}

void ChromeClientMg::runOpenPanel(Frame*, PassRefPtr<FileChooser> fileChooser)
{
    MDChooseFilesInfo fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));

    fileInfo.isMutilFiles = fileChooser->allowsMultipleFiles();
    String acceptTypes = fileChooser->acceptTypes();
    fileInfo.acceptTypes = acceptTypes.utf8().data();


    if(m_webView->uiDelegate()->chooseFiles(m_webView, &fileInfo))
    {
        Vector<String> fileList;
        for(int i= 0; i < fileInfo.files_count; i++)
            fileList.append(String::fromUTF8(fileInfo.files[i]));
        fileChooser->chooseFile(fileList[0]);

        //mdolphin_cleanChooseFileInfo(&fileInfo);
        for(int i= 0; i < fileInfo.files_count; i++){
            free(fileInfo.files[i]);
        }
        free(fileInfo.files);

    }
}

void ChromeClientMg::chooseIconForFiles(const Vector<String>&, FileChooser*)
{
    notImplemented();
}

void ChromeClientMg::setCursor(const Cursor&)
{
    notImplemented();
}

void ChromeClientMg::formStateDidChange(const Node*)
{
}

#if ENABLE(DATABASE)
void ChromeClientMg::exceededDatabaseQuota(Frame* frame, const String& databaseName)
{
    //FIXME: We should set default quota with outside API.
    unsigned int defaultQuota = 5 * 1024 * 1024;
    DatabaseTracker::tracker().setQuota(frame->document()->securityOrigin(), defaultQuota);
}
#endif

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
void ChromeClientMg::reachedMaxAppCacheSize(int64_t spaceNeeded)                            
{
     // FIXME: Free some space.
     notImplemented();
}

void ChromeClientMg::reachedApplicationCacheOriginQuota(SecurityOrigin*)                    
{                                                                                         
     notImplemented();
}                                                                                         
#endif

bool ChromeClientMg::selectItemWritingDirectionIsNatural()
{
    return true;
}

bool ChromeClientMg::selectItemAlignmentFollowsMenuWritingDirection()
{
    return false;
}

PassRefPtr<PopupMenu> ChromeClientMg::createPopupMenu(PopupMenuClient* client) const
{
    return adoptRef(new PopupMenuMg(client));
}

PassRefPtr<SearchPopupMenu> ChromeClientMg::createSearchPopupMenu(PopupMenuClient* client) const
{
    return adoptRef(new SearchPopupMenuMg(client));
}

}
