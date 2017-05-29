/*
 ** $Id: MDDefaultWebFrameLoadDelegate.cpp 1 2010-07-12 03:29:10Z lijiangwei $ 
 **
 ** MDDefaultWebFrameLoadDelegate.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-13 
 */

#include "minigui.h"
#include "config.h"

#include "mdolphin.h"
#include "MDDefaultWebFrameLoadDelegate.h"
#include "MDolphinDelegates.h"
#include "MDWebFrame.h"
#include "Frame.h"
#include "FrameView.h"
#include "Logging.h"
#include "IMDWebBackForwardList.h"
#include "MDCommonFunc.h"
#include "KURL.h"

using namespace WebCore;

MDDefaultWebFrameLoadDelegate::MDDefaultWebFrameLoadDelegate()
{
}

MDDefaultWebFrameLoadDelegate::~MDDefaultWebFrameLoadDelegate()
{
}

MDDefaultWebFrameLoadDelegate* MDDefaultWebFrameLoadDelegate::sharedInstance()
{
    static MDDefaultWebFrameLoadDelegate* shared;
    if (!shared)
        shared = MDDefaultWebFrameLoadDelegate::createInstance();
    return shared;
}

MDDefaultWebFrameLoadDelegate* MDDefaultWebFrameLoadDelegate::createInstance()
{
    MDDefaultWebFrameLoadDelegate* instance = new MDDefaultWebFrameLoadDelegate();
    return instance;
}

static void updateNavigations(IMDWebView* webView)
{
    if(Control::MDCB_SET_HISTORY_STATUS)
    {
        IMDWebBackForwardList * backforwardList = webView->mdWebBackForwardList();
        Control::MDCB_SET_HISTORY_STATUS(webView->viewWindow(), backforwardList->backListCount(), 
                backforwardList->forwardListCount(), backforwardList->capacity());
    }
}

void MDDefaultWebFrameLoadDelegate::didReceiveResponse(IMDWebView* webView, IMDWebFrame* webFrame, unsigned long identifier, MDHResponse response)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didReceiveResponse %p %p", webView, webFrame);

    if(Control::MDCB_SET_STATUS)
    {
        char* url = mdRespGetURL(response);
        if (url && (url[0]!='\0')){
            char szMsg [3072];
            snprintf(szMsg, sizeof(szMsg), "Waiting for %s" , url);
            Control::MDCB_SET_STATUS(webView->viewWindow(), szMsg);
        }
        //remember!
        freeUtf8Char(url);
    }
}

void MDDefaultWebFrameLoadDelegate::didReceiveTitle(IMDWebView* webView, IMDWebFrame* webFrame, const char* title)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didReceiveTitle %p %p", webView, webFrame);

    if(Control::MDCB_SET_TITLE && (webView->mainWebFrame() == webFrame))
    {
        Control::MDCB_SET_TITLE(webView->viewWindow(),  title);
    }
}

void MDDefaultWebFrameLoadDelegate::identifierForInitialRequest(IMDWebView* webView, IMDWebFrame* webFrame, MDHRequest req, unsigned long identifier)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - identifierForInitialRequest %p %p", webView, webFrame);
}

void MDDefaultWebFrameLoadDelegate::didFailLoad(IMDWebView* webView, IMDWebFrame* webFrame, int code, const char* url)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didFailLoad %p %p", webView, webFrame);
    if(Control::MDCB_ERROR)
        Control::MDCB_ERROR(webView->viewWindow(), code, url);
}

void MDDefaultWebFrameLoadDelegate::didFailProvisionalLoad(IMDWebView* webView, IMDWebFrame* webFrame, int code, const char* url)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didFailProvisionalLoad %p %p", webView, webFrame);
    if(Control::MDCB_ERROR)
        Control::MDCB_ERROR(webView->viewWindow(), code, url);
}

void MDDefaultWebFrameLoadDelegate::didProgressStarted(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didProgressStarted %p %p", webView, webFrame);
    if (Control::MDCB_SET_LOADING_STATUS && core(static_cast<MDWebFrame*>(webFrame))->loader()->isLoadingMainFrame()) 
        Control::MDCB_SET_LOADING_STATUS (webView->viewWindow(), TRUE, 0);
    updateNavigations(webView);
}

void MDDefaultWebFrameLoadDelegate::didProgressEstimateChanged(IMDWebView* webView, IMDWebFrame* webFrame, int percent)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didProgressEstimateChanged %p %p", webView, webFrame);
    if (Control::MDCB_SET_LOADING_STATUS && core(static_cast<MDWebFrame*>(webFrame))->loader()->isLoadingMainFrame()) 
        Control::MDCB_SET_LOADING_STATUS (webView->viewWindow(), TRUE, percent);
}

void MDDefaultWebFrameLoadDelegate::didProgressFinished(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didProgressFinished %p %p", webView, webFrame);
    if (Control::MDCB_SET_LOADING_STATUS && core(static_cast<MDWebFrame*>(webFrame))->loader()->isLoadingMainFrame()) 
        Control::MDCB_SET_LOADING_STATUS (webView->viewWindow(), FALSE, 100);
    if(Control::MDCB_SET_STATUS)
        Control::MDCB_SET_STATUS(webView->viewWindow(), "Done");
}

void MDDefaultWebFrameLoadDelegate::didHandleOnloadEventsForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didHandleOnloadEventsForFrame %p %p", webView, webFrame);
}

BOOL MDDefaultWebFrameLoadDelegate::didClearWindowObject(IMDWebView* webView,IMDWebFrame* webFrame, JSContextRef cotext, 
        JSObjectRef windowObject)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didClearWindowObject %p %p", webView, webFrame);
    return TRUE;
}

void MDDefaultWebFrameLoadDelegate::didStartProvisionalLoadForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didStartProvisionalLoadForFrame %p %p", webView, webFrame);
}

void MDDefaultWebFrameLoadDelegate::didFinishLoadForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didFinishLoadForFrame %p %p", webView, webFrame);
    updateNavigations(webView);
}

void MDDefaultWebFrameLoadDelegate::didMainFrameDocumentReady(IMDWebView* webView)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didMainFrameDocumentReady %p", webView);
}
    
void MDDefaultWebFrameLoadDelegate::didChangeLocationWithinPageForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didChangeLocationWithinPageForFrame%p %p", webView, webFrame);
    char *url = static_cast<MDWebFrame*>(webFrame)->url();
    KURL standardURL = KURL(ParsedURLString, url);
    if (standardURL.hasPort() && isDefaultPortForProtocol(standardURL.port(), standardURL.protocol())) {
        standardURL.removePort();
    }
    if(Control::MDCB_SET_LOCATION)
        Control::MDCB_SET_LOCATION(webView->viewWindow(), standardURL.string().utf8().data());
    freeUtf8Char(url);
    updateNavigations(webView);
}

void MDDefaultWebFrameLoadDelegate::didCommitLoadForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didCommitLoadForFrame %p %p", webView, webFrame);
    char *url = static_cast<MDWebFrame*>(webFrame)->url();
    KURL standardURL = KURL(ParsedURLString, url);
    if (standardURL.hasPort() && isDefaultPortForProtocol(standardURL.port(), standardURL.protocol())) {
        standardURL.removePort();
    }
    if(Control::MDCB_SET_LOCATION)
        Control::MDCB_SET_LOCATION(webView->viewWindow(), standardURL.string().utf8().data());
    freeUtf8Char(url);
    
    if(Control::MDCB_SET_TITLE && (webView->mainWebFrame() == webFrame))
    {
        Control::MDCB_SET_TITLE(webView->viewWindow(),  "");
    }

    updateNavigations(webView);
}

void MDDefaultWebFrameLoadDelegate::didPerformFirstNavigation(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didPerformFirstNavigation %p %p", webView, webFrame);
    updateNavigations(webView);
}

void MDDefaultWebFrameLoadDelegate::didFinishDocumentLoad(IMDWebView* webView, IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebFrameLoadDelegate - didFinishDocumentLoad %p %p", webView, webFrame);
    updateNavigations(webView);
}
