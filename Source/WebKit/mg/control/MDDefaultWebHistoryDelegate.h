/*
 ** $Id$
 **
 ** MDDefaultWebHistoryDelegate.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */
#ifndef MDDefaultHistoryDelegate_H
#define MDDefaultHistoryDelegate_H
#include "IMDWebHistoryDelegate.h"
#ifdef __cplusplus
class MDDefaultWebHistoryDelegate : public IMDWebHistoryDelegate {
	public: 
        static MDDefaultWebHistoryDelegate *createInstance();
		virtual ~MDDefaultWebHistoryDelegate() ; 
		virtual int didNavigateWithNavigationData(IMDWebView* webView, IMDWebNavigationData* navigationData, IMDWebFrame* webFrame);
		virtual int didPerformClientRedirectFromURL(IMDWebView* webView, const char* sourceURL, const char* destinationURL, IMDWebFrame* webFrame);
		virtual int didPerformServerRedirectFromURL( IMDWebView* webView, const char* sourceURL, const char* destinationURL, IMDWebFrame* webFrame);
		virtual int updateHistoryTitle( IMDWebView* webView, const char* title, const char* url);
		virtual int populateVisitedLinksForWebView( IMDWebView* webView);
};
#endif
#endif
