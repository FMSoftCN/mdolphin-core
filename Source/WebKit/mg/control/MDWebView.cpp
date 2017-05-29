/*
 ** $Id$
 **
 ** MDWebView.h: the header file of WebView
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-06-24 
 */

#include "minigui.h"
#include "mdolphin.h"

#include "mdconfig.h"
#include "MDWebView.h"
#include "MDWebFrame.h"
#include "MDWebSettings.h"

#include "Frame.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "FrameLoader.h"
#include "DocumentLoader.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "EventHandler.h"
#include "Page.h"
#include "ProgressTracker.h"
#include "KURL.h"
#include "SelectionController.h"
#include "Document.h"
#include "ResourceRequest.h"
#include "PlatformMouseEvent.h"
#include "PlatformKeyboardEvent.h"
#include "KeyboardEvent.h"
#include "FocusController.h"
#include "Settings.h"
#include "CookieJar.h"
#include "Language.h"
#include "Chrome.h"
#include "ContextMenuController.h"
#include "ContextMenu.h"
#include "Cursor.h"
#include "Editor.h"
#include "MIMETypeRegistry.h"
#include "ResourceError.h"

#include "PopupMenuMg.h"
#include "InitializeThreading.h"
#include "Logging.h"

#include "GCController.h"
#include "HTMLFrameOwnerElement.h"
#include "JSDOMWindow.h"
#include "JSElement.h"
#include "JSLock.h"

#include "ChromeClientMg.h"
#include "ContextMenuClientMg.h"
#include "EditorClientMg.h"
#include "DragClientMg.h"
#include "InspectorClientMg.h"
#include "FrameLoaderClientMg.h"
#include "BackForwardListImpl.h"

#include "InspectorController.h"

#include "MDolphinEncoding.h"
#include "MDolphinDelegates.h"
#include "RenderView.h"
#include "HTMLInputElement.h"
#include "HTMLTextAreaElement.h"
#include "HTMLSelectElement.h"
#if ENABLE(WML)
#include "WMLInputElement.h"
#endif


#if USE(ICU_UNICODE)
#include "TextCodecICU.h"
#elif USE(MINIGUI_UNICODE)
#include "TextCodecMg.h"
#endif

#include "MDolphinDelegates.h"
#include "MDDefaultWebUIDelegate.h"
//#include "MDDefaultWebFramLoadDelegate.h"
#include "MDDefaultWebHistoryDelegate.h"
#include "MDDefaultWebCustomDelegate.h"
#include "MDDefaultWebPolicyDelegate.h"
#include "MDWebBackForwardList.h"
#include "MDWebInspector.h"
#include "MDCommonFunc.h"
#include "MDWebDownload.h"
#include "DomSerializer.h"
#include "FileSystem.h"

#if ENABLE(PLUGIN)
#include "mg/PluginApiMg.h"
#endif

//add mdolphin platform resource
#include "MDInnerResource.h"

#ifdef _MD_ENABLE_LOADSPLASH
#include "SystemTime.h"
#include "license/loadSplash.h"
#include "license/sharedTimerLoadSplash.h"
#endif

#ifdef _MD_ENABLE_WATERMARK
#include "license/waterMark.h"
#include "license/licenseHidden.h"
#include "license/c_files/watermark/watermark1.dat.c"
#include "license/c_files/watermark/watermark2.dat.c"
#endif

#if ENABLE(DATABASE)
#include "Database.h"
#include "DatabaseTracker.h"
#endif

using namespace WebCore;
using namespace WTF;

static void cleanupJSGlobalData()
{
    JSC::JSLock lock(JSC::SilenceAssertionsOnly);
    JSDOMWindow::commonJSGlobalData()->heap.collectAllGarbage();
}

bool MDWebView::m_haveRegisteredWindowClass = false;   
//START_MDWEBVIEW_INITWINDOW
HWND MDWebView::viewWindow()
{
    return m_viewWindow;
}
//END_MDWEBVIEW_INITWINDOW

//START_MDWEBVIEW_NAVIGATION
bool MDWebView::loadURL(const char *sUrl)
{
    return m_mainFrame->loadURL(sUrl);
}

void MDWebView::stopLoading(void)
{
    m_mainFrame->stopLoading();
    return;
}

void MDWebView::reload(void)
{
    if (m_page && !m_page->settings()->developerExtrasEnabled() && m_inspector)
    {
        m_inspector->close();
    }
    m_mainFrame->reload();
    return;
}

void MDWebView::goBack(void)
{
    m_page->goBack();
    return;
}

void MDWebView::goForward(void)
{
    m_page->goForward();
    return;
}

bool MDWebView::canGoBack(void)
{
    return (!!m_page->backForwardList()->backItem());
}

bool MDWebView::canGoForward(void)
{
    return (!!m_page->backForwardList()->forwardItem());
}
//END_MDWEBVIEW_NAVIGATION


//START_MDWEBVIEW_SEARCH
bool MDWebView::search(const char *str, bool forward, bool caseSensitive, bool wrap)
{
    if (!m_page || !m_page->mainFrame())
        return false;

    if (!str || !strlen(str))
        return false;

    String searchStr(str, strlen(str));
    return m_page->findString(searchStr, caseSensitive ? TextCaseSensitive : TextCaseInsensitive, 
                            forward ? FindDirectionForward : FindDirectionBackward, wrap);
}

unsigned int MDWebView::markAllMatches(const char *str, bool caseSensitive, 
                                    bool highlight, unsigned int limit)
{
    String searchStr(str, strlen(str));
    return m_page->markAllMatchesForText(searchStr, caseSensitive ? TextCaseSensitive : TextCaseInsensitive, 
                    highlight, limit);
}

void MDWebView::unmarkAllMatches(void)
{
    m_page->unmarkAllTextMatches();
    return;
}
//END_MDWEBVIEW_SEARCH

//START_MDWEBVIEW_CONTENTPOSITIPON
void MDWebView::contentsSize(int* w, int* h)
{
    if(w)
     *w = m_page->mainFrame()->view()->contentsWidth();
    if(h)
     *h = m_page->mainFrame()->view()->contentsHeight();
     return ;
}

void MDWebView::contentPos(int *x, int *y)
{

    IntSize offsetIntSize = m_page->mainFrame()->view()->scrollOffset();
    if(x)
        *x = offsetIntSize.width();
    if(y)
        *y = offsetIntSize.height();

    return;
}

void MDWebView::setContentPos(int x, int y)
{
    int dx, dy;
    int curx, cury;

    contentPos(&curx, &cury);
    dx = x - curx;
    dy = y - cury;
    scrollBy(dx, dy);
}

void MDWebView::scrollBy(int offsetX, int offsetY)
{
    m_page->mainFrame()->view()->scrollBy(IntSize(offsetX, offsetY));
    return;
}

//END_MDWEBVIEW_CONTENTPOSITIPON

//START_MDWEBVIEW_ZOOM
void MDWebView::setTextSizeMultiplier(float multiplier)
{
    setZoomMultiplier(multiplier, true);
    return;
}

float MDWebView::textSizeMultiplier()
{
    return zoomMultiplier(true);
}

// zoompage
void MDWebView::setPageSizeMultiplier(float multiplier)
{
    setZoomMultiplier(multiplier, false);
}

float MDWebView::pageSizeMultiplier()
{
    return zoomMultiplier(false);
}

const float MinZoomFactor = 0.25;
const float MaxZoomFactor = 4.0;

void MDWebView::setZoomMultiplier(float multiplier, bool isTextOnly)
{
  if (multiplier < MinZoomFactor) //>
      multiplier = MinZoomFactor;

  if (multiplier > MaxZoomFactor) 
      multiplier = MaxZoomFactor;

    m_zoomMultiplier = multiplier;
    m_zoomsTextOnly = isTextOnly;
    if (Frame* coreFrame = core(m_mainFrame)){
        if (m_zoomsTextOnly)
            coreFrame->setPageAndTextZoomFactors(1, multiplier);
        else
            coreFrame->setPageAndTextZoomFactors(multiplier, 1);
    }
}

float MDWebView::zoomMultiplier(bool isTextOnly)
{
    if (isTextOnly != m_zoomsTextOnly)
        return 1.0f;
    return m_zoomMultiplier;
}

//END_MDWEBVIEW_ZOOM

//START_MDWEBVIEW_PROGRESS
double MDWebView::estimatedProgress()
{
    return m_page->progress()->estimatedProgress();
}

void MDWebView::loadFinished()
{
#ifdef  _MD_ENABLE_LOADSPLASH
    if (m_loadSplash->isRuning()) {
        m_loadSplash->finish();
#if 1
        HWND hwnd = viewWindow();
        RECT rc;
        int left = m_loadSplash->offsetX();
        int top = m_loadSplash->offsetY();
        SetRect(&rc, left, top, left + m_loadSplash->width(), top + m_loadSplash->height());
        InvalidateRect(hwnd, &rc, FALSE);
#endif
    }
#endif
    //added by huangsh begin 2011.4.27
    HWND hwnd = viewWindow();
    MDWebSettings* settings=this->mdWebSettings();
    if (settings->areShowAllAtOnceEnabled()) {
        InvalidateRect(hwnd, NULL, FALSE);
    }
    //added by huangsh end 2011.4.27
}

bool MDWebView::isLoading()
{
    Frame *frame = core(m_mainFrame);
    if (!frame)
        return false;
    //first checke document loader
    DocumentLoader *documentLoader = frame->loader()->documentLoader();
    if (documentLoader && documentLoader->isLoading())
        return TRUE;

    //secondly check provision documentlader
    documentLoader = frame->loader()->provisionalDocumentLoader();
    if(documentLoader)
        return  documentLoader->isLoading();
    else 
        return false;
}
//END_MDWEBVIEW_PROGRESS

//START_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST
MDWebSettings* MDWebView::mdWebSettings()
{
    return m_pwebSettings;
}

MDWebInspector* MDWebView::mdWebInspector()
{
    return m_inspector;
}

IMDWebBackForwardList* MDWebView::mdWebBackForwardList()
{
    return m_backForwardList;
}
//END_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST

//START_MDWEBVIEW_PAGEINFO
void MDWebView::setCustomTextEncodingName(const char *encodingName)
{

    const char *oldEncoding = NULL;

    if (!encodingName)
        return;

    oldEncoding = customTextEncodingName();
    if ( !oldEncoding || !strlen(encodingName)||strncmp(oldEncoding, encodingName, strlen(encodingName)) != 0) {
        if (Frame* coreFrame = core(m_mainFrame))
            coreFrame->loader()->reloadWithOverrideEncoding(String(encodingName, strlen(encodingName)));
    }
    freeUtf8Char((void *)oldEncoding);

}

char* MDWebView::customTextEncodingName()
{
    Frame *frame = core(m_mainFrame);
    ASSERT(frame);
    DocumentLoader *documentLoader = frame->loader()->documentLoader();
    if (!documentLoader)
        return NULL;

    return copyStringToUtf8Char(documentLoader->overrideEncoding());
}

char* MDWebView::mainFrameURL()
{
    Frame *frame = core(m_mainFrame);
    ASSERT(frame);
    DocumentLoader *documentLoader = frame->loader()->documentLoader();
    if (!documentLoader)
        return NULL;

    return copyStringToUtf8Char(documentLoader->url().string());
}

IMDWebFrame*  MDWebView::mainWebFrame()
{
    return  m_mainFrame;
}
//END_MDWEBVIEW_PAGEINFO

//START_MDWEBVIEW_MIME
bool MDWebView::canShowMIMEType(const char *mime)
{
    bool ret = false;
    String mimeTypeStr(mime, strlen(mime));

    ret = MIMETypeRegistry::isSupportedImageMIMEType(mimeTypeStr) ||
        MIMETypeRegistry::isSupportedNonImageMIMEType(mimeTypeStr);

        //TODO: add the MIME of Plugin 
        //||PluginInfoStore::supportsMIMEType(mimeTypeStr) ||
        //shouldUseEmbeddedView(mimeTypeStr);

    return ret;
}
//END_MDWEBVIEW_MIME

//START_MDWEBVIEW_MENUCONTEXT
#if ENABLE(CONTEXT_MENUS)
// We should convert the ContextMenuActio to the flag mDolphin used in mdolphin.h
// Maybe we have a good way to implement this.
static unsigned int convertToMDAction(WebCore::ContextMenuAction action)
{
    unsigned int value = MD_POPMENU_NO;
    switch(action){
        case ContextMenuItemTagOpenLink:
            value = MD_POPMENU_OPEN_LINK;
            break;
        case ContextMenuItemTagOpenLinkInNewWindow:
            value = MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW;
            break;
        case ContextMenuItemTagDownloadLinkToDisk:
           value = MD_POPMENU_DOWNLOAD_LINK_TO_DISK; 
            break;
        case ContextMenuItemTagCopyLinkToClipboard:
            value = MD_POPMENU_COPY_LINK_TO_CLIPBOARD;
            break;
        case ContextMenuItemTagOpenImageInNewWindow:
            value = MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW;
            break;
        case ContextMenuItemTagDownloadImageToDisk:
            value = MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK;
            break;
        case ContextMenuItemTagCopyImageToClipboard:
            value = MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD;
            break;
        case ContextMenuItemTagGoBack:
            value = MD_POPMENU_GOBACK;
            break;
        case ContextMenuItemTagGoForward:
            value = MD_POPMENU_GOFORWARD;
            break;
        case ContextMenuItemTagStop:
            value = MD_POPMENU_STOP;
            break;
        case ContextMenuItemTagReload:
            value = MD_POPMENU_RELOAD;
            break;
        case ContextMenuItemTagOpenFrameInNewWindow:
            value = MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW;
            break;
        case ContextMenuItemTagCopy:
            value = MD_POPMENU_COPY;
            break;
        case ContextMenuItemTagCut:
            value = MD_POPMENU_CUT;
            break;
        case ContextMenuItemTagPaste:
            value = MD_POPMENU_PASTE;
            break;
        case ContextMenuItemTagSearchWeb:
            value = MD_POPMENU_SEARCH_WEB;
            break;
        case ContextMenuItemTagBold:
            value = MD_POPMENU_BOLD;
            break;
        case ContextMenuItemTagItalic:
            value = MD_POPMENU_ITALIC;
            break;
        case ContextMenuItemTagUnderline:
            value = MD_POPMENU_UNDERLINE;
            break;
        case ContextMenuItemTagDefaultDirection:
            value = MD_POPMENU_DEFAULT_DIRECTION;
            break;
        case ContextMenuItemTagLeftToRight:
            value = MD_POPMENU_LEFT_TO_RIGHT;
            break;
        case ContextMenuItemTagRightToLeft:
            value = MD_POPMENU_RIGHT_TO_LEFT;
            break;
#if ENABLE(INSPECTOR)
        case ContextMenuItemTagInspectElement:
            value = MD_POPMENU_INSPECT_ELEMENT;
            break;
#endif
        //We can do the following larger than ContextMenuItemBaseApplicationTag
        // and so we can implement it in ContextMenuClient::contextMenuItemSelected
        /* TODO! Not done.
        case :
            value = MD_POPMENU_SAVEAS_CURLINK;
            break;
        */
        default:
            value = MD_POPMENU_NO;
    }
    return value;
}

bool MDWebView::contextMenu(int message, WPARAM wParam, LPARAM lParam)
{
    m_page->contextMenuController()->clearContextMenu();

    mapMousePosition(&wParam, &lParam);
    PlatformMouseEvent mouseEvent(m_viewWindow, message, wParam, lParam);
    core(m_mainFrame)->eventHandler()->handleMousePressEvent(mouseEvent);

    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    frame->view()->setCursor(pointerCursor());
    bool handledEvent = frame->eventHandler()->sendContextMenuEvent(mouseEvent);
    if (!handledEvent) {
        core(m_mainFrame)->eventHandler()->handleMouseReleaseEvent(mouseEvent);
        return false;
    }    

    ContextMenu* coreMenu = m_page->contextMenuController()->contextMenu();
    if (!coreMenu){
        core(m_mainFrame)->eventHandler()->handleMouseReleaseEvent(mouseEvent);
        return false;
    }

    // Does delegate has context menu implementation?
    BOOL hasCustomMenus = m_uiDelegate->hasCustomMenuImplementation();
    if (!hasCustomMenus) {
        core(m_mainFrame)->eventHandler()->handleMouseReleaseEvent(mouseEvent);
        return false;
    }
    // Get context menu information  
    Vector<ContextMenuItem> items = coreMenu->platformDescription();
    MDPopupMenu menu_data;
    memset(&menu_data, 0, sizeof(menu_data));
    menu_data.itemsEnabled = 0xFFFFFFFF;
    for (unsigned i = 0; i < items.size(); ++i) { 
        ContextMenuItem &item = items[i];
        switch(item.type()) {
            case WebCore::CheckableActionType:
            case WebCore::ActionType: { 
                unsigned int action = convertToMDAction(item.action());
                if (action != MD_POPMENU_NO) {
                    m_page->contextMenuController()->checkOrEnableIfNeeded(item); 

                    PlatformMenuItemDescription desc = item.releasePlatformDescription(); 
                    if (desc.checked) 
                        menu_data.itemsChecked |=  action; 
                    if (!desc.enabled) 
                        menu_data.itemsEnabled &= ~action; 

                    menu_data.itemsFlag |= action; 
                }
                break; 
            } 
            // ignore the following 2 cases
            case WebCore::SeparatorType:
            case WebCore::SubmenuType: 
                break;
        }
    }
    menu_data.x = mouseEvent.pos().x();
    menu_data.y = mouseEvent.pos().y();

    m_uiDelegate->trackCustomPopupMenu((IMDWebView*)this, &menu_data);

    // It is good here. 
    core(m_mainFrame)->eventHandler()->handleMouseReleaseEvent(mouseEvent);
    
    return true;
}

void MDWebView::performContextAction(MDEPopupMenuElementEvent event)
{
    ContextMenu* menu = m_page->contextMenuController()->contextMenu();
    ASSERT(menu);
    ContextMenuItem* item = menu->itemWithAction((ContextMenuAction)event);
    m_page->contextMenuController()->contextMenuItemSelected(item);
}
#endif

void MDWebView::downloadURL(const WebCore::KURL& url)
{
    MDWebDownload* download = MDWebDownload::createInstance(url, m_downloadDelegate);
    download->start();
    download->release();
}
//END_MDWEBVIEW_MENUCONTEXT

//START_MDWEBVIEW_GETANDSETDELEGATE

#define  SET_MDWEBVIEW_DELEGATE(delegate, newDelegate,  globalDelgate) \
    if(newDelegate == delegate)\
        return;\
    if(delegate)\
        delegate->release();\
   if(!newDelegate)\
     delegate = globalDelgate;\
   else\
       delegate = newDelegate;\
   delegate->addRef();

void MDWebView::setUIDelegate(IMDWebUIDelegate* d)
{
#if 0
    if(d == m_uiDelegate)
        return;

    if(m_uiDelegate)
        m_uiDelegate->release();
   if(!d)
     m_uiDelegate = gMDWebUIDelegate;
   else
       m_uiDelegate = d;

   m_uiDelegate->addRef();
#endif
   SET_MDWEBVIEW_DELEGATE(m_uiDelegate, d,  gMDWebUIDelegate);
}

IMDWebUIDelegate*  MDWebView::uiDelegate()
{
    return m_uiDelegate;
}


void MDWebView::setDownloadDelegate(IMDWebDownloadDelegate* d)
{
   SET_MDWEBVIEW_DELEGATE(m_downloadDelegate, d,  gMDWebDownloadDelegate);
}

IMDWebDownloadDelegate* MDWebView::downloadDelegate( )
{
    return m_downloadDelegate;
}

void MDWebView::setHistoryDelegate(IMDWebHistoryDelegate* d)
{
   SET_MDWEBVIEW_DELEGATE(m_historyDelegate, d,  gMDWebHistoryDelegate);
}

IMDWebHistoryDelegate* MDWebView::historyDelegate( )
{
    return m_historyDelegate;
}

void MDWebView::setCustomDelegate(IMDWebCustomDelegate* d)
{
   SET_MDWEBVIEW_DELEGATE(m_customDelegate, d,  gMDWebCustomDelegate);
}

IMDWebCustomDelegate* MDWebView::customDelegate()
{
    return m_customDelegate;
}

void MDWebView::setPolicyDelegate(IMDWebPolicyDelegate* d)
{
   SET_MDWEBVIEW_DELEGATE(m_policyDelegate, d,  gMDWebPolicyDelegate);
}

IMDWebPolicyDelegate* MDWebView::policyDelegate()
{
    return m_policyDelegate;
}

void MDWebView::setFrameLoadDelegate(IMDWebFrameLoadDelegate* d)
{
   SET_MDWEBVIEW_DELEGATE(m_frameLoadDelegate, d,  gMDWebFrameLoadDelegate);
}

IMDWebFrameLoadDelegate* MDWebView::frameLoadDelegate()
{
    return m_frameLoadDelegate;
}

#undef SET_MDWEBVIEW_DELEGATE
//END_MDWEBVIEW_GETANDSETDELEGATE



//START_MDWEBVIEW_INNERFUNC
//START_MDWEBVIEW_EVENT
void MDWebView::mapMousePosition(WPARAM* wParam, LPARAM* lParam)
{
    int x = LOSWORD(*lParam);
    int y = HISWORD(*lParam);

    //x = (int)roundf(((float)x)/d->scale_x);
    //y = (int)roundf(((float)y)/d->scale_y);

    *lParam = MAKELONG(x, y);
}

bool MDWebView::mouseMove(int message, WPARAM wParam, LPARAM lParam)
{
    mapMousePosition(&wParam, &lParam);
    PlatformMouseEvent mouseEvent(m_viewWindow, message, wParam, lParam);
    return core(m_mainFrame)->eventHandler()->mouseMoved(mouseEvent);
}

bool MDWebView::mouseDown(int message, WPARAM wParam, LPARAM lParam)
{
    mapMousePosition(&wParam, &lParam);
    PlatformMouseEvent mouseEvent(m_viewWindow, message, wParam, lParam);
    return core(m_mainFrame)->eventHandler()->handleMousePressEvent(mouseEvent);
}

bool MDWebView::mouseUp(int message, WPARAM wParam, LPARAM lParam)
{
    mapMousePosition(&wParam, &lParam);
    PlatformMouseEvent mouseEvent(m_viewWindow, message, wParam, lParam);
    return core(m_mainFrame)->eventHandler()->handleMouseReleaseEvent(mouseEvent);
}

bool MDWebView::keyDown(int message, WPARAM wParam, LPARAM lParam)
{
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    if (wParam == SCANCODE_CAPSLOCK)
        frame->eventHandler()->capsLockStateMayHaveChanged();

//pandy add the statement for ctrl+tab
    if(wParam == SCANCODE_TAB && (lParam & KS_CTRL)) 
            m_page->focusController()->setInitialFocus(FocusDirectionBackward, 0);

    PlatformKeyboardEvent keyEvent(m_viewWindow, message, wParam, lParam);
    bool handled = frame->eventHandler()->keyEvent(keyEvent);

    if (handled) {
        return true;
    }

    // We need to handle back/forward using either Backspace(+Shift) or Ctrl+Left/Right Arrow keys.
    if ((wParam == SCANCODE_BACKSPACE && keyEvent.shiftKey()) 
            || (wParam == SCANCODE_CURSORBLOCKRIGHT && keyEvent.ctrlKey()))
        return m_page->goForward();
    if (wParam == SCANCODE_BACKSPACE || (wParam == SCANCODE_CURSORBLOCKLEFT && keyEvent.ctrlKey()))
        return m_page->goBack();

    // Need to scroll the page if the arrow keys, pgup/dn, or home/end are hit.
    ScrollDirection direction;
    ScrollGranularity granularity;
    switch (wParam) {
        case SCANCODE_CURSORBLOCKLEFT:
            granularity = ScrollByLine;
            direction = ScrollLeft;
            break;
        case SCANCODE_CURSORBLOCKRIGHT:
            granularity = ScrollByLine;
            direction = ScrollRight;
            break;
        case SCANCODE_CURSORBLOCKUP:
            granularity = ScrollByLine;
            direction = ScrollUp;
            break;
        case SCANCODE_CURSORBLOCKDOWN:
            granularity = ScrollByLine;
            direction = ScrollDown;
            break;
        case SCANCODE_HOME:
            granularity = ScrollByDocument;
            direction = ScrollUp;
            break;
        case SCANCODE_END:
            granularity = ScrollByDocument;
            direction = ScrollDown;
            break;
        case SCANCODE_PAGEUP:
            granularity = ScrollByPage;
            direction = ScrollUp;
            break;
        case SCANCODE_PAGEDOWN:
            granularity = ScrollByPage;
            direction = ScrollDown;
            break;
        default:
            return false;
    }

    return frame->eventHandler()->scrollRecursively(direction, granularity);
}

bool MDWebView::keyUp(int message, WPARAM wParam, LPARAM lParam)
{
    PlatformKeyboardEvent keyEvent(m_viewWindow, message, wParam, lParam);
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    return frame->eventHandler()->keyEvent(keyEvent);
}
//END_MDWEBVIEW_EVENT

//START_MDWEBVIEW_MESSAGEHANDLER
void MDWebView::setFocus(void)
{
    FocusController *focusController = m_page->focusController();
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    focusController->setActive(true);
    focusController->setFocused(true);
    focusController->setFocusedFrame(frame);

    ChromeClientMg * client = static_cast<ChromeClientMg *> (m_page->chrome()->client());
    client->focus();
}

void MDWebView::killFocus(void)
{
    // only set the focused frame inactive so that we stop painting the caret
    // and the focus frame. But don't tell the focus controller so that upon
    // focusInEvent() we can re-activate the frame.
    FocusController *focusController = m_page->focusController();
    // Call setFocused first so that window.onblur doesn't get called twice
    focusController->setFocusedFrame(0);
    focusController->setFocused(false);
    focusController->setActive(false);
    
    ChromeClientMg * client = static_cast<ChromeClientMg *> (m_page->chrome()->client());
    client->unfocus();
}

void MDWebView::setFocusEditable(bool b)
{
     m_isFocusEditable = b;
}

bool MDWebView::isFocusEditable(void)
{
    //TODO: m_isEditable is setted by EditorClientMg::setInputMethodState
    //so  need to call setFocusEditable in  EditorClientMg::setInputMethodState
    return m_isFocusEditable;
}

static bool input_char (unsigned int input, char * buf, int * size)
{
    if (input == 127) // BS
        input = '\b';

    buf[0] = LOBYTE (input);
    buf[1] = HIBYTE (input);
    buf[2] = (0x00ff0000 & input) >> 16;

    if (buf[2]) {
        *size = 3;
    }
    else if (HIBYTE (input)) {
        *size = 2;
    }
    else 
        *size = 1;
    
    //FIXME
    if (*size == 1) {
        if (buf[0]<0x20 && buf[0]!='\b' && buf[0] != '\r') { //>
            return false;
        }
    }
    return true;
}

bool MDWebView::charInput(unsigned int inputChar, unsigned int keyFlags)
{

    char charBuffer [3] = {0};
    int chars;
    bool sawError;

    //FixMe:  
    if (!input_char(inputChar, charBuffer, &chars))
        return false;

#if USE(ICU_UNICODE)
    TextCodecICU codec("UTF-8");
    String str = codec.decode((const char*)charBuffer, (size_t)chars, true, true, sawError);
#elif USE(MINIGUI_UNICODE)
    TextCodecMg codec("UTF-8");
    String str = codec.decode(charBuffer, chars, true, true, sawError);
#endif

    PlatformKeyboardEvent keyEvent(m_viewWindow, MSG_CHAR, inputChar, keyFlags, str);
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    return frame->eventHandler()->keyEvent(keyEvent);

}
//END_MDWEBVIEW_MESSAGEHANDLER

//START_MDWEBVIEW_HELPFUNC

Page* core(MDWebView* WebView)
{
    ASSERT(WebView);
    return WebView->page();
}

Page* MDWebView::page()
{
    return m_page;
}
//END_MDWEBVIEW_HELPFUNC

bool MDWebView::setInViewSourceMode(bool flag)
{           
    Frame* coreFrame = core(m_mainFrame);
    if (!coreFrame)
        return false;
    coreFrame->setInViewSourceMode(flag);
    return true;
}   

bool MDWebView::isInViewSourceMode()
{
    Frame* coreFrame = core(m_mainFrame);
    if (!coreFrame)
        return false;
    return coreFrame->inViewSourceMode();
}


//START_MDWEBVIEW_PAINT
static BOOL ScrollDC(HDC hdc, int dx, int dy, const RECT *lprcScroll,
        const RECT *lprcClip, PCLIPRGN hrgnUpdate, PRECT lprcUpdate)
{
    RECT rcScroll, rcClient, rcClip;
    int dcWidth = GetGDCapability(hdc, GDCAP_MAXX) + 1;
    int dcHeight = GetGDCapability(hdc, GDCAP_MAXY) + 1;

    SetRect(&rcClient, 0, 0, dcWidth, dcHeight);
    if (lprcScroll) {
        if (!IntersectRect(&rcScroll, lprcScroll, &rcClient))
            return FALSE;
    }
    else
        CopyRect(&rcScroll, &rcClient);

    if (lprcClip) {
        if (!IntersectRect(&rcClip, lprcClip, &rcClient))
            return FALSE;
    }
    else
        CopyRect(&rcClip, &rcClient);

    //add clip region
    SelectClipRect(hdc, &rcClip);

    //blit
    BitBlt (hdc, rcScroll.left, rcScroll.top, 
            RECTW(rcScroll), RECTH(rcScroll),
            hdc, dx + rcScroll.left, dy + rcScroll.top, 0);

    //calculate uncovered region
    if (hrgnUpdate || lprcUpdate) {
        PCLIPRGN pClipRgn;
        RECT rcDest, rcInv;

        if (hrgnUpdate)
            pClipRgn = hrgnUpdate;
        else
            pClipRgn = CreateClipRgn();

        //affected region by the scroll operation
        GetClipRegion(hdc, pClipRgn);

        SetRect(&rcDest, dx + rcScroll.left, dy + rcScroll.top, 
               dx + rcScroll.left + RECTW(rcScroll), 
               dy + rcScroll.top + RECTH(rcScroll));

        //uncovered region
        if (IntersectRect(&rcDest, &rcDest, &rcClient)
                && IntersectRect(&rcInv, &rcDest, &rcClip)) {
            SubtractClipRect(pClipRgn, &rcInv);
        }

        if (lprcUpdate)
            GetClipRgnBoundRect(pClipRgn, lprcUpdate);

        if (!hrgnUpdate)
            DestroyClipRgn(pClipRgn);
    }

    //restore dc region
    SelectClipRect(hdc, NULL);
    return TRUE;
}

/* Re-calculate the rcBound of a region */
static void recomputeRegionBound (CLIPRGN *region)
{
    PCLIPRECT cliprect;
    RECT *pExtents;

    if (region->head == NULL) {
        region->rcBound.left = 0; region->rcBound.top = 0;
        region->rcBound.right = 0; region->rcBound.bottom = 0;
        return;
    }

    pExtents = &region->rcBound;

    /*
     * Since head is the first rectangle in the region, it must have the
     * smallest top and since tail is the last rectangle in the region,
     * it must have the largest bottom, because of banding. Initialize left and
     * right from head and tail, resp., as good things to initialize them
     * to...
     */
    pExtents->left = region->head->rc.left;
    pExtents->top = region->head->rc.top;
    pExtents->right = region->tail->rc.right;
    pExtents->bottom = region->tail->rc.bottom;

    cliprect = region->head;
    while (cliprect) {
        if (cliprect->rc.left < pExtents->left)
            pExtents->left = cliprect->rc.left;
        if (cliprect->rc.right > pExtents->right)
            pExtents->right = cliprect->rc.right;

        cliprect = cliprect->next;
    }
}

static void OffsetUpdateRegion(PCLIPRGN region, 
        const RECT* lprcScroll, const RECT *lprcClip, int dx, int dy)
{
    CLIPRECT* cliprect = region->head;
    RECT old_cliprc, rc_array[4], rc;
    int i, nCount = 0;

    if (!lprcClip || !lprcScroll)
        return;

    if (!IntersectRect (&rc, lprcClip, lprcScroll))
        return;

    while (cliprect) {
        /*not in scroll window region, return*/
        if (!DoesIntersect (&cliprect->rc, &rc)) {
            cliprect = cliprect->next;
            continue;
        }

        /*not covered, recalculate cliprect*/
        if (!IsCovered (&cliprect->rc, &rc)) {
            CopyRect (&old_cliprc, &cliprect->rc);
            IntersectRect (&cliprect->rc, &old_cliprc, &rc);
            nCount = SubtractRect (rc_array, &old_cliprc, &rc);
            for (i = 0; i < nCount; i++) {
                AddClipRect (region, &rc_array[i]);
            }
        }

        OffsetRect (&cliprect->rc, dx, dy);

        /*if intersect, tune cliprect*/
        if (!IsCovered (&cliprect->rc, &rc)) {
            CopyRect (&old_cliprc, &cliprect->rc);
            IntersectRect (&cliprect->rc, &old_cliprc, &rc);
        }

        /*if not intersect, empty the rect*/
        if (!DoesIntersect (&cliprect->rc, &rc)) {
            SetRect(&cliprect->rc, 0, 0, 0, 0);
        }

        if (region->head) {
            recomputeRegionBound(region);
        }

        cliprect = cliprect->next;
    }

}

static void getUpdateRects(PCLIPRGN pRgn, Vector<WebCore::IntRect>& rects)
{
    PCLIPRECT pClipRect = pRgn->head;
    while (pClipRect) {
        rects.append(pClipRect->rc);
        pClipRect = pClipRect->next;
    }
}

void MDWebView::resize(int newWidth, int newHeight)
{
    if (core(m_mainFrame) && core(m_mainFrame)->view())
        core(m_mainFrame)->view()->resize(newWidth, newHeight);

#ifdef  _MD_ENABLE_LOADSPLASH
        m_loadSplash->calcOffsetPos();
#endif
}

void MDWebView::drawWaterMark(HDC hdc)
{
#ifdef _MD_ENABLE_WATERMARK 
   int waterMarkWidth, waterMarkHeight;
   int waterMarkOffsetX, waterMarkOffsetY;

   waterMark::waterMarkSize(waterMarkWidth, waterMarkHeight);
#ifdef WATERMARK_RIGHTTOP
   waterMarkOffsetX = GetGDCapability(hdc, GDCAP_HPIXEL)  - waterMarkWidth;
   waterMarkOffsetY = 0;
   waterMark::fillWatmark(hdc, waterMarkOffsetX, waterMarkOffsetY);
#else //center
   waterMarkOffsetX = (GetGDCapability(hdc, GDCAP_HPIXEL)  - waterMarkWidth) >> 1;
   waterMarkOffsetY = (GetGDCapability(hdc, GDCAP_VPIXEL)  - waterMarkHeight) >> 1;
   waterMark::fillWatmark(hdc, waterMarkOffsetX, waterMarkOffsetY);
#endif
#endif
}

void MDWebView::drawLoadSplash(HDC hdc)
{
#ifdef _MD_ENABLE_LOADSPLASH
    if (isLoading()) {
        if (!m_loadSplash->isRuning()) {
            m_loadSplash->start();
        }
        m_loadSplash->updateBackgroundDC(m_backingStoreMemDC);
        m_loadSplash->updateToWindow();
#if 1 //FixMe: Don't update loadSplash area
        RECT rc;
        int left = m_loadSplash->offsetX();
        int top = m_loadSplash->offsetY();
        SetRect(&rc, left, top, left + m_loadSplash->width(), top + m_loadSplash->height());
        ExcludeClipRect(hdc, &rc); 
#endif
    } else if (m_loadSplash->isRuning()) {
        m_loadSplash->finish();
        HWND hwnd = viewWindow();
        RECT rc;
        int left = m_loadSplash->offsetX();
        int top = m_loadSplash->offsetY();
        SetRect(&rc, left, top, left + m_loadSplash->width(), top + m_loadSplash->height());
        InvalidateRect(hwnd, &rc, FALSE);
    }
#endif
}

void MDWebView::paint(void)
{
    Frame* coreFrame = core(m_mainFrame);
    if (!coreFrame)
        return;

    RECT rcPaint;
    GetUpdateRect(m_viewWindow, &rcPaint);
    if (IsRectEmpty(&rcPaint)) {
        return;
    }

    m_paintCount++;

    bool backingStoreCompletelyDirty = ensureBackingStore();
    FrameView* frameView = coreFrame->view();

    // Update our backing store if needed.
    updateBackingStore(frameView, backingStoreCompletelyDirty);

    // Apply the same heuristic for this update region too.
    PCLIPRGN pClipRgn = CreateClipRgn();
    Vector<IntRect> blitRects;
    HDC hdc = BeginPaint(m_viewWindow);
 
    //added by huangsh begin 2011.4.27 
    MDWebSettings* settings = this->mdWebSettings();
    static bool loadflag;
    if (settings->areShowAllAtOnceEnabled()) {
        if (isLoading()){
            loadflag = true;
            EndPaint(m_viewWindow, hdc);
            m_paintCount--;
            return;
        } else {
            if (loadflag == true) {
                InvalidateRect(m_viewWindow, NULL, false);
                loadflag = false;
            }
        }
    }
    //added by huangsh end 2011.4.27 

    // custom Delegate 
    if (m_customDelegate) {
        RECT rcClient;
        GetClientRect(m_viewWindow, &rcClient); 
        m_customDelegate->beforeDrawViewport(this, hdc, m_backingStoreMemDC, &rcClient);
    }
    
    //drawLoadSplash(hdc);
#ifdef _MD_ENABLE_LOADSPLASH
    if (isLoading()) {
        RECT rc;
        int left = m_loadSplash->offsetX();
        int top = m_loadSplash->offsetY();
        SetRect(&rc, left, top, left + m_loadSplash->width(), top + m_loadSplash->height());
        ExcludeClipRect(hdc, &rc); 
    }
#endif

    GetClipRegion(hdc, pClipRgn);
    if (pClipRgn->type != NULLREGION)
        getUpdateRects(pClipRgn, blitRects);
    else
        blitRects.append(IntRect(rcPaint));

    DestroyClipRgn(pClipRgn);

    //modify by huangsh begin 2011.4.27
    if (!settings->areShowAllAtOnceEnabled()) {
        // Now we blit the updated backing store
        for (unsigned i = 0; i < blitRects.size(); ++i)
            paintIntoWindow(m_backingStoreMemDC, hdc, blitRects[i]);
    } else if (!isLoading()) {
        // Now we blit the updated backing store
        for (unsigned i = 0; i < blitRects.size(); ++i)
            paintIntoWindow(m_backingStoreMemDC, hdc, blitRects[i]);     
    }
    //modify by huangsh end 2011.4.27

    drawWaterMark(hdc);

#if ENABLE(INSPECTOR)
    GraphicsContext gc(&hdc);
    coreFrame->page()->inspectorController()->drawNodeHighlight(gc);
#endif

    if (m_customDelegate) {
        RECT rcClient;
        GetClientRect(m_viewWindow, &rcClient); 
        m_customDelegate->afterDrawViewport(this, hdc, m_backingStoreMemDC, &rcClient);
    }


    EndPaint(m_viewWindow, hdc);

    m_paintCount--;
}

bool MDWebView::invalidateBackingStore(const RECT* rect)
{
    if (!IsWindow(m_viewWindow))
        return true;

    RECT clientRect;
    if (!GetClientRect(m_viewWindow, &clientRect))
        return false;

    RECT rectToInvalidate;
    if (!rect)
        rectToInvalidate = clientRect;
    else if (!IntersectRect(&rectToInvalidate, &clientRect, rect))
        return true;

    repaint(rectToInvalidate, true);
    return true;
}

void MDWebView::paintIntoWindow(HDC backingStoreDC, HDC windowDC, const IntRect& dirtyRect)
{
    // Blit the dirty rect from the backing store into the same position in the destination DC
    BitBlt(backingStoreDC, dirtyRect.x(), dirtyRect.y(), 
            dirtyRect.width(), dirtyRect.height(),
            windowDC, dirtyRect.x(), dirtyRect.y(), 0);
}

//static functions implements
static HDC getMemDC(HWND hwnd, int width, int height)
{
    HDC mem_dc = CreateMemDC(width, height,32, MEMDC_FLAG_HWSURFACE | MEMDC_FLAG_SRCALPHA
                             , 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    //FIXME: Do we really need following? 
    //FillBox (mem_dc, 0, 0, width, height);

    return mem_dc;
}

bool MDWebView::ensureBackingStore(void)
{
    RECT windowRect;
    int width, height;

    GetClientRect(m_viewWindow, &windowRect);
    width = RECTW(windowRect);
    height = RECTH(windowRect);

    if (width > 0 && height > 0 && (width != m_backingStoreSize.cx || height != m_backingStoreSize.cy)) {
        deleteBackingStore();

        m_backingStoreSize.cx = width;
        m_backingStoreSize.cy = height;
        m_backingStoreMemDC = getMemDC(m_viewWindow, width, height);
        return true;
    }

    return false;
}

void MDWebView::addToDirtyRegion(const IntRect& dirtyRect)
{
    RECT newRect = dirtyRect;
    if (!m_backingStoreDirtyRegion)
        m_backingStoreDirtyRegion = CreateClipRgn();

    if (m_backingStoreDirtyRegion)
        AddClipRect(m_backingStoreDirtyRegion, &newRect);

}

void MDWebView::addToDirtyRegion(PCLIPRGN newRegion)
{
    if (m_backingStoreDirtyRegion) {
        PCLIPRGN combinedRegion = CreateClipRgn();

        UnionRegion(combinedRegion, m_backingStoreDirtyRegion, newRegion);
        DestroyClipRgn(m_backingStoreDirtyRegion);

        m_backingStoreDirtyRegion = combinedRegion;
    } else {
        m_backingStoreDirtyRegion = CreateClipRgn();
        ClipRgnCopy(m_backingStoreDirtyRegion, newRegion);
    }

    /*  
    if (m_uiDelegate)
        m_uiDelegate->webViewDidInvalidate(this);*/
}


void MDWebView::scrollBackingStore(FrameView* frameView, int dx, int dy, 
        const IntRect& scrollViewRect, const IntRect& clipRect)
{
    // If there's no backing store we don't need to update it
    if (!m_backingStoreMemDC) {
        /*  
        if (m_uiDelegate)
            m_uiDelegate->webViewScrolled(this);*/
        return;
    }

    PCLIPRGN updateRegion = CreateClipRgn();
    RECT scrollRectWin(scrollViewRect);
    RECT clipRectWin(clipRect);

    //scroll dc
    if (ScrollDC(m_backingStoreMemDC, dx, dy, &scrollRectWin, &clipRectWin, updateRegion, 0)) {
        if (m_backingStoreDirtyRegion) {
            //FIXME:in here calling updateBackingStore to make sure refresh correctly, 
            //because UnionRegion now can remove some invalid region incorrectly.
            OffsetUpdateRegion(m_backingStoreDirtyRegion, &scrollRectWin, &clipRectWin, dx, dy);
            updateBackingStore(frameView, false);
        }

        // Add the dirty region to the backing store's dirty region.
        addToDirtyRegion(updateRegion);
        DestroyClipRgn(updateRegion);

        /*  
            if (m_uiDelegate)
            m_uiDelegate->webViewScrolled(this);*/

    }
    // Update the backing store.
    updateBackingStore(frameView, false);
}

void MDWebView::deleteBackingStore()
{
    if (m_backingStoreDirtyRegion)
        EmptyClipRgn(m_backingStoreDirtyRegion);

    if (m_backingStoreMemDC) {
        DeleteMemDC(m_backingStoreMemDC);
        m_backingStoreMemDC = 0;
    }

    m_backingStoreSize.cx = m_backingStoreSize.cy = 0;
}

void MDWebView::repaint(const IntRect& windowRect, bool contentChanged, 
        bool immediate, bool repaintContentOnly)
{
    if (!repaintContentOnly) {
        RECT rect = windowRect;
        InvalidateRect(m_viewWindow, &rect, false);
    }
    if (contentChanged)
        addToDirtyRegion(windowRect);
    if (immediate) {
        if (repaintContentOnly)
            updateBackingStore(core(m_mainFrame)->view());
        else
            UpdateWindow(m_viewWindow, FALSE);
    }
}

void MDWebView::updateBackingStore(FrameView* frameView,
        bool backingStoreCompletelyDirty)
{
    if (m_backingStoreMemDC && (m_backingStoreDirtyRegion || backingStoreCompletelyDirty)) {
        // Do a layout first so that everything we render to the backing store is always current.
        if (Frame* coreFrame = core(m_mainFrame))
            if (FrameView* view = coreFrame->view())
                view->updateLayoutAndStyleIfNeededRecursive();

        Vector<IntRect> paintRects;
        if (!backingStoreCompletelyDirty && m_backingStoreDirtyRegion) {
            getUpdateRects(m_backingStoreDirtyRegion, paintRects);
        } else {
            RECT clientRect;
            GetClientRect(m_viewWindow, &clientRect);
            paintRects.append(clientRect);
        }

        for (unsigned i = 0; i < paintRects.size(); ++i) {
            paintIntoBackingStore(frameView, m_backingStoreMemDC, paintRects[i]);
        }

        /*  
        if (m_uiDelegate)
            m_uiDelegate->webViewPainted(this);
            */

        if (m_backingStoreDirtyRegion)
            EmptyClipRgn(m_backingStoreDirtyRegion);
    } 
}

void MDWebView::paintIntoBackingStore(FrameView* frameView, 
        HDC backingStoreDC, const IntRect& dirtyRect)
{
    RECT rect = dirtyRect;

    GraphicsContext gc(&backingStoreDC);
    gc.save();

    //Fill rectangle with white brush.
    gal_pixel oldBrushColor = SetBrushColor(backingStoreDC, PIXEL_lightwhite);
    FillBox(backingStoreDC, rect.left, rect.top, RECTW(rect), RECTH(rect));
    SetBrushColor(backingStoreDC, oldBrushColor);

    //TODO: drawBackground for ui delegate.

    //frame view painting
    if (frameView && frameView->frame() && frameView->frame()->contentRenderer()) {
        gc.clip(dirtyRect);
        frameView->paint(&gc, dirtyRect);
    }
    gc.restore();
}
//END_MDWEBVIEW_PAINT

//START_MDWEBVIEW_CONSTRUCTOR

MDWebView::MDWebView()
    : m_viewWindow(0)
    , m_mainFrame(0)
    , m_page(0)
    , m_zoomMultiplier (1)
    , m_zoomsTextOnly(false)
    , m_isFocusEditable(false)
    , m_backingStoreMemDC(0)
    , m_paintCount(0)
    , m_backingStoreDirtyRegion(0)
    , m_uiDelegate(0)
    , m_downloadDelegate(0)
    , m_historyDelegate(0)
    , m_customDelegate(0)
    , m_policyDelegate(0)
    , m_frameLoadDelegate(0)
    , m_pwebSettings(0)
    , m_inspector(0)
    , m_backForwardList(0)
{
    m_backingStoreSize.cx = m_backingStoreSize.cy = 0;
}

MDWebView::~MDWebView()
{
    deleteBackingStore();
    if (m_backingStoreDirtyRegion) {
        DestroyClipRgn(m_backingStoreDirtyRegion);
        m_backingStoreDirtyRegion = 0;
    }


    if (m_page) {
        if (Frame* frame = m_page->mainFrame())
            frame->loader()->detachFromParent();
    }

    m_mainFrame->release();

    m_uiDelegate->release();
    m_downloadDelegate->release();
    m_historyDelegate->release();
    m_customDelegate->release();
    m_policyDelegate->release();
    m_frameLoadDelegate->release();
    
#if ENABLE(INSPECTOR)
    if(m_inspector)
        m_inspector->release();
#endif
    m_backForwardList->release();

    delete m_page;
    if(m_pwebSettings)
        delete m_pwebSettings;
#ifdef _MD_ENABLE_LOADSPLASH
        delete m_loadSplash;
#endif
}

MDWebView* MDWebView::createInstance()
{
    MDWebView* instance = new MDWebView();
    return instance;
}

void MDWebView::initSettings(Settings *settings)
{
    settings->setLoadsImagesAutomatically(true); 

    //for default font family information
    settings->setStandardFontFamily(AtomicString("Arial"));
    settings->setSansSerifFontFamily(AtomicString("Helvetical"));
    settings->setFixedFontFamily(AtomicString("Courier New"));
    settings->setSerifFontFamily(AtomicString("Times New Roman"));
    settings->setDefaultTextEncodingName(String("ISO8859-1"));
    settings->setUsesEncodingDetector(true);
    settings->setMinimumFontSize(0);
    settings->setMinimumLogicalFontSize(9);
    settings->setDefaultFontSize(16);
    settings->setDefaultFixedFontSize(13);


    settings->setJavaScriptEnabled(true);
    settings->setShrinksStandaloneImagesToFit(true);
    settings->setShouldPrintBackgrounds(true);
    settings->setMediaEnabled(true);
    //default we disable SpatialNavigation
    settings->setSpatialNavigationEnabled(false);

#if ENABLE(DATABASE)
    Database::setIsAvailable(true);
    settings->setLocalStorageEnabled(true);

    String databaseDirectory;
    char *databaseEnv = getenv("MD_DATABASE_DIR");
    if (databaseEnv)
        databaseDirectory.append(databaseEnv);
    else {
        String userDatabasePath = homeDirectoryPath();
        userDatabasePath.append(String("/.mdolphin/database"));
        databaseDirectory.append(userDatabasePath);
    }   

    settings->setLocalStorageDatabasePath(databaseDirectory);
    WebCore::DatabaseTracker::tracker().setDatabaseDirectoryPath(databaseDirectory);

#endif

#if ENABLE(PLUGIN)
    settings->setPluginsEnabled(true);
#endif
    //settings->setJavaEnabled(true);
    
    //The following value is same as Settings default value.
    //settings->setJavaScriptCanOpenWindowsAutomatically(false);
    //settings->setShowAllAtOnce(false);
    //settings->setTextAreasAreResizable(false);
    //settings->setPrivateBrowsingEnabled(false);
    //settings->setUsesDashboardBackwardCompatibilityMode(false);
    //settings->setNeedsAdobeFrameReloadingQuirk(false);
    //settings->setDOMPasteAllowed(false);
    //settings->setUsesPageCache(false);
    //settings->setForceFTPDirectoryListings(false);
    //settings->setDeveloperExtrasEnabled(false);

    settings->setShouldPaintCustomScrollbars(true);
    //added by huangsh begin 2011.4.27
    settings->setShowAllAtOnceEnabled(false);
    //added by huangsh end 2011.4.27
}

void MDWebView::init(HWND hwnd)
{
    RECT rect;

    if(hwnd == HWND_INVALID)
        return;
    m_viewWindow = hwnd;
    GetClientRect(m_viewWindow, &rect);


    //set up global delegate
    setUIDelegate(gMDWebUIDelegate);
    setDownloadDelegate(gMDWebDownloadDelegate);
    setHistoryDelegate(gMDWebHistoryDelegate);
    setCustomDelegate(gMDWebCustomDelegate);
    setPolicyDelegate(gMDWebPolicyDelegate);
    setFrameLoadDelegate(gMDWebFrameLoadDelegate);

    // save a potinter of WebView in ChromeClientMg, but don't add count in ChromeClientMg
    Page::PageClients pageClients;
    pageClients.chromeClient = new ChromeClientMg(this);
    pageClients.contextMenuClient = new ContextMenuClientMg(this);
    pageClients.editorClient = new EditorClientMg(this);
    pageClients.dragClient = new DragClientMg;

#if ENABLE(INSPECTOR)
    pageClients.inspectorClient = new InspectorClientMg(this);
#else
    pageClients.inspectorClient = NULL;
#endif
    pageClients.pluginHalterClient = 0;

    m_page = new Page(pageClients);

    MDWebView::initSettings(m_page->settings());
    m_page->setGroupName(String("PageGroup"));
    MDWebFrame* webFrame = MDWebFrame::createInstance();
    // don't call 'webFrame->addRef();'
    FrameLoaderClientMg *client = new FrameLoaderClientMg();
    RefPtr<Frame> coreFrame = webFrame->init(this, 0, client);
    //coreFrame->tree()->setName(String(frameName, SysStringLen(frameName)));
    client->setWebFrame(webFrame);
    coreFrame->init();

    //TODO: modify the contructor FrameLoaderClientMg, add WebFrame member. 
    //setWebFrame should call addRef() to add the count
    m_mainFrame = webFrame;

    //init websettings
    m_pwebSettings = new MDWebSettings(this);

#if ENABLE(INSPECTOR)
    m_inspector = MDWebInspector::createInstance(); 
#endif
    //init WebBackForward object lijiangwei
    m_backForwardList = MDWebBackForwardList::createInstance(static_cast<WebCore::BackForwardListImpl*>(m_page->backForwardList())); 

	// load mdlphin resource ,such as mediacontrol .etc.
	loadInnerResource();
   
#if defined(_MD_ENABLE_WATERMARK) || defined(_MD_ENABLE_LOADSPLASH)
    HDC memdc = getMemDC(hwnd, 1, 1);
#endif

#ifdef _MD_ENABLE_WATERMARK
    waterMark::initWaterMark(memdc); //m_backingStoreMemDC);
#endif

#ifdef _MD_ENABLE_LOADSPLASH
    m_loadSplash = new loadSplash(hwnd, memdc); //m_backingStoreMemDC);
    m_loadSplash->calcOffsetPos();
#endif

#if defined(_MD_ENABLE_WATERMARK) || defined(_MD_ENABLE_LOADSPLASH)
    ReleaseDC(memdc);
#endif

}
//END_MDWEBVIEW_CONSTRUCTOR

//END_MDWEBVIEW_INNERFUNC

//START_MDWEBVIEW_MINIGUICONTROL


bool MDWebView::registerWebViewWindowClass()
{
    if (MDWebView::m_haveRegisteredWindowClass)  
        return true;     
    MDWebView::m_haveRegisteredWindowClass = true;   

    //init WebCore 
    JSC::initializeThreading();
    WTF::initializeMainThread();

    WebCore::InitializeLoggingChannelsIfNecessary();
    WebCore::RegisterPopmenu();

#if ENABLE(PLUGIN)
    WebCore::RegisterPluginControl();
#endif


    //init global delegate
    init_global_delegates();

    WNDCLASS MDViewClass;
    MDViewClass.spClassName = MDOLPHIN_CTRL;
    MDViewClass.dwStyle = WS_NONE;
    MDViewClass.dwExStyle = WS_EX_NONE;
    MDViewClass.hCursor = GetSystemCursor (IDC_ARROW);
    MDViewClass.iBkColor = COLOR_lightwhite;
    MDViewClass.WinProc = MDWebView::WebViewWndProc;

    return RegisterWindowClass (&MDViewClass);
}

bool MDWebView::unregisterWebViewWindowClass()
{
    if (!MDWebView::m_haveRegisteredWindowClass)  
        return false;

    bool ret = false;
#if ENABLE(PLUGIN)
    WebCore::UnregisterPluginControl();
#endif
    WebCore::UnregisterPopmenmu ();

    ret =  UnregisterWindowClass (MDOLPHIN_CTRL);
    if (ret) {
        finish_global_delegates();
        MDWebView::m_haveRegisteredWindowClass = false;
    }
    return ret;
}

bool MDWebView::focusOnChild (HWND hwnd, DWORD lParam)
{
    //FIXME: I want to implements popup menu and don't using control feature.
    if (GetFocusChild(hwnd))
    {
        int x_pos = LOSWORD (lParam);
        int y_pos = HISWORD (lParam);
        RECT focusrect;
        GetWindowRect(GetFocusChild(hwnd), &focusrect);
        if(PtInRect(&focusrect, x_pos, y_pos))
            return true;
        else
            return false;
    }
    return false;
}

void MDWebView::executeScript(const char* script)
{
    if (!script)
        return;

    Frame* coreFrame = core(m_mainFrame);
    if (!coreFrame)
        return;

    coreFrame->script()->executeScript(String::fromUTF8(script), true);
}

bool MDWebView::getFocusedEditorInfo(MDEditorElement* editorInfo)
{
    Frame* frame = core(m_mainFrame);
    bool result=false;
    if (editorInfo && frame && frame->document() && frame->document()->focusedNode()) {
        Node* node = frame->document()->focusedNode();
        RenderObject* renderer =node->renderer();
        if(!renderer)
            return false;
        // set pos
        int x_editor = node->getRect().x();
        int y_editor = node->getRect().y();

        RenderView* render_view = renderer->view();
        ASSERT(render_view);
        IntRect frame_rect = render_view->frameView()->windowClipRect();
        int x_view = render_view->viewRect().x();
        int y_view = render_view->viewRect().y();

        int offsetX = (x_editor>x_view)?(x_editor-x_view):0;
        int offsetY = (y_editor>y_view)?(y_editor-y_view):0;
        editorInfo->xPos = frame_rect.x() + offsetX; 
        editorInfo->yPos = frame_rect.y() + offsetY;
        editorInfo->width = node->getRect().width();
        editorInfo->height = node->getRect().height();
        //the input type has no common method to get
        //HTMLInputElement and  WMLInputElement is diffrent with each other

        //test if is html
        String node_name = node->nodeName().upper();
        if(node->document()->isHTMLDocument()){
            if( node_name == String("INPUT") ){
                HTMLInputElement* inputElement=static_cast<HTMLInputElement*>(node);
                //FIXME!!!
                //editorInfo->type = (MDEEditorType)inputElement->inputType();
                editorInfo->maxLen = inputElement->maxLength();
                result=true;
            }
            else if( node_name == String("TEXTAREA") ){
                editorInfo->type = MD_EDITOR_TEXTAREA;
                editorInfo->maxLen = 0;
                result=true;
            }
        }
#if ENABLE(WML)
        else if(node->document()->isWMLDocument()){
            if( node_name == String("INPUT") ){
                WMLInputElement* inputElement=static_cast<WMLInputElement*>(node);
                editorInfo->type = inputElement->isPasswordField()?MD_EDITOR_PASSWORD:MD_EDITOR_TEXT;
                editorInfo->maxLen = inputElement->s_maximumLength;
                result=true;
            }
        }
#endif
    }
    return result;
}

void MDWebView::dealWithMDMessages(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    const char* type = (const char*)lParam;
    if (strcmp(type, "load-error") == 0) {
        ResourceError* err = (ResourceError*)wParam;
		if(err){
			char* str = copyStringToUtf8Char(err->failingURL());
			frameLoadDelegate()->didFailLoad(this, m_mainFrame, err->errorCode(), str);
			freeUtf8Char(str);
			//the err is coming from a deep copy new , so we should delete it ,after used it.
			delete err;
		}
    }
}

int MDWebView::WebViewWndProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    MDWebView* view = reinterpret_cast<MDWebView*>(GetWindowAdditionalData2(hwnd));
    switch (message){
        case MSG_CREATE:{
                IncludeWindowExStyle(hwnd, WS_EX_CLIPCHILDREN);
                ExcludeWindowStyle (hwnd, WS_HSCROLL);
                ExcludeWindowStyle (hwnd, WS_VSCROLL);

                MDWebView *view = MDWebView::createInstance();
                view->init(hwnd);
                SetWindowAdditionalData2(hwnd, (DWORD)view);
                return 0;
        }

        case MSG_MOUSEMOVE:
            {
                if (view)
                    if (Frame* coreFrame = core(view->m_mainFrame))
                        if (!coreFrame->view()->didFirstLayout())
                            break; 
                if (MDWebView::focusOnChild (hwnd, lParam))
                    break;
                if(GetCapture()==hwnd){
                    int mouseX=LOWORD(lParam);
                    int mouseY=HIWORD(lParam);
                    ScreenToClient (hwnd, &mouseX, &mouseY);
                    lParam=mouseX|mouseY<<16;
                }
            
                if (view)
                    view->mouseMove(message, wParam, lParam);
            }
            break;
        case MSG_LBUTTONDOWN:
            {
                if (view)
                    if (Frame* coreFrame = core(view->m_mainFrame))
                        if (!coreFrame->view()->didFirstLayout())
                            break; 
                if(GetCapture()==HWND_NULL){
                    SetCapture(hwnd);
                }

                SetFocus(hwnd);
                if (focusOnChild (hwnd, lParam))
                    break;

                if (view)
                    view->mouseDown(message, wParam, lParam);
            }
            break;
        case MSG_LBUTTONUP:
            {
                if (view)
                    if (Frame* coreFrame = core(view->m_mainFrame))
                        if (!coreFrame->view()->didFirstLayout())
                            break; 
                if(GetCapture()==hwnd){
                    ReleaseCapture();
                    //we must transfer the x,y coordinate 
                    //or when click link ,it may not response(the mouse postion is not correct
                    int mouseX=LOWORD(lParam);
                    int mouseY=HIWORD(lParam);
                    ScreenToClient (hwnd, &mouseX, &mouseY);
                    lParam=mouseX|mouseY<<16;

                }

                if (MDWebView::focusOnChild (hwnd, lParam))
                    break;

                if (view)
                    view->mouseUp(message, wParam, lParam);
            }
            break;
        case MSG_RBUTTONDOWN:
            {
                if (view)
                    if (Frame* coreFrame = core(view->m_mainFrame))
                        if (!coreFrame->view()->didFirstLayout())
                            break; 
#if ENABLE(CONTEXT_MENUS)
                if (view)
                    view->contextMenu(message, wParam, lParam);
#endif
            }
            break;
        case MSG_KEYDOWN:
            {
                if (view) 
                    view->keyDown(message, wParam, lParam);
            }
            break;
        case MSG_KEYUP:
            {
                if (view)
                    view->keyUp(message, wParam, lParam);
            }
            break;
        case MSG_CHAR: 
            {
                if (view) 
                    view->charInput(wParam, lParam);
            }
            break;
        case MSG_SETFOCUS: 
            {
                if (view)
                    view->setFocus();
            }
            break;
        case MSG_KILLFOCUS:
            {
                if (view)
                    view->killFocus();
            }
            break;
        case MSG_DOESNEEDIME:
            {
                if (view)
                    return view->isFocusEditable() ? TRUE : FALSE;
            }
            break;
        case MSG_ERASEBKGND:
            return 0;

        case MSG_PAINT:
            {
                if (view)
                    view->paint();
            }
            return 0;
        case MSG_SIZECHANGED:
            {
                if (view) { 
                    RECT* rc = (RECT*)lParam;
                    view->resize(RECTWP(rc), RECTHP(rc));
                }
            }
            break;


        case MSG_DESTROY:
            {
                if (view)
                    view->release();

				//cleanup the javascript resource
				cleanupJSGlobalData();
                return 0;
            }

        case MSG_GSTREAMER:
            {
                typedef  int (*glibfunc_t)(void*);
                glibfunc_t glibfunc=(glibfunc_t)wParam; 
                glibfunc((void*)lParam);
                break;
            }
        case MSG_MDCORE:
            {
                view->dealWithMDMessages(hwnd, wParam, lParam);
                break;
            }

    }           
    return DefaultControlProc (hwnd, message, wParam, lParam);

}
void  MDWebView::saveas(bool htmlonly,const char * savedName)
{
    DomSerializer ds(core(m_mainFrame),!htmlonly,savedName);
    ds.SerializeDom();
    return;
}

//END_MDWEBVIEW_MINIGUICONTROL

//START_MDWEBVIEW_CREATEWEBVIEWINSTANCE
extern "C"
{

IMDWebView* mdCreateWebViewInstance(RECT rect, int id, HWND hParent)
{
    MDWebView::registerWebViewWindowClass();

    HWND hwnd  = CreateWindow (MDOLPHIN_CTRL,
            "", 
            WS_VISIBLE | WS_CHILD,
            id,
            rect.left, rect.top, RECTW(rect), RECTH(rect), hParent, 0); 

    IMDWebView*  view = reinterpret_cast<IMDWebView*>(GetWindowAdditionalData2(hwnd));

    return view;
}

IMDWebView* mdGetWebViewInstance(HWND hWnd)
{
    IMDWebView*  view = reinterpret_cast<IMDWebView*>(GetWindowAdditionalData2(hWnd));
    return view;
}

} //extern "C"

//END_MDWEBVIEW_CREATEWEBVIEWINSTANCE

void GCCollectJSObjects()
{
    gcController().garbageCollectNow();
}

void GCCollectJSObjectsOnAlternateThread(bool waitUntilDone)
{
    gcController().garbageCollectOnAlternateThreadForDebugging(waitUntilDone);
}

size_t GCCollectJSObjectsCount()
{
    JSC::JSLock lock(JSC::SilenceAssertionsOnly);
    return JSDOMWindow::commonJSGlobalData()->heap.objectCount();
}
