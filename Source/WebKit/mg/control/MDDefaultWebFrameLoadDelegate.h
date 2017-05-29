/*
 ** $Id: MDDefaultWebWebFrameLoadDelegate.h 145 2010-07-12 03:15:02Z lijiangwei$
 **
 ** MDDefaultWebFrameLoadDelegate.h: Interface of default frame load delegate.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */

#ifndef MDDefaultWebFrameLoadDelegate_h
#define MDDefaultWebFrameLoadDelegate_h

#include "IMDWebFrameLoadDelegate.h"


#ifdef __cplusplus

class MDDefaultWebFrameLoadDelegate: public IMDWebFrameLoadDelegate
{
public:
    static MDDefaultWebFrameLoadDelegate* sharedInstance();
    static MDDefaultWebFrameLoadDelegate* createInstance();

    virtual void didReceiveResponse(IMDWebView*, IMDWebFrame*, unsigned long identifier, MDHResponse response);

    virtual void didReceiveTitle(IMDWebView*, IMDWebFrame*, const char* title);

    virtual void identifierForInitialRequest(IMDWebView*, IMDWebFrame*, MDHRequest, unsigned long identifier);

    virtual void didFailLoad(IMDWebView*, IMDWebFrame*, int code, const char* url);

    virtual void didFailProvisionalLoad(IMDWebView*, IMDWebFrame*, int code, const char* url);

    virtual void didProgressStarted(IMDWebView*, IMDWebFrame*);

    virtual void didProgressEstimateChanged(IMDWebView*, IMDWebFrame*, int percent);

    virtual void didProgressFinished(IMDWebView*, IMDWebFrame*);

    virtual void didHandleOnloadEventsForFrame(IMDWebView*, IMDWebFrame*);
    
    virtual BOOL didClearWindowObject(IMDWebView*,IMDWebFrame*, JSContextRef cotext, JSObjectRef windowObject);

    virtual void didStartProvisionalLoadForFrame(IMDWebView*, IMDWebFrame*);

    virtual void didFinishLoadForFrame(IMDWebView*, IMDWebFrame*);

    virtual void didMainFrameDocumentReady(IMDWebView*);

    virtual void didChangeLocationWithinPageForFrame(IMDWebView*, IMDWebFrame*);

    virtual void didCommitLoadForFrame(IMDWebView*, IMDWebFrame*);

    virtual void didPerformFirstNavigation(IMDWebView*, IMDWebFrame*);
    
    virtual void didFinishDocumentLoad(IMDWebView*, IMDWebFrame*);
protected:
    MDDefaultWebFrameLoadDelegate();
    ~MDDefaultWebFrameLoadDelegate();
};


#endif

#endif
