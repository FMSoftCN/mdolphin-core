/*
 ** $Id$
 **
 ** IMDWebFrame.h: the interface file of MDWebFrame
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-08
 */

#ifndef IMDWebFrame_h
#define IMDWebFrame_h

#include "IUnknown.h"
#include <list>

#ifdef __cplusplus
class IMDWebView;

class IMDWebFrame : public IUnknown
{
public:
    virtual bool loadURL(const char * sUrl) =0;
    virtual void stopLoading(void) =0;
    virtual void reload(void)=0;
    virtual void loadData(const char *data, int len, const char *mimeType, 
            const char *encoding, const char *baseURL, const char* unreachableURL) = 0;


    virtual IMDWebView* webView() const =0;
    virtual char* url() const =0;

    virtual const char* name() const = 0;
    virtual char* renderTreeAsExternalRepresentation() = 0;
    virtual IMDWebFrame* findFrame(const char* name) = 0; 
    virtual IMDWebFrame* parentFrame() = 0; 
    virtual void layout() = 0;
    virtual char* responseMIMEType() = 0;
    virtual char* innerText() = 0;
    virtual bool childFrames(std::list<IMDWebFrame*>&) = 0;
};

#endif

#endif
