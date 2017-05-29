/*
 ** $Id: MDDefaultWebUIDelegate.h 569 2011-03-30 07:29:33Z zjwang $
 **
 ** MDDefaultWebUIDelegate.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-09 
 */

#ifndef MDDefaultWebUIDelegate_H
#define MDDefaultWebUIDelegate_H

#include "IUnknown.h"
#include "IMDWebUIDelegate.h"

#ifdef __cplusplus

class MDDefaultWebUIDelegate : public IMDWebUIDelegate
{
public:
    static MDDefaultWebUIDelegate* createInstance();
protected:
    MDDefaultWebUIDelegate();
    ~MDDefaultWebUIDelegate();

public:
    /**
     * createWebViewWithRequest
     */
    virtual HWND createWebViewWithRequest(IMDWebView*, MDFrameLoaderInfo*, MDWindowFeatureInfo*);

    /**
     * webViewShow
     */
    virtual void webViewShow(IMDWebView* );

    /**
     * setFrame
     */
    virtual void setFrame(IMDWebView*, const RECT*);

    /*
     * webViewFrame
     */
    virtual BOOL webViewFrame(IMDWebView*, RECT *);

    /*
     * webViewFocus
     */
    virtual void webViewFocus(IMDWebView*, BOOL bFocus);

    /**
     * canTakeFocus
     */
    virtual BOOL canTakeFocus(IMDWebView*, BOOL bForward);

    /**
     * takeFocus
     */
    virtual BOOL takeFocus(IMDWebView*, BOOL bForward);
    
    /**
     * canRunModal
     */
    virtual BOOL canRunModal(IMDWebView*);
    /**
     * runModal
     */
    virtual BOOL runModal(IMDWebView*);

    /**
     * setBarVisible
     */
    virtual void setBarVisible(IMDWebView*, MDEBarType barType, BOOL bVisible);
    /**
     * isBarVisible
     */
    virtual BOOL isBarVisible(IMDWebView*, MDEBarType barType);
    
    /**
     * setResizeable
     */
    virtual void setResizeable(IMDWebView*, bool resizeable);

    /**
     * webViewAddMessageToConsole
     */
    virtual void webViewAddMessageToConsole(IMDWebView*, const char* message, int line, const char* url, int level, int source_type);
    
    /**
     * runJavaScriptAlertPanelWithMessage
     */
    virtual void runJavaScriptAlertPanelWithMessage(IMDWebView*, const char* strMessage);

    /**
     * runJavaScriptConfirmPanelWithMessage
     */
    virtual BOOL runJavaScriptConfirmPanelWithMessage(IMDWebView*, const char* string);

    /**
     * runJavaScriptTextInputPanelWithPrompt
     */
    virtual char* runJavaScriptTextInputPanelWithPrompt(IMDWebView*, const char* strMessage, const char* strDefValue);

    /**
     * setStatusText
     */
    virtual void setStatusText(IMDWebView*, const char* strStatusText);
    /**
     * shouldInterruptJavaScript
     */
    virtual BOOL shouldInterruptJavaScript(IMDWebView*);
    /**
     * chooseFile
     */
    virtual BOOL chooseFiles(IMDWebView*, MDChooseFilesInfo*);

    /**
     * printFrame
     */
    virtual void printFrame(IMDWebView*, IMDWebFrame*);

    /**
     * webViewClose
     */
    virtual void webViewClose(IMDWebView*);
    
    /**
     * mouseDidMoveOverElement 
     */
    virtual void mouseDidMoveOverElement(IMDWebView*, const char*);

    /**
     * hasCustomerIME
     */
    virtual BOOL hasCustomerIME();

    /**
     * setCustomerIME
     */
    //please refrence with http://bugzilla.rd.minigui.com/show_bug.cgi?id=5499
    virtual void setCustomerIME(IMDWebView*, BOOL active);

    /**
     * didChangeCaret
     */
    virtual void didChangeCaret(IMDWebView*, const RECT* rect);

    /**
     * hasCustomMenuImplementation
     */
    virtual BOOL hasCustomMenuImplementation();

    /**
     * trackCustomPopupMenu
     */
    virtual void trackCustomPopupMenu(IMDWebView*, const MDPopupMenu*);

    /**
     * receivedAuthentication
     */
    virtual BOOL receivedAuthentication(IMDWebView*, const char* title, char* user, size_t userNameMaxLen,
            char* password, size_t passwordMaxLen);

    /**
     * setToolTip
     */
    virtual void setToolTip(IMDWebView*, const char* strTip, int textDirection);

    /**
     * paintCustomScrollCorner
     */
    virtual BOOL paintCustomScrollCorner(IMDWebView* webView, HDC hdc, RECT rect);
};


#endif


#endif

