/*
 ** $Id: MDWebInspector.h 145 2011-05-18 01:56:02Z lijiangwei$
 **
 ** MDWebDownload.h:  Interface of web inspect.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2011-05-18
 */
#ifndef MDWebInspector_h
#define MDWebInspector_h

#include "IMDWebInspector.h"

#ifdef __cplusplus

class IMDWebView;

namespace WebCore
{
	class InspectorFrontendClientMg;
}

class MDWebInspector: public IMDWebInspector
{
public:
    static MDWebInspector* createInstance();

protected:
    MDWebInspector();
    ~MDWebInspector();

public:
    void show() {}
	void close();
    void setInspectorWebView(IMDWebView*);
	void setInspectorFrontendClient(WebCore::InspectorFrontendClientMg*);
	WebCore::InspectorFrontendClientMg* getInspectorFrontendClient() { return m_inspectorFrontendClient; }
private:
    IMDWebView* m_inspectorWebView;
	WebCore::InspectorFrontendClientMg* m_inspectorFrontendClient;
};

#endif

#endif

