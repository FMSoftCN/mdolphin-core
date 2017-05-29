/*
 ** $Id: MDWebDownload.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** MDWebDownload.h:  Interface of web download.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */
#ifndef MDWebDownload_h
#define MDWebDownload_h

#include "IMDWebDownload.h"
#include "IMDWebDownloadDelegate.h"

#include <wtf/RefPtr.h>
#include <wtf/OwnPtr.h>

#ifdef __cplusplus

namespace WebCore {
    class ResourceHandleClient;
    class ResourceHandle;
    class ResourceResponse;
    class ResourceRequest;
    class KURL;
}

class MDWebDownloadPrivate;
class MDWebDownload : public IMDWebDownload
{
public:
    static MDWebDownload* createInstance(WebCore::ResourceHandle* handle, const WebCore::ResourceRequest& request, 
        const WebCore::ResourceResponse& response, IMDWebDownloadDelegate* delegate);

    static MDWebDownload* createInstance(const WebCore::KURL& url, IMDWebDownloadDelegate* delegate);

protected:
    MDWebDownload();
    ~MDWebDownload();

public:
    virtual void start();
    virtual void cancel();

    BOOL didReceiveResponse(const WebCore::ResourceResponse& response);
    BOOL didReceiveData(WebCore::ResourceHandle* job, const char* ptr, int length, int lenReceived);
    BOOL didFail(WebCore::ResourceHandle* job, int err);
    BOOL didFinish(WebCore::ResourceHandle* job);

private:
    void init(WebCore::ResourceHandle* handle, const WebCore::ResourceRequest& request, const WebCore::ResourceResponse& response, 
            IMDWebDownloadDelegate* delegate);
    void init(const WebCore::KURL& url, IMDWebDownloadDelegate* delegate);

    void setMDDownloadInfo(WebCore::ResourceHandle* job, MDDownloadInfo* data, bool isFinish = true, const char* ptr = 0, int length = 0);

    void freeMDDownloadInfo(MDDownloadInfo* data);
    OwnPtr<MDWebDownloadPrivate> d;
    IMDWebDownloadDelegate* m_delegate;
};

#endif

#endif
