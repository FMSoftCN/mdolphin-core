/*
 ** $Id: IMDWebDownload.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** IMDWebDownload.h:  Interface of web download.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */
#ifndef IMDWebDownload_h
#define IMDWebDownload_h

#include "IUnknown.h"

#ifdef __cplusplus

class IMDWebDownload : public IUnknown
{
public:
    /**
     * start
     */
    virtual void start() = 0;
    /**
     * cancel
     */
    virtual void cancel() = 0;
};

#endif

#endif
