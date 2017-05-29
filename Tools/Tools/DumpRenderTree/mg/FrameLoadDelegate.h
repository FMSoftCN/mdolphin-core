
#ifndef FrameLoadDelegate_h
#define FrameLoadDelegate_h

#include "IMDWebFrameLoadDelegate.h"

class GCController;
class FrameLoadDelegate : public IMDWebFrameLoadDelegate
{
public:
    FrameLoadDelegate();
    virtual void didReceiveTitle(IMDWebView*, IMDWebFrame*, const char* title);
    virtual BOOL didClearWindowObject(IMDWebView*, IMDWebFrame*, JSContextRef cotext, JSObjectRef windowObject);
    virtual void didFinishDocumentLoad(IMDWebView*, IMDWebFrame*);
    virtual void didStartProvisionalLoadForFrame(IMDWebView*, IMDWebFrame*);
    virtual void didCommitLoadForFrame(IMDWebView*, IMDWebFrame*);
    virtual void didHandleOnloadEventsForFrame(IMDWebView*, IMDWebFrame*);
    virtual void didFinishLoadForFrame(IMDWebView*, IMDWebFrame*);
    virtual void didFailLoad(IMDWebView*, IMDWebFrame*, int code, const char* url);

    virtual void didReceiveResponse(IMDWebView*, IMDWebFrame*, unsigned long identifier, MDHResponse response) {} ;
    virtual void identifierForInitialRequest(IMDWebView*, IMDWebFrame*, MDHRequest, unsigned long identifier) {} ;
    virtual void didFailProvisionalLoad(IMDWebView*, IMDWebFrame*, int code, const char* url) {} ;
    virtual void didProgressStarted(IMDWebView*, IMDWebFrame*) {} ;
    virtual void didProgressEstimateChanged(IMDWebView*, IMDWebFrame*, int percent) {} ;
    virtual void didProgressFinished(IMDWebView*, IMDWebFrame*) {} ;
    virtual void didMainFrameDocumentReady(IMDWebView*) {} ;
    virtual void didChangeLocationWithinPageForFrame(IMDWebView*, IMDWebFrame*) {} ;
    virtual void didPerformFirstNavigation(IMDWebView*, IMDWebFrame*) {} ;

private:
    GCController* m_gcController;
};
#endif
