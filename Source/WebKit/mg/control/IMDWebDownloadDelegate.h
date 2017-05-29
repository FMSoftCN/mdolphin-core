/*
 ** $Id: IMDWebDownloadDelegate.h 145 2010-07-12 01:56:02Z lijiangwei$
 **
 ** IMDWebDownloadDelegate.h:  Interface of web download delegate.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */
#ifndef IMDWebDownloadDelegate_h
#define IMDWebDownloadDelegate_h

#include "IUnknown.h"
#include "MDResourceResponse.h"

//define the common struct for this delegate
#ifdef __cplusplus
extern "C" {
#endif

/* MDDownloadInfo struct */
typedef struct _MDDownloadInfo {
        int id;
        char* url;
        const void* data;
        size_t dataSize;
        unsigned long totalSize;
        char* fileName;
        char* mimeType;
        BOOL isFinish;
} MDDownloadInfo;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class IMDWebDownload;

class IMDWebDownloadDelegate : public IUnknown
{
public:
    /**
     * decideDestinationWithSuggestedFilename: decide download file name
     */
    virtual BOOL decideDestinationWithSuggestedFilename(IMDWebDownload* download, const char* filename) = 0;

    /** 
     * didBegin
     */
    virtual BOOL didBegin(IMDWebDownload* download) = 0;

    /**
     * didReceiveResponse
     */ 
    virtual BOOL didReceiveResponse(IMDWebDownload* download, MDHResponse resp) = 0;

    /**
     * didReceiveData
     */
    virtual BOOL didReceiveData(IMDWebDownload* download, const MDDownloadInfo* data) = 0;

    /**
     * didFinish
     */
    virtual BOOL didFinish(IMDWebDownload* download, const MDDownloadInfo* data) = 0;

    /**
     * didFailWithError
     */
    virtual BOOL didFailWithError(IMDWebDownload* download, const MDDownloadInfo* data, const int error) = 0;
};

#endif

#endif
