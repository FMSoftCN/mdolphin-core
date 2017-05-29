/*
 ** $Id: MDDefaultWebPolicyDelegate.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** MDDefaultWebPolicyDelegate.h:  Interface of web download.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-13
 */
#ifndef MDDefaultWebPolicyDelegate_h
#define MDDefaultWebPolicyDelegate_h

#include "IMDWebPolicyDelegate.h"

class MDDefaultWebPolicyDelegate: public IMDWebPolicyDelegate
{
public:
    static MDDefaultWebPolicyDelegate* sharedInstance();
    static MDDefaultWebPolicyDelegate* createInstance();

    virtual MDEWebPolicyDecisionResult decidePolicyForNavigationAction(const IMDWebView*, MDENavigationType, MDHRequest, const IMDWebFrame*);
    virtual MDEWebPolicyDecisionResult decidePolicyForNewWindowAction(const IMDWebView*, MDENavigationType, MDHRequest, 
            const IMDWebFrame*, const char *frameName);
    virtual MDEWebPolicyDecisionResult decidePolicyForMIMEType(const IMDWebView*, const char* MIME, MDHRequest, const IMDWebFrame*);
protected:
    MDDefaultWebPolicyDelegate();
    ~MDDefaultWebPolicyDelegate();
};

#endif

