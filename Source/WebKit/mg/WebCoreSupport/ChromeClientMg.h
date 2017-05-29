/*
** $Id: ChromeClientMg.h 1072 2008-03-18 09:59:10Z jpzhang $
**
** ChromeClientMg.h: Chrome Client header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-5
*/

#ifndef ChromeClientMg_H
#define ChromeClientMg_H

#include "minigui.h"

#include "ChromeClient.h"
#include "FloatRect.h"
#include "WindowFeatures.h"
#include "Element.h"

class MDWebView;

namespace WebCore {

class ChromeClientMg : public ChromeClient
{
public:
    ChromeClientMg(MDWebView*);
    virtual ~ChromeClientMg();

    virtual void chromeDestroyed(){delete this;}

    virtual void setWindowRect(const FloatRect&);
    virtual FloatRect windowRect();

    virtual FloatRect pageRect();

    virtual float scaleFactor();

    virtual void focus();
    virtual void unfocus();

    virtual bool canTakeFocus(FocusDirection);
    virtual void takeFocus(FocusDirection);

    virtual void focusedNodeChanged(Node*); //FIXME

    virtual void show(); //FIXME

    virtual bool canRunModal();
    virtual void runModal();

#if 1
    virtual void setToolbarsVisible(bool);
    virtual bool toolbarsVisible();

    virtual void setStatusbarVisible(bool);
    virtual bool statusbarVisible();

    virtual void setScrollbarsVisible(bool);
    virtual bool scrollbarsVisible();

    virtual void setMenubarVisible(bool);
    virtual bool menubarVisible();

    virtual void setResizable(bool);
#endif //FIXME
#if 0
    virtual bool toolbarVisible();
    virtual bool menubarVisible();
    virtual bool scrollbarsVisible();
    virtual bool statusbarVisible();
    virtual bool locationbarVisible();
    virtual bool personalbarVisible();
#endif
    virtual void addMessageToConsole(MessageSource, MessageType, MessageLevel, const String& message, unsigned int lineNumber, 
            const String& sourceID);
//FIXME
    //virtual void addMessageToConsole(const String& message, unsigned int lineNumber, const String& sourceID);

    virtual bool canRunBeforeUnloadConfirmPanel();
    virtual bool runBeforeUnloadConfirmPanel(const String& message, Frame* frame);

    virtual void closeWindowSoon();

    virtual void runJavaScriptAlert(Frame*, const String&);
    virtual bool runJavaScriptConfirm(Frame*, const String&);
    virtual bool runJavaScriptPrompt(Frame*, const String& message, const String& defaultValue, String& result);

    virtual void setStatusbarText(const String&);
    virtual bool shouldInterruptJavaScript();
    virtual WebCore::KeyboardUIMode keyboardUIMode();
    virtual bool tabsToLinks() const;

    virtual IntRect windowResizerRect() const;

#if 1
    // Methods used by HostWindow.
    virtual void invalidateWindow(const IntRect&, bool);
    virtual void invalidateContentsAndWindow(const IntRect&, bool);
    virtual void invalidateContentsForSlowScroll(const IntRect&, bool);
    virtual void scroll(const IntSize&, const IntRect&, const IntRect&);
    virtual IntPoint screenToWindow(const IntPoint&) const;
    virtual IntRect windowToScreen(const IntRect&) const;
    virtual PlatformPageClient platformPageClient() const;
    virtual void contentsSizeChanged(Frame*, const IntSize&) const;
    virtual void scrollRectIntoView(const IntRect&, const ScrollView*) const; // Currently only Mac has a non empty implementation.
    // End methods used by HostWindow.
#endif 

    virtual bool paintCustomScrollCorner(GraphicsContext* context, const FloatRect& rect);
        
    virtual void scrollbarsModeDidChange() const; //FIXME
    virtual void mouseDidMoveOverElement(const HitTestResult&, unsigned modifierFlags);

    virtual void setToolTip(const WTF::String&, WebCore::TextDirection);
    //virtual void setToolTip(const String&, const IntPoint&);
    virtual void print(Frame*);

#if 1
    virtual void requestGeolocationPermissionForFrame(Frame*, Geolocation*);
    virtual void cancelGeolocationPermissionRequestForFrame(Frame*, Geolocation*);

    virtual void runOpenPanel(Frame*, PassRefPtr<FileChooser>);
    // Asynchronous request to load an icon for specified filenames.
    virtual void chooseIconForFiles(const Vector<String>&, FileChooser*);

    virtual void setCursor(const Cursor&);

    MDWebView* webView() const { return m_webView; }

    // Notification that the given form element has changed. This function
    // will be called frequently, so handling should be very fast.
    virtual void formStateDidChange(const Node*);

#endif
    virtual void focusedFrameChanged(Frame*) {}
    virtual Page* createWindow(Frame*, const FrameLoadRequest&, const WindowFeatures&, const NavigationAction&);
    virtual bool selectItemWritingDirectionIsNatural();
    virtual bool selectItemAlignmentFollowsMenuWritingDirection();
    virtual PassRefPtr<PopupMenu> createPopupMenu(PopupMenuClient*) const;
    virtual PassRefPtr<SearchPopupMenu> createSearchPopupMenu(PopupMenuClient*) const;
#if ENABLE(CONTEXT_MENUS)
    virtual void showContextMenu() { }
#endif


#if ENABLE(DATABASE)
    virtual void exceededDatabaseQuota(WebCore::Frame*, const WTF::String&);
#endif

#if PLATFORM(MG)
    void setFocused(bool b) { m_focused = b; }
    bool isFocused(void) const { return m_focused; }
#endif

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
        virtual void reachedMaxAppCacheSize(int64_t spaceNeeded);
        virtual void reachedApplicationCacheOriginQuota(WebCore::SecurityOrigin*);
#endif

#if ENABLE(NOTIFICATIONS)
    virtual WebCore::NotificationPresenter* notificationPresenter() const { return reinterpret_cast<WebCore::NotificationPresenter*>(NULL); }            
#endif 

private:
    MDWebView *m_webView;
    Element* m_lastHoverElement;
    unsigned m_lastTooltipId;
    bool m_focused;
};

}

#endif // ChromeClientMg_H
