/*
 ** $Id: MDDefaultWebDownloadDelegate.cpp 1 2010-07-12 03:29:10Z lijiangwei $ 
 **
 ** MDDefaultWebDownloadDelegate.cpp: 
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

#include "mdolphin.h"
#include "MDDefaultWebDownloadDelegate.h"

#include "Logging.h"
#include "IMDWebDownload.h"
#include "MDolphinDelegates.h"

using namespace WebCore;

MDDefaultWebDownloadDelegate::MDDefaultWebDownloadDelegate()
{
}

MDDefaultWebDownloadDelegate::~MDDefaultWebDownloadDelegate()
{
    HashSet<IMDWebDownload*>::iterator i = m_downloads.begin();
    for (;i != m_downloads.end(); ++i)
        (*i)->release();
}

MDDefaultWebDownloadDelegate* MDDefaultWebDownloadDelegate::sharedInstance()
{
    static MDDefaultWebDownloadDelegate* shared;
    if (!shared)
        shared = MDDefaultWebDownloadDelegate::createInstance();
    return shared;
}

MDDefaultWebDownloadDelegate* MDDefaultWebDownloadDelegate::createInstance()
{
    MDDefaultWebDownloadDelegate* instance = new MDDefaultWebDownloadDelegate();
    return instance;
}

BOOL MDDefaultWebDownloadDelegate::decideDestinationWithSuggestedFilename(IMDWebDownload* download, const char* filename)
{
    LOG(Network, "MDDefaultWebDownloadDelegate %p - decideDestinationWithSuggestedFilename %s", download, filename);

    return TRUE; 
}

BOOL MDDefaultWebDownloadDelegate::didBegin(IMDWebDownload* download)
{
    LOG(Network, "MDDefaultWebDownloadDelegate %p - didBegin", download);
    registerDownload(download);  
    
    return TRUE;
}

BOOL MDDefaultWebDownloadDelegate::didReceiveResponse(IMDWebDownload* download, MDHResponse resp)
{
    LOG(Network, "MDDefaultWebDownloadDelegate %p - didReceiveResponse %p", download, resp);
    
    return TRUE;
}

BOOL MDDefaultWebDownloadDelegate::didReceiveData(IMDWebDownload* download, const MDDownloadInfo* data)
{
    LOG(Network, "DefaultDownloadDelegate %p - didReceiveData %p", download, data);
    if (Control::MDCB_SAVE_FILE_DATA) {
        BOOL result = Control::MDCB_SAVE_FILE_DATA(data->id, data->url, data->data, data->dataSize, data->totalSize, 
                data->fileName , data->mimeType,  data->isFinish);
        if (!result)
            download->cancel();
        return TRUE;
    }
    return TRUE;
}

BOOL MDDefaultWebDownloadDelegate::didFinish(IMDWebDownload* download, const MDDownloadInfo* data)
{
    LOG(Network, "DefaultDownloadDelegate %p - didFinish %p", download, data);
    unregisterDownload(download);    
    if (Control::MDCB_SAVE_FILE_DATA) {
        Control::MDCB_SAVE_FILE_DATA(data->id, data->url, data->data, data->dataSize, data->totalSize, 
                data->fileName , data->mimeType,  data->isFinish);
        return TRUE;
    }
    return TRUE;
}

BOOL MDDefaultWebDownloadDelegate::didFailWithError(IMDWebDownload* download, const MDDownloadInfo* data, const int error)
{
    LOG(Network, "MDDefaultWebDownloadDelegate %p - didFailWithError %p %d", download, data, error);
    didFinish(download, data);
    return TRUE;
}

// manager IMDWebDownload
void MDDefaultWebDownloadDelegate::registerDownload(IMDWebDownload* download)
{
    if (m_downloads.contains(download))
        return;
    download->addRef();
    m_downloads.add(download);
}

void MDDefaultWebDownloadDelegate::unregisterDownload(IMDWebDownload* download)
{
    if (m_downloads.contains(download)) {
        download->release();
        m_downloads.remove(download);
    }
}
