/*
 ** $Id: MDDefaultWebUIDelegate.cpp 759 2011-08-15 07:36:09Z xueshuming $
 **
 ** MDDefaultWebUIDelegate.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-09 
 */

#include "minigui.h"
#include "config.h"
#include "mdolphin.h"

#include "IMDWebView.h"
#include "MDDefaultWebCustomDelegate.h"
#include "MDolphinDelegates.h"
#include "MDDefaultWebUIDelegate.h"

#include <string.h>

MDDefaultWebUIDelegate::MDDefaultWebUIDelegate()
{

}

MDDefaultWebUIDelegate::~MDDefaultWebUIDelegate()
{

}

MDDefaultWebUIDelegate* MDDefaultWebUIDelegate::createInstance()
{
    return new MDDefaultWebUIDelegate();
}

HWND MDDefaultWebUIDelegate::createWebViewWithRequest(IMDWebView* webView, MDFrameLoaderInfo* frameLoaderInfo, MDWindowFeatureInfo* windowFeatures)
{
    if(Control::MDCB_OPEN_WINDOW)
    {
        int x = 0, y = 0, width = 0, height =0;
        DWORD styles = 0;
        if(windowFeatures->xSet)
            x = windowFeatures->x;
        if(windowFeatures->ySet)
            y = windowFeatures->y;
        if(windowFeatures->widthSet)
            width = windowFeatures->width;
        if(windowFeatures->heightSet)
            height = windowFeatures->height;
        
        if(windowFeatures->locationBarVisible)
            styles |= CWS_TOOLBAR;
        if(windowFeatures->statusBarVisible)
            styles |= CWS_LOCATION;
        if(windowFeatures->scrollbarsVisible)
            styles |= CWS_SCROLLBARS;
        if(windowFeatures->menuBarVisible)
            styles |= CWS_MENUBAR;
        if(windowFeatures->dialog)
            styles |= CWS_MODAL;

        return Control::MDCB_OPEN_WINDOW(frameLoaderInfo->url,
                styles, x, y, width, height);

    }
    return HWND_NULL;
}

void MDDefaultWebUIDelegate::webViewShow(IMDWebView* webView)
{
    //do nothing
}

void MDDefaultWebUIDelegate::setFrame(IMDWebView* webView, const RECT* prc)
{
    //do nothing
    //TODO: call MoveWindow ?
}

BOOL MDDefaultWebUIDelegate::webViewFrame(IMDWebView* webView, RECT *rc)
{
    HWND hwnd = webView->viewWindow();
    if (!rc || hwnd == HWND_INVALID)
        return FALSE;

    GetClientRect(hwnd, rc);
    ClientToScreen(hwnd, &rc->left, &rc->top);
    ClientToScreen(hwnd, &rc->right, &rc->bottom);

    return TRUE;

}


void MDDefaultWebUIDelegate::webViewFocus(IMDWebView* webView, BOOL bFocus)
{
    SetFocus(bFocus?webView->viewWindow():HWND_INVALID);
}

BOOL MDDefaultWebUIDelegate::canTakeFocus(IMDWebView * webView, BOOL bForward)
{
    //do nothing
    return FALSE;
}

BOOL MDDefaultWebUIDelegate::takeFocus(IMDWebView* webView, BOOL bForward)
{
    //do nothing
    return FALSE;
}

BOOL MDDefaultWebUIDelegate::canRunModal(IMDWebView* webView)
{
    return TRUE;
}

BOOL MDDefaultWebUIDelegate::runModal(IMDWebView* wevView)
{
    return FALSE;
}

void MDDefaultWebUIDelegate::setBarVisible(IMDWebView* webView, MDEBarType bar, BOOL bVisible)
{
    //do nothing
}

BOOL MDDefaultWebUIDelegate::isBarVisible(IMDWebView* webView, MDEBarType bar)
{
    //do nothing
    return FALSE;
}

void MDDefaultWebUIDelegate::setResizeable(IMDWebView* webView, bool resizeable)
{
    //do nothing
}


void MDDefaultWebUIDelegate::webViewAddMessageToConsole(IMDWebView* webView,  const char* message, int line, const char* url, int level, int source_type)
{
    //do nothing
}
void MDDefaultWebUIDelegate::runJavaScriptAlertPanelWithMessage(IMDWebView* webView, const char* strMessage)
{
    if(Control::MDCB_MESSAGE_BOX)
        Control::MDCB_MESSAGE_BOX(webView->viewWindow(), strMessage, "JavaScript Alert");
}


BOOL MDDefaultWebUIDelegate::runJavaScriptConfirmPanelWithMessage(IMDWebView* webView, const char* strMessage)
{
    BOOL ret = FALSE;
    if(Control::MDCB_CONFIRM_BOX)
        ret = Control::MDCB_CONFIRM_BOX(webView->viewWindow(), strMessage, "JavaScript Confirm");

    return ret;
}

char* MDDefaultWebUIDelegate::runJavaScriptTextInputPanelWithPrompt(IMDWebView* webView, const char* strMessage, const char* strDefValue)
{
    if(Control::MDCB_PROMPT_BOX)
        return Control::MDCB_PROMPT_BOX(webView->viewWindow(), strMessage, strDefValue, "JavaScript Prompt");

    return NULL;
}

void MDDefaultWebUIDelegate::setStatusText(IMDWebView* webView, const char* strStatusText)
{
    //do nothing
}

BOOL MDDefaultWebUIDelegate::shouldInterruptJavaScript(IMDWebView* webView)
{
    return FALSE;
}

BOOL MDDefaultWebUIDelegate::chooseFiles(IMDWebView* webView, MDChooseFilesInfo* chooseInfo)
{
    if(Control::MDCB_CHOOSEFILE_BOX)
    {
        BOOL bret = FALSE;
        char szFileName[PATH_MAX] = "\0";
        webView->addRef(); //protected the wevView
        if(Control::MDCB_CHOOSEFILE_BOX(webView->viewWindow(), szFileName, PATH_MAX))
        {
            chooseInfo->files = (char**)malloc(sizeof(char*));
            chooseInfo->files[0] = strdup(szFileName);
            chooseInfo->files_count = 1;
            bret = TRUE;
        }
        webView->release();
        return bret;
    }
    return FALSE;
}

void MDDefaultWebUIDelegate::printFrame(IMDWebView *webView, IMDWebFrame *webFrame)
{
    //do nothing
}

void MDDefaultWebUIDelegate::webViewClose(IMDWebView* view)
{
    if (Control::MDCB_CLOSE_WINDOW)
        Control::MDCB_CLOSE_WINDOW(view->viewWindow());

}

void MDDefaultWebUIDelegate::mouseDidMoveOverElement(IMDWebView* view, const char* msg)
{
    if (Control::MDCB_SET_STATUS)
        Control::MDCB_SET_STATUS(view->viewWindow(), msg);
}

BOOL MDDefaultWebUIDelegate::hasCustomerIME()
{
    if (Control::MDCB_SET_IME_STATUS)
        return TRUE;
    else
        return FALSE;
}

void MDDefaultWebUIDelegate::setCustomerIME(IMDWebView* view, BOOL active)
{
    if (Control::MDCB_SET_IME_STATUS)
        Control::MDCB_SET_IME_STATUS(view->viewWindow(), active);
}

void MDDefaultWebUIDelegate::didChangeCaret(IMDWebView*, const RECT* rect)
{
    if (Control::MDCB_GET_CARET_RECT)
        Control::MDCB_GET_CARET_RECT(rect);
}

BOOL MDDefaultWebUIDelegate::hasCustomMenuImplementation()
{
    if (Control::MDCB_CREATE_POPUP_MENU)
        return TRUE;
    else
        return FALSE;
}

void MDDefaultWebUIDelegate::trackCustomPopupMenu(IMDWebView* view, const MDPopupMenu* data)
{
    if (Control::MDCB_CREATE_POPUP_MENU)
        Control::MDCB_CREATE_POPUP_MENU(view->viewWindow(), data);
}

BOOL MDDefaultWebUIDelegate::receivedAuthentication(IMDWebView* view, const char* title, char* userName, 
        size_t userNameMaxLen, char* password, size_t passwordMaxLen)
{
    if (Control::MDCB_PROVIDE_AUTH)
        return Control::MDCB_PROVIDE_AUTH(title, userName, userNameMaxLen, password, passwordMaxLen);

    return FALSE;
}

void MDDefaultWebUIDelegate::setToolTip(IMDWebView* view, const char* strTip, int textDirection)
{

    if (Control::MDCB_SET_TOOLTIP) {

        POINT pt;
        GetCursorPos(&pt);
        
        ScreenToClient(view->viewWindow(), &(pt.x), &(pt.y));
        BOOL bShow = (strTip != NULL);
        Control::MDCB_SET_TOOLTIP(view->viewWindow(), pt.x, pt.y, strTip, bShow);
        return;
    }

    return ;
}

BOOL MDDefaultWebUIDelegate::paintCustomScrollCorner(IMDWebView* webView, HDC hdc, RECT rect) 
{
    return false;
}
