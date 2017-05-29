/*
 ** $Id: IMDWebFrameLoadDelegate.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** IMDWebFrameLoadDelegate.h:  Interface of web frame load delegate.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */

#ifndef IMDWebFrameLoadDelegate_h
#define IMDWebFrameLoadDelegate_h

#include "IUnknown.h"
#include "IMDWebView.h"
#include "IMDWebFrame.h"
#include "JSContextRef.h"
#include "JSObjectRef.h"
#include "MDResourceRequest.h"
#include "MDResourceResponse.h"

#ifdef __cplusplus

class IMDWebFrameLoadDelegate : public IUnknown
{
public:
    /**
     * didReceiveResponse
     */
    virtual void didReceiveResponse(IMDWebView*, IMDWebFrame*, unsigned long identifier, MDHResponse response) = 0;

    /**
     * didReceiveTitle
     */
    virtual void didReceiveTitle(IMDWebView*, IMDWebFrame*, const char* title) = 0;

    /* 
     * identifierForInitialRequest
     */
    virtual void identifierForInitialRequest(IMDWebView*, IMDWebFrame*, MDHRequest, unsigned long identifier) = 0;

    /* 
     * didFailLoad
     */
    virtual void didFailLoad(IMDWebView*, IMDWebFrame*, int code, const char* url) = 0;

    /*
     * didFailProvisionalLoad
     */
    virtual void didFailProvisionalLoad(IMDWebView*, IMDWebFrame*, int code, const char* url) = 0;

    /**
     * didProgressStarted
     */
    virtual void didProgressStarted(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didProgressEstimateChanged
     */
    virtual void didProgressEstimateChanged(IMDWebView*, IMDWebFrame*, int percent) = 0;

    /**
     * didProgressFinished
     */
    virtual void didProgressFinished(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didHandleOnloadEventsForFrame
     */
    virtual void didHandleOnloadEventsForFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didClearWindowObject
     */
    virtual BOOL didClearWindowObject(IMDWebView*,IMDWebFrame*, JSContextRef cotext, JSObjectRef windowObject) = 0;

    /**
     * didStartProvisionalLoadForFrame
     */
    virtual void didStartProvisionalLoadForFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didFinishLoadForFrame
     */
    virtual void didFinishLoadForFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didMainFrameDocumentReady
     */
    virtual void didMainFrameDocumentReady(IMDWebView*) = 0;

    /**
     * didChangeLocationWithinPageForFrame
     */
    virtual void didChangeLocationWithinPageForFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didCommitLoadForFrame
     */
    virtual void didCommitLoadForFrame(IMDWebView*, IMDWebFrame*) = 0;

    /**
     * didPerformFirstNavigation
     */
    virtual void didPerformFirstNavigation(IMDWebView*, IMDWebFrame*) = 0;
    
    /**
     * didFinishDocumentLoad
     */
    virtual void didFinishDocumentLoad(IMDWebView*, IMDWebFrame*) = 0;
};

#endif

#endif
