/*
 ** $Id: IMDWebPolicyDelegate.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** IMDWebPolicyDelegate.h:  Interface of web policy.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */
#ifndef IMDWebPolicyDelegate_h
#define IMDWebPolicyDelegate_h

#include "IUnknown.h"
#include "IMDWebView.h"
#include "IMDWebFrame.h"
#include "MDResourceRequest.h"

//define the common struct of this delegate
#ifdef __cplusplus
extern "C" {
#endif

/* typedef navigation type */
typedef enum {
    MD_NavigationTypeLinkClicked,
    MD_NavigationTypeFormSubmitted,
    MD_NavigationTypeBackForward,
    MD_NavigationTypeReload,
    MD_NavigationTypeFormResubmitted,
    MD_NavigationTypeOther
}MDENavigationType;

typedef enum {
    MD_WebPolicyDecisionUse,
    MD_WebPolicyDecisionDownload,
    MD_WebPolicyDecisionIgnore
}MDEWebPolicyDecisionResult;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class IMDWebPolicyDelegate : public IUnknown
{
public:
    /**
     * decidePolicyForNavigationAction
     */
    virtual MDEWebPolicyDecisionResult decidePolicyForNavigationAction(const IMDWebView*, MDENavigationType, MDHRequest, const IMDWebFrame*) = 0;
    /**
     * decidePolicyForNewWindowAction
     */
    virtual MDEWebPolicyDecisionResult decidePolicyForNewWindowAction(const IMDWebView*, MDENavigationType, MDHRequest, 
            const IMDWebFrame*, const char *frameName) = 0;

    /**
     * dispatchDecidePolicyForMIMEType
     */
    virtual MDEWebPolicyDecisionResult decidePolicyForMIMEType(const IMDWebView*, const char* MIME, MDHRequest, const IMDWebFrame*) = 0;

};

#endif


#endif
