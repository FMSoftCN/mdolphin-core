/*
** $Id: DownloadResourceClient.h 894 2008-02-19 02:45:24Z whzeng $
**
** DownloadResourceClient.h: download reaource handle to disk.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2008-02-19
*/

#ifndef DownloadResourceClient_h
#define DownloadResourceClient_h

#include "ResourceHandleClient.h"
#if ENABLE(FILESYSTEM)
#include "FileSystemMg.h"
#endif
namespace WebCore {

class ResourceHandle;
class ResourceResponse;
class ResourceError;

class DownloadResourceClient : public ResourceHandleClient {
public:

    DownloadResourceClient(bool fromSave=false);
    DownloadResourceClient(String savedfile);
    ~DownloadResourceClient();
    
    void destroy(ResourceHandle *);
    void cancel(ResourceHandle *);

    virtual void didReceiveResponse(ResourceHandle*, const ResourceResponse&);
    virtual void didReceiveData(ResourceHandle*, const char*, int, int lengthReceived);
    virtual void didFinishLoading(ResourceHandle*);
    virtual void didFail(ResourceHandle*, const ResourceError&);

private:
#if ENABLE(FILESYSTEM)
    bool m_overExist;
    String m_filename;
    HFile m_fp ;
#endif

};

}

#endif /* DownloadResourceClient_h */
