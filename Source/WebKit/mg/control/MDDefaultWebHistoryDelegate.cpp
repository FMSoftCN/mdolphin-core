/*
 ** $Id$
 **
 ** MDDefaultWebhistoryDelegate.cpp:
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */

#include "minigui.h"
#include "mdolphin.h"

#include "MDDefaultWebHistoryDelegate.h"

MDDefaultWebHistoryDelegate * MDDefaultWebHistoryDelegate::createInstance()
{
    return new MDDefaultWebHistoryDelegate();
}

MDDefaultWebHistoryDelegate::~MDDefaultWebHistoryDelegate()
{
}   

int MDDefaultWebHistoryDelegate::didNavigateWithNavigationData(IMDWebView* webView, IMDWebNavigationData* navigationData, IMDWebFrame* webFrame)
{
    return -1;
}

int MDDefaultWebHistoryDelegate::didPerformClientRedirectFromURL(IMDWebView* webView, const char* sourceURL, const char* destinationURL, IMDWebFrame* webFrame)
{
    return -1;
}

int MDDefaultWebHistoryDelegate::didPerformServerRedirectFromURL( IMDWebView* webView, const char* sourceURL, const char* destinationURL, IMDWebFrame* webFrame)
{
    return -1;
}

int MDDefaultWebHistoryDelegate::updateHistoryTitle( IMDWebView* webView, const char* title, const char* url)
{
    return -1;
}

int MDDefaultWebHistoryDelegate::populateVisitedLinksForWebView( IMDWebView* webView)
{
    return -1;
}

