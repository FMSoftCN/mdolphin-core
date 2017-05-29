/*
 ** $Id$
 **
 ** IMDWebInspector.h: the interface file of MDWebInspector
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei
 **  
 ** Create date: 2011-05-18
 */

#ifndef IMDWebInspector_h
#define IMDWebInspector_h

#include "IUnknown.h"
#include <list>

#ifdef __cplusplus
class IMDWebView;

class IMDWebInspector: public IUnknown
{
public:
    virtual void show() = 0;
    virtual void close() = 0;
    virtual void setInspectorWebView(IMDWebView*) = 0;
};

#endif

#endif

