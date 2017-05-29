/*
 ** $Id$
 **
 ** IWebHistoryDelegate.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */

#ifndef IMDWebHistoryDelegate_H
#define IMDWebHistoryDelegate_H
#include "IUnknown.h"

#ifdef __cplusplus
#include "IMDWebView.h"
#include "IMDWebFrame.h"
class IMDWebNavigationData;
class IMDWebHistoryDelegate :public IUnknown {
	public: 
		virtual ~IMDWebHistoryDelegate(){}   
		virtual int didNavigateWithNavigationData(IMDWebView* webView, IMDWebNavigationData* navigationData, IMDWebFrame* webFrame)=0;
		virtual int didPerformClientRedirectFromURL(IMDWebView* webView, const char* sourceURL, const char* destinationURL,
				IMDWebFrame* webFrame)=0;
		virtual int didPerformServerRedirectFromURL( IMDWebView* webView, const char* sourceURL, const char* destinationURL, IMDWebFrame* webFrame)=0;
		virtual int updateHistoryTitle( IMDWebView* webView, const char* title, const char* url)=0;
		virtual int populateVisitedLinksForWebView( IMDWebView* webView)=0;
};

typedef void * MDHURLRequest;
typedef void * MDHURLResponse;
class IMDWebNavigationData : public IUnknown {
	public:
		virtual ~IMDWebNavigationData(){}
		virtual const char* url()=0;
		virtual const char* title()=0;
		virtual MDHURLRequest originalRequest()=0;
		virtual MDHURLResponse response()=0;
		virtual BOOL hasSubstituteData()=0;
		virtual const char* clientRedirectSource()=0;
};
#endif

#endif
