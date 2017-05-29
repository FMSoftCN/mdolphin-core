/*
** $Id: MDDefaultWebPolicyDelegate.cpp 1 2010-07-13 11:05:10Z lijiangwei $
**
** MDDefaultWebPolicyDelegate.cpp: web policy implement.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2010-07-13 
*/
#include "minigui.h"
#include "mdolphin.h"

#include "MDDefaultWebPolicyDelegate.h"
#include "MDWebFrame.h"

#include "Logging.h"
#include "Frame.h"
#include "DocumentLoader.h"
#include "ResourceResponse.h"
#include "HTTPParsers.h"
#include "FrameLoaderClient.h"

#if ENABLE(SCHEMEEXTENSION)
#include "SchemeExtension.h"
#endif

using namespace WebCore;
static const char* downloadMIMETypeTab[] = {
	"application/octet-stream",
	"application/x-gzip"
};

static bool downloadForMIMEType(const char* type)
{
    unsigned int i;
	for(i = 0; i < sizeof(downloadMIMETypeTab)/sizeof(char*); ++i) {
		if(strcmp(downloadMIMETypeTab[i], type) == 0)
			return true;
	}
	return false;
}

MDDefaultWebPolicyDelegate::MDDefaultWebPolicyDelegate()
{
}

MDDefaultWebPolicyDelegate::~MDDefaultWebPolicyDelegate()
{
}

MDDefaultWebPolicyDelegate* MDDefaultWebPolicyDelegate::sharedInstance()
{
    static MDDefaultWebPolicyDelegate* shared;
    if (!shared)
        shared = MDDefaultWebPolicyDelegate::createInstance();
    return shared;
}

MDDefaultWebPolicyDelegate* MDDefaultWebPolicyDelegate::createInstance()
{
    MDDefaultWebPolicyDelegate* instance = new MDDefaultWebPolicyDelegate();
    return instance;
}
MDEWebPolicyDecisionResult MDDefaultWebPolicyDelegate::decidePolicyForNavigationAction(const IMDWebView*, MDENavigationType, 
        MDHRequest req, const IMDWebFrame*)
{

#if ENABLE(SCHEMEEXTENSION)
	const KURL& kurl = ((ResourceRequest*)req)->url();
	if (scheduleScheme(kurl)){
		return MD_WebPolicyDecisionIgnore;
	}
#endif

	LOG(Loading, "MDDefaultWebPolicyDelegate - decidePolicyForNavigationAction");
    return MD_WebPolicyDecisionUse;
}

MDEWebPolicyDecisionResult MDDefaultWebPolicyDelegate::decidePolicyForNewWindowAction(const IMDWebView*, MDENavigationType, MDHRequest, 
        const IMDWebFrame*, const char *frameName)
{
    LOG(Loading, "MDDefaultWebPolicyDelegate - decidePolicyForNewWindowAction");
    return MD_WebPolicyDecisionUse;
}

MDEWebPolicyDecisionResult MDDefaultWebPolicyDelegate::decidePolicyForMIMEType(const IMDWebView* webview, const char* mimeType, 
        MDHRequest, const IMDWebFrame* webFrame)
{
    LOG(Loading, "MDDefaultWebPolicyDelegate - dispatchDecidePolicyForMIMEType");
    Frame* frame = core((MDWebFrame*)webFrame);
    const ResourceResponse& response = frame->loader()->activeDocumentLoader()->response();

    if (WebCore::contentDispositionType(response.httpHeaderField("Content-Disposition")) 
            == WebCore::ContentDispositionAttachment) {
        return MD_WebPolicyDecisionDownload;
    }

    if (const_cast<IMDWebView*>(webview)->canShowMIMEType(mimeType)) {
        return MD_WebPolicyDecisionUse;
    }
	else if(downloadForMIMEType(mimeType))
		return MD_WebPolicyDecisionDownload;
	else {
        // We use download policy for ftp download.
        // Because there is no Content-Disposition for ftp download
        if (response.url().protocolIs("ftp"))
            return MD_WebPolicyDecisionDownload;
        else
            return MD_WebPolicyDecisionIgnore;
    }   
}
