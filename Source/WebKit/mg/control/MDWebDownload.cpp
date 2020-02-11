/*
** $Id: MDWebDownload.cpp 1 2010-07-12 03:29:10Z lijiangwei $
**
** MDWebDownload.cpp: download object implement.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2010-07-12 
*/

#include "minigui.h"
#include "config.h"

#include "MDWebDownload.h"
#include "MDDefaultWebDownloadDelegate.h"
#include "ResourceHandle.h"
#include "ResourceHandleClient.h"
#include "ResourceHandleInternal.h"
#include "ResourceRequest.h"
#include "ResourceError.h"
#include "Logging.h"
#include "MDCommonFunc.h"

using namespace WebCore;

// definination -------------------------------------------------------------------------------------------

//DownloadClient
class DownloadClient : public ResourceHandleClient {
public:
    DownloadClient(MDWebDownload*);
    ~DownloadClient();

    virtual void didReceiveResponse(ResourceHandle*, const ResourceResponse&);
    virtual void didReceiveData(ResourceHandle*, const char*, int, int);
    virtual void didFinishLoading(ResourceHandle*, double);
    virtual void didFail(ResourceHandle*, const ResourceError&);

private:
    MDWebDownload* m_download;
};

//MDWebDownloadPrivate
class MDWebDownloadPrivate {
public:
    MDWebDownloadPrivate();
    ~MDWebDownloadPrivate();
    DownloadClient* downloadClient;
    const ResourceRequest* networkRequest;
    const ResourceResponse* networkResponse;
    RefPtr<ResourceHandle> resourceHandle;
    bool requestCreated;
};

// DownloadClient -----------------------------------------------------------------------------------------

DownloadClient::DownloadClient(MDWebDownload* download)
:m_download(download)
{
}

DownloadClient::~DownloadClient()
{
}

void DownloadClient::didReceiveResponse(ResourceHandle*, const ResourceResponse& response)
{
    m_download->didReceiveResponse(response);
}

void DownloadClient::didReceiveData(ResourceHandle* handle, const char* ptr, int length, int lenReceived)
{  
    m_download->didReceiveData(handle, ptr, length, lenReceived);
}

void DownloadClient::didFinishLoading(ResourceHandle* handle, double)
{
    m_download->didFinish(handle);
}

void DownloadClient::didFail(ResourceHandle* handle, const ResourceError& err)
{
    m_download->didFail(handle, err.errorCode());
}

// MDWebDownloadPrivata -----------------------------------------------------------------------------------

MDWebDownloadPrivate::MDWebDownloadPrivate()
    : downloadClient(0)
    , networkRequest(0)
    , networkResponse(0)
    , resourceHandle(0)
    , requestCreated(false)
{
}

MDWebDownloadPrivate::~MDWebDownloadPrivate()
{
    if (requestCreated) {
        delete networkRequest;
        networkRequest = 0;
    }
    //ResourceHandle
    if (resourceHandle) {
        resourceHandle->setClient(0);
        resourceHandle->cancel();

        resourceHandle.release();
    }
    //DownloadClient
    if (downloadClient)
        delete downloadClient;
}

// MDWebdownload ------------------------------------------------------------------------------------------

MDWebDownload::MDWebDownload()
    :d(new MDWebDownloadPrivate())
{
}

MDWebDownload::~MDWebDownload()
{
    if (d)
        d.clear();
}

MDWebDownload* MDWebDownload::createInstance(ResourceHandle* handle, const ResourceRequest& request, 
        const ResourceResponse& response, IMDWebDownloadDelegate* delegate)
{
    MDWebDownload* instance = new MDWebDownload();
    instance->init(handle, request, response, delegate);
    return instance;
}

MDWebDownload* MDWebDownload::createInstance(const KURL& url, IMDWebDownloadDelegate* delegate)
{
    MDWebDownload* instance = new MDWebDownload();
    instance->init(url, delegate);
    return instance;
}

void MDWebDownload::init(ResourceHandle* handle, const ResourceRequest& request, const ResourceResponse& response, 
        IMDWebDownloadDelegate* delegate)
{
    //init delegate
    m_delegate = delegate ? delegate : MDDefaultWebDownloadDelegate::sharedInstance();
    // init DWebDownloadPrivate
    d->networkRequest = &request;
    d->networkResponse = &response;
    d->resourceHandle = handle;
    d->downloadClient = new DownloadClient(this);

    // set ResourceHandleClient
    d->resourceHandle->setClient(d->downloadClient);
}

void MDWebDownload::init(const KURL& url, IMDWebDownloadDelegate* delegate)
{
    // init delegate 
    m_delegate = delegate ? delegate : MDDefaultWebDownloadDelegate::sharedInstance();
    // 
    ResourceRequest* request = new ResourceRequest(url);
    d->networkRequest = request;
    d->downloadClient = new DownloadClient(this); 
    d->requestCreated = true;
}

void MDWebDownload::start()
{
    LOG(Network, "MDWebDownload - Starting download (%p)", this);
    if (!d->resourceHandle)
        d->resourceHandle = ResourceHandle::create(0, *d->networkRequest, d->downloadClient, false, false);
    // manage MDWebDownload
    m_delegate->didBegin(this);
}

void MDWebDownload::cancel()
{
    LOG(Network, "MDWebDownload - Cancelling download (%p)", this);
    if (d->resourceHandle){
        d->resourceHandle->setClient(0);
        d->resourceHandle->cancel();
    }

    // release MDWebDownload object!!!!
    didFinish(d->resourceHandle.get());
}

BOOL MDWebDownload::didReceiveResponse(const ResourceResponse& response)
{
    LOG(Network, "MDWebDownload - didReceiveResponse (%p) %p", this, &response);
    char* filename = copyStringToUtf8Char(response.suggestedFilename());
    m_delegate->decideDestinationWithSuggestedFilename(this, filename);

    m_delegate->didReceiveResponse(this, (void *)(&response));
    
    freeUtf8Char(filename);
    return TRUE;
}

void MDWebDownload::setMDDownloadInfo(ResourceHandle* job, MDDownloadInfo* data, bool isFinish/* true */, 
        const char* ptr /* 0 */, int length /* 0 */)
{
    ResourceHandleInternal* d = job->getInternal();
    //data->id = (int)d->m_handle;
    data->id = (long)(d->m_handle);     // gengyue
    data->url = copyStringToUtf8Char(job->firstRequest().url().string());
    data->totalSize = d->m_response.expectedContentLength();
    if (!d->m_response.suggestedFilename().isEmpty())
        data->fileName = copyStringToUtf8Char(d->m_response.suggestedFilename());
    else {
        char* pos = strrchr(data->url, '/');
    
        String fileName;
        if (strlen(pos+1) == 0)
            fileName = String::fromUTF8(data->url);
        else
            fileName = String::fromUTF8(pos+1);
        data->fileName = copyStringToUtf8Char(fileName);
    }
    data->mimeType = copyStringToUtf8Char(d->m_response.mimeType());
    data->isFinish = isFinish ? TRUE: FALSE;
    data->data = ptr;
    data->dataSize = length; 
}

void MDWebDownload::freeMDDownloadInfo(MDDownloadInfo* data)
{
    if (data->url)
        freeUtf8Char(data->url);

    if (data->fileName)
        freeUtf8Char(data->fileName);

    if (data->mimeType)
        freeUtf8Char(data->mimeType);
}

BOOL MDWebDownload::didReceiveData(ResourceHandle* job, const char* ptr, int length, int lenReceived)
{
    LOG(Network, "MDWebDownload - didReceiveData (%p) job %p ptr %p len %d  received %d", this, job, ptr, length, lenReceived);
    // set MDDownloadInfo
    MDDownloadInfo data;
    setMDDownloadInfo(job, &data, false, ptr, length);
    // 
    m_delegate->didReceiveData(this, &data);
    freeMDDownloadInfo(&data);

    return TRUE;
}

BOOL MDWebDownload::didFail(ResourceHandle* job, int err)
{
    LOG(Network, "MDWebDownload - didFail (%p) %d", this, err);
    // set MDDownloadInfo
    MDDownloadInfo data;
    setMDDownloadInfo(job, &data);
    m_delegate->didFailWithError(this, &data, err);

    freeMDDownloadInfo(&data);
    return TRUE;
}

BOOL MDWebDownload::didFinish(ResourceHandle* job)
{
    LOG(Network, "MDWebDownload - didFinish (%p) job %p", this, job);
    // set MDDownloadInfo
    MDDownloadInfo data;
    setMDDownloadInfo(job, &data);
    m_delegate->didFinish(this, &data);

    freeMDDownloadInfo(&data);
    return TRUE;
}
