/*
** $Id: FtpProtocolHandler.cpp 546 2007-11-16 09:26:33Z whzeng $
**
** FtpProtocolHandler.cpp: FTP protocol source file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-11-16
*/

#include "config.h"

#if ENABLE(FTPDIR)
#include "ResourceHandle.h"
#include "ResourceHandleClient.h"
#include "ResourceHandleInternal.h"
#include "MIMETypeMg.h"
#include "Logging.h"

namespace WebCore {

size_t ftpWriteCallback(char* ptr, size_t size, size_t nmemb, void *data)
{
    LOG(FTP, "FTP - ftpWriteCallback ptr %p size %d nmemb %d data %p", ptr, size, nmemb, data);
    size_t length = size * nmemb;
    ResourceHandle* job = static_cast<ResourceHandle*>(data);
    ResourceHandleInternal* d = job->getInternal();
    if (d->m_cancelled)
        return 0;

    if (! d->m_response.mimeTypeChecked()) {
        MIMEType guestMIMEType = MIMEType::guestFromResource(d->m_firstRequest.url(), ptr, length);
        d->m_response.setMimeType(guestMIMEType.mimeType());

        d->m_response.setMIMETypeChecked(true);
    }
    
    CURL* handle = d->m_handle; 
    const char* hdr;
    CURLcode err = curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &hdr);
    ASSERT(CURLE_OK == err);
    d->m_response.setURL(KURL(ParsedURLString, hdr));
    if (d->client()) {
        LOG(FTP, "FTP - ftpWriteCallback : client->didReceiveResponse");
        d->client()->didReceiveResponse(job, d->m_response);
    }
    d->m_response.setResponseFired(true);
                                  
    if (d->client()) {
        LOG(FTP, "FTP - ftpWriteCallback : client->didReceiveData");
        d->client()->didReceiveData(job, static_cast<char*>(ptr), length, 0);
    }

    return length;
}

static size_t ftpHeaderCallback(char* ptr, size_t size, size_t nmemb, void* data)
{
    LOG(FTP, "FTP - ftpHeaderCallback ptr %p size %d nmemb %d data %p", ptr, size, nmemb, data);
    int i=0;
    int len = size * nmemb;

    ResourceHandle* job = static_cast<ResourceHandle*>(data);
    ResourceHandleInternal* d = job->getInternal();
    while (i < len) {
        if (ptr[i] == ' ')
            break;
        ++i;
    }

    if (i == len)
        return len;

    if (d->m_cancelled)
        return 0;
    
    int resCode = atol(ptr);
    switch (resCode) {
    case 213:
        d->m_response.setExpectedContentLength(atoll(ptr+i+1));
        break;
    case 150:
        if (d->m_firstRequest.url().path() == "" || d->m_firstRequest.url().path().endsWith("/")) {
            d->m_response.setMimeType("application/x-ftp-directory");
            d->m_response.setMIMETypeChecked(true);
        }
        break;
    default:
        d->m_response.setHTTPStatusCode(resCode);
        break;
    }
    return len;
}

void initFtpJob(ResourceHandle* job)
{
    ResourceHandleInternal* d = job->getInternal();

    curl_easy_setopt(d->m_handle, CURLOPT_WRITEFUNCTION, ftpWriteCallback);
    curl_easy_setopt(d->m_handle, CURLOPT_HEADERFUNCTION, ftpHeaderCallback);
}

} /* namespace WebCore */

#endif
