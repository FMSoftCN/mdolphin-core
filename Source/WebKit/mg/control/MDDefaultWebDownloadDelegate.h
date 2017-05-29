/*
 ** $Id: MDDefaultWebDownloadDelegate.h 145 2010-07-12 03:15:02Z lijiangwei$
 **
 ** MDDefaultWebDownloadDelegate.h: Interface of default download delegate.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-12
 */

#ifndef MDDefaultWebDownloadDelegate_H
#define MDDefaultWebDownloadDelegate_H

#include "IMDWebDownloadDelegate.h"
#include <wtf/HashSet.h>

#ifdef __cplusplus

class MDDefaultWebDownloadDelegate : public IMDWebDownloadDelegate
{
public:
    static MDDefaultWebDownloadDelegate* sharedInstance();
    static MDDefaultWebDownloadDelegate* createInstance();

    void registerDownload(IMDWebDownload*);
    void unregisterDownload(IMDWebDownload*);

    virtual BOOL decideDestinationWithSuggestedFilename(IMDWebDownload* download, const char* filename);
    virtual BOOL didBegin(IMDWebDownload* download); 
    virtual BOOL didReceiveResponse(IMDWebDownload* download, MDHResponse resp);
    virtual BOOL didReceiveData(IMDWebDownload* download, const MDDownloadInfo* data);
    virtual BOOL didFinish(IMDWebDownload* download, const MDDownloadInfo* data);
    virtual BOOL didFailWithError(IMDWebDownload* download, const MDDownloadInfo* data, const int error);

protected:
    MDDefaultWebDownloadDelegate();
    ~MDDefaultWebDownloadDelegate();

    HashSet<IMDWebDownload*> m_downloads;
};

#endif

#endif
