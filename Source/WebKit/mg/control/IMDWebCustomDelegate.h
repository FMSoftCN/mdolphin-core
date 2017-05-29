/*
 ** $Id$
 **
 ** IMDWebCustomDelegate.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-13 
 */

#ifndef IMDWebCustomDelegate_H
#define IMDWebCustomDelegate_H

#include "IUnknown.h"

//define the common struct of this delegate
#ifdef __cplusplus
class IMDWebView;

class IMDWebCustomDelegate : public IUnknown {
public:
    virtual const char* userArget(IMDWebView*, const char* url)=0;
    virtual void beforeDrawViewport(IMDWebView*, HDC windowdDC, HDC memDC, const RECT* prcClient)=0;
    virtual void afterDrawViewport(IMDWebView*, HDC windowDC, HDC memDC, const RECT *prcClient)=0;
    virtual void linkURL(const char *url)=0;
};

#endif

#endif
