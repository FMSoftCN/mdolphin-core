/*
 ** $Id$
 **
 ** MDWebView.h: the header file of MDWebView.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: lvlei
 **
 ** Create date: 2010-07-08
 */

#ifndef MDWebView_h
#define MDWebView_h

#include "IMDWebView.h"
#include "MDWebFrame.h"
#include "MDWebBackForwardList.h"

#include "IntRect.h"

#ifdef _MD_ENABLE_LOADSPLASH
#include "license/loadSplash.h"
#endif

namespace WebCore
{
class Page;
class Settings;
class KURL;
}

class MDWebSettings;
class MDWebInspector;

WebCore::Page* core(MDWebView* WebView);

class MDWebView: public IMDWebView
{
public:
    //START_MDWEBVIEW_INITWINDOW
    static MDWebView* createInstance();
    static bool registerWebViewWindowClass();
    static bool unregisterWebViewWindowClass();
    static bool m_haveRegisteredWindowClass;
    //static int WebViewWndProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam);
    static LRESULT WebViewWndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);       // gengyue
    void init(HWND hwnd);
    virtual HWND viewWindow();
    //END_MDWEBVIEW_INITWINDOW

    //START_MDWEBVIEW_NAVIGATION
    virtual bool loadURL(const char * sUrl) ;
    virtual void stopLoading(void) ;
    virtual void reload(void);
    virtual void goBack(void);
    virtual void goForward(void);
    virtual bool canGoBack(void);
    virtual bool canGoForward(void);
    //END_MDWEBVIEW_NAVIGATION


    //START_MDWEBVIEW_SEARCH
    virtual bool search(const char *str, bool forward, bool caseSensitive, bool wrap);
    virtual unsigned int markAllMatches(const char *str, bool caseSensitive, bool highlight, unsigned int limit);
    virtual void unmarkAllMatches(void);
    //END_MDWEBVIEW_SEARCH

    //START_MDWEBVIEW_CONTENTPOSITIPON
    virtual void contentsSize(int* w, int* h);
    virtual void contentPos(int *x, int *y);
    virtual void setContentPos(int x, int y);
    virtual void scrollBy(int offsetX, int offsetY);
    //END_MDWEBVIEW_CONTENTPOSITIPON

    //START_MDWEBVIEW_ZOOM
    virtual void setTextSizeMultiplier(float multiplier);
    virtual float textSizeMultiplier();
    // zoompage
    virtual void setPageSizeMultiplier(float multiplier);
    virtual float pageSizeMultiplier();
    void setZoomMultiplier(float multiplier, bool isTextOnly);
    float zoomMultiplier(bool isTextOnly);
    //END_MDWEBVIEW_ZOOM

    //START_MDWEBVIEW_PROGRESS
    virtual double estimatedProgress();
    virtual bool isLoading();
    //END_MDWEBVIEW_PROGRESS

    //START_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST
    virtual MDWebSettings * mdWebSettings();
    virtual MDWebInspector* mdWebInspector();
    virtual IMDWebBackForwardList * mdWebBackForwardList();
    //END_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST

    //START_MDWEBVIEW_PAGEINFO
    virtual void setCustomTextEncodingName(const char *encodingName);
    virtual char* customTextEncodingName();
    virtual char* mainFrameURL();
    virtual IMDWebFrame*  mainWebFrame();
    MDWebFrame* topLevelFrame() const { return m_mainFrame; };
    //END_MDWEBVIEW_PAGEINFO

    //START_MDWEBVIEW_MIME
    virtual bool canShowMIMEType(const char *mime);
    //END_MDWEBVIEW_MIME

    //START_MDWEBVIEW_MENUCONTEXT
#if ENABLE(CONTEXT_MENUS)
    virtual void performContextAction(MDEPopupMenuElementEvent event);
    bool contextMenu(int message, WPARAM wParam, LPARAM lParam);
#endif

    void downloadURL(const WebCore::KURL&);
    //END_MDWEBVIEW_MENUCONTEXT

    //START_MDWEBVIEW_GETANDSETDELEGATE
    virtual void setUIDelegate(IMDWebUIDelegate* d);
    virtual IMDWebUIDelegate*  uiDelegate();
    virtual void setDownloadDelegate(IMDWebDownloadDelegate* d);
    virtual IMDWebDownloadDelegate* downloadDelegate( );
    virtual void setHistoryDelegate(IMDWebHistoryDelegate* d);
    virtual IMDWebHistoryDelegate* historyDelegate( );
    virtual void setCustomDelegate(IMDWebCustomDelegate* d);
    virtual IMDWebCustomDelegate* customDelegate();
    virtual void setPolicyDelegate(IMDWebPolicyDelegate* d);
    virtual IMDWebPolicyDelegate* policyDelegate();
    virtual void setFrameLoadDelegate(IMDWebFrameLoadDelegate* d);
    virtual IMDWebFrameLoadDelegate* frameLoadDelegate();
    //END_MDWEBVIEW_GETANDSETDELEGATE

    void executeScript(const char* script);
    virtual bool getFocusedEditorInfo(MDEditorElement* elment);

    //START_MDWEBVIEW_EVENT
    void mapMousePosition(WPARAM* wParam, LPARAM* lParam);
    bool mouseMove(int message, WPARAM wParam, LPARAM lParam);
    bool mouseDown(int message, WPARAM wParam, LPARAM lParam);
    bool mouseUp(int message, WPARAM wParam, LPARAM lParam);
    bool keyDown(int message, WPARAM wParam, LPARAM lParam);
    bool keyUp(int message, WPARAM wParam, LPARAM lParam);
    //END_MDWEBVIEW_EVENT

    //START_MDWEBVIEW_MESSAGEHANDLER
    void setFocus(void);
    void killFocus(void);
    void setFocusEditable(bool b);
    bool isFocusEditable(void);
    bool charInput(unsigned int inputChar, unsigned int keyFlags);
    //END_MDWEBVIEW_MESSAGEHANDLER
    //

    bool setInViewSourceMode(bool);
    bool isInViewSourceMode();

    WebCore::Page* page();

    void resize(int newWidth, int newHeight);
    void paint(void);
    bool isPainting(void) const { return m_paintCount > 0; }
    void paintIntoWindow(HDC backingStoreDC, HDC windowDC, const WebCore::IntRect& dirtyRect);
    bool ensureBackingStore(void);
    void addToDirtyRegion(const WebCore::IntRect&);
    void addToDirtyRegion(PCLIPRGN newRegion);
    void scrollBackingStore(WebCore::FrameView*, int dx, int dy, 
            const WebCore::IntRect& scrollViewRect, const WebCore::IntRect& clipRect);
    void deleteBackingStore(void);
    void repaint(const WebCore::IntRect&, bool contentChanged, 
            bool immediate = false, bool repaintContentOnly = false);
    virtual bool invalidateBackingStore(const RECT*);
//save as
	void  saveas(bool htmlonly,const char* savedName);
    void drawLoadSplash(HDC hdc);
    void drawWaterMark(HDC hdc);
    void loadFinished();
private:
    MDWebView();
    void updateBackingStore(WebCore::FrameView*, bool backingStoreCompletelyDirty = false);
    void paintIntoBackingStore(WebCore::FrameView*, HDC backingStoreDC, const WebCore::IntRect& dirtyRect);

    void dealWithMDMessages(HWND hwnd, WPARAM wParam, LPARAM lParam);

    static void initSettings(WebCore::Settings *settings);
    static bool focusOnChild (HWND hwnd, DWORD lParam);

public:
    ~MDWebView();

private:
    //START_MDWEBVIEW_WINDOW
    HWND m_viewWindow;
    MDWebFrame* m_mainFrame;
    WebCore::Page* m_page;
    float m_zoomMultiplier;
    bool m_zoomsTextOnly;
    bool m_isFocusEditable;
    //END_MDWEBVIEW_WINDOW

    //START_MDWEBVIEW_BACKINGSTORE
    HDC m_backingStoreMemDC;
    SIZE m_backingStoreSize;
    unsigned int m_paintCount;
    PCLIPRGN m_backingStoreDirtyRegion;
    //END_MDWEBVIEW_BACKINGSTORE

    //START_MDWEBVIEW_DELEGATE
    IMDWebUIDelegate *m_uiDelegate;
    IMDWebDownloadDelegate *m_downloadDelegate;
    IMDWebHistoryDelegate *m_historyDelegate;
    IMDWebCustomDelegate *m_customDelegate;
    IMDWebPolicyDelegate *m_policyDelegate;
    IMDWebFrameLoadDelegate *m_frameLoadDelegate;


    //END_MDWEBVIEW_DELEGATE

    //START_MDWEBVIEW_PREFERENCES
    MDWebSettings* m_pwebSettings;
    MDWebInspector* m_inspector;
    //END_MDWEBVIEW_PREFERENCES
    //START_MDWEBVIEW_BACKFORWARDLIST
    MDWebBackForwardList *m_backForwardList;
    //END_MDWEBVIEW_BACKFORWARDLIST
#ifdef _MD_ENABLE_LOADSPLASH
    loadSplash* m_loadSplash;
#endif
};

#endif
