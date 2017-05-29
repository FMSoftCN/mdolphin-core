/*
 ** $Id: IMDWebUIDelegate.h 569 2011-03-30 07:29:33Z zjwang $
 **
 ** IMDWebUIDelegate.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-08 
 */

#ifndef IMDWebUIDelegate_H
#define IMDWebUIDelegate_H

#include "mdolphin.h"

#include "IUnknown.h"
#include "IMDWebView.h"
#include "IMDWebFrame.h"

//define the common struct of this delegate
#ifdef __cplusplus
extern "C" {
#endif

/* MDFrameLoaderInfo struct  */
typedef struct _MDFrameLoaderInfo {
    const char* url; /* The loader URL, utf8 code */
    /*for expand*/
}MDFrameLoaderInfo;

/* MDWindowFeatureInfo */
typedef struct _MDWindowFeatureInfo{
    int x;
    int y;
    int width;
    int height;

    //flags
    unsigned int xSet:1;
    unsigned int ySet:1;
    unsigned int widthSet:1;
    unsigned int heightSet:1;

    //visibles
    unsigned int toolBarVisible:1;
    unsigned int locationBarVisible:1;
    unsigned int statusBarVisible:1;
    unsigned int scrollbarsVisible:1;
    unsigned int menuBarVisible:1;
    unsigned int resizeable:1;
    unsigned int fullScreen:1;
    unsigned int dialog:1;

    //Ignored the WindowFeature's additonFeature, add it
}MDWindowFeatureInfo;


/* typedef Bar type for setBarVisible and isBarVisible */
typedef enum {
    MD_TOOLBAR = 0,
    MD_STATUSBAR,
    MD_SCROLLBAR,
    MD_MENUBAR
}MDEBarType;

/**
 * MDChooseFilesInfo
 */
typedef struct _MDChooseFilesInfo {
    BOOL        isMutilFiles;
    const char* acceptTypes;

    /* this is the output params */
    int   files_count;
    char** files;
}MDChooseFilesInfo;

void mdCleanChooseFilesInfo(MDChooseFilesInfo*);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class IMDWebUIDelegate : public IUnknown
{
public:
    /**
     * createWebViewWithRequest
     */
    virtual HWND createWebViewWithRequest(IMDWebView*, MDFrameLoaderInfo*, MDWindowFeatureInfo*) = 0;

    /**
     * webViewShow
     */
    virtual void webViewShow(IMDWebView* ) = 0;

    /**
     * setFrame
     */
    virtual void setFrame(IMDWebView*, const RECT*) = 0;

    /*
     * webViewFrame
     */
    virtual BOOL webViewFrame(IMDWebView*, RECT *) = 0;

    /*
     * webViewFocus
     */
    virtual void webViewFocus(IMDWebView*, BOOL bFocus) = 0;

    /**
     * canTakeFocus
     */
    virtual BOOL canTakeFocus(IMDWebView*, BOOL bForward) = 0;

    /**
     * takeFocus
     */
    virtual BOOL takeFocus(IMDWebView*, BOOL bForward) = 0;
    
    /**
     * canRunModal
     */
    virtual BOOL canRunModal(IMDWebView*) = 0;
    /**
     * runModal
     */
    virtual BOOL runModal(IMDWebView*) = 0;

    /**
     * setBarVisible
     */
    virtual void setBarVisible(IMDWebView*, MDEBarType barType, BOOL bVisible) = 0;
    /**
     * isBarVisible
     */
    virtual BOOL isBarVisible(IMDWebView*, MDEBarType barType) = 0;
    
    /**
     * setResizeable
     */
    virtual void setResizeable(IMDWebView*, bool resizeable) = 0;

    /**
     * webViewAddMessageToConsole
     */
    virtual void webViewAddMessageToConsole(IMDWebView*, const char* message, int line, const char* url, int level, int source_type) = 0;
    
    /**
     * runJavaScriptAlertPanelWithMessage
     */
    virtual void runJavaScriptAlertPanelWithMessage(IMDWebView*, const char* strMessage) = 0;

    /**
     * runJavaScriptConfirmPanelWithMessage
     */
    virtual BOOL runJavaScriptConfirmPanelWithMessage(IMDWebView*, const char* string) = 0;

    /**
     * runJavaScriptTextInputPanelWithPrompt
     */
    virtual char* runJavaScriptTextInputPanelWithPrompt(IMDWebView*, const char* strMessage, const char* strDefValue) = 0;

    /**
     * setStatusText
     */
    virtual void setStatusText(IMDWebView*, const char* strStatusText) = 0;
    /**
     * shouldInterruptJavaScript
     */
    virtual BOOL shouldInterruptJavaScript(IMDWebView*) = 0;
    /**
     * chooseFile
     */
    virtual BOOL chooseFiles(IMDWebView*, MDChooseFilesInfo*) = 0;

    /**
     * printFrame
     */
    virtual void printFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * webViewClose
     */
    virtual void webViewClose(IMDWebView*) = 0;

    /**
     * mouseDidMoveOverElement 
     */
    virtual void mouseDidMoveOverElement(IMDWebView*, const char*) = 0;

    /**
     * hasCustomerIME
     */
    virtual BOOL hasCustomerIME() = 0;

    /**
     * setCustomerIME
     */
    //please refrence with http://bugzilla.rd.minigui.com/show_bug.cgi?id=5499
    virtual void setCustomerIME(IMDWebView*, BOOL active) = 0;

    /**
     * didChangeCaret
     */
    virtual void didChangeCaret(IMDWebView*, const RECT* rect) = 0;

    /**
     * hasCustomMenuImplementation
     */
    virtual BOOL hasCustomMenuImplementation() = 0;

    /**
     * trackCustomPopupMenu
     */
    virtual void trackCustomPopupMenu(IMDWebView*, const MDPopupMenu*) = 0;
    
    /**
     * receivedAuthentication
     */
    virtual BOOL receivedAuthentication(IMDWebView*, const char* title, char* userName, size_t userNameMaxLen,
            char* password, size_t passwordMaxLen) = 0;

    /**
     * setToolTip
     */
    virtual void setToolTip(IMDWebView*, const char* strTip, int textDirection) {};

    /**
     * paintCustomScrollCorner
     */
    virtual BOOL paintCustomScrollCorner(IMDWebView* webView, HDC hdc, RECT rect) {return false;};
};


#endif

#endif


