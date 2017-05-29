/*
 ** $Id$
 **
 ** IMDWebCustomDelegate.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-13 
 */


#include <string.h>

#include "minigui.h"
#include "config.h"
#include "mdolphin.h"

#include "IMDWebView.h"
#include "MDDefaultWebCustomDelegate.h"
#include "MDolphinDelegates.h"

//define the common struct of this delegate
#ifdef __cplusplus

MDDefaultWebCustomDelegate * MDDefaultWebCustomDelegate::createInstance()
{
    return new MDDefaultWebCustomDelegate;
}

const char* MDDefaultWebCustomDelegate::userArget(IMDWebView*, const char* url)
{
    if(Control::MDCB_CUSTOM_USERAGENT) {
        return Control::MDCB_CUSTOM_USERAGENT(url);
    }

    return NULL;
}

void MDDefaultWebCustomDelegate::beforeDrawViewport(IMDWebView* view, HDC windowDC,
                                 HDC memDC, const RECT* prcClient)
{
    HWND hwnd = view->viewWindow();
    if(Control::MDCB_BEFORE_DRAWING) {
        Control::MDCB_BEFORE_DRAWING(hwnd, windowDC, prcClient);
    }
    return;
}

void MDDefaultWebCustomDelegate::afterDrawViewport(IMDWebView* view, HDC windowDC, HDC memDC, const RECT *prcClient)
{
    HWND hwnd = view->viewWindow();
    if(Control::MDCB_AFTER_DRAWING) {
        Control::MDCB_AFTER_DRAWING(hwnd, windowDC, prcClient);
    }
}

void  MDDefaultWebCustomDelegate::linkURL(const char *url) 
{
    if(Control::MDCB_URL_IS_VISITED) 
        Control::MDCB_URL_IS_VISITED(url);
}
#endif

