/*
** $Id: DownloadResourceClient.cpp 894 2008-02-19 02:45:24Z whzeng $
**
** DownloadResourceClient.cpp: download reaource handle to disk.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2008-02-19
*/
#include "config.h"
#include "minigui.h"

#include "MDolphinDelegates.h"
#include "ResourceHandle.h"
#include "ResourceHandleInternal.h"
#include "ResourceResponse.h"
#include "ResourceError.h"
#include "ResourceHandleClient.h"
#include "DownloadResourceClient.h"
#include "PlatformString.h"
#include <wtf/text/CString.h>
#include <wtf/Forward.h>

namespace WebCore {

DownloadResourceClient::DownloadResourceClient(bool from)
{
#if ENABLE(FILESYSTEM)
    m_overExist = false;
    m_fp = NULL;
#endif
}

DownloadResourceClient::DownloadResourceClient(String localfile)
{
    
#if ENABLE(FILESYSTEM)
    m_filename = localfile;
    m_fp = NULL;
    m_overExist = false;
#endif
}

DownloadResourceClient::~DownloadResourceClient()
{}

void DownloadResourceClient::cancel(ResourceHandle *job)
{
    //ResourceHandleInternal* d = job->getInternal();
    //d->m_mightDownloadFromHandle = false;
    job->cancel();
    destroy(job);
}

void DownloadResourceClient::destroy(ResourceHandle *job)
{
	job->setClient(0);
	delete this;
}

void  DownloadResourceClient::didReceiveResponse(ResourceHandle* job, const ResourceResponse&)
{
	
}

void  DownloadResourceClient::didReceiveData(ResourceHandle* job, const char* ptr, int length, int lengthReceived)
{
    bool result = false;
    int pos;
    char file[256]; 
    int ret;

    ResourceHandleInternal* d = job->getInternal();
    //const String& suggestedFilename() const;
    String filename = d->m_response.suggestedFilename();
    String mimeType = d->m_response.mimeType();
    CString cstr = filename.utf8();
    CString cstrType = mimeType.utf8();
    //printf("download file: %s, url=%s, length=%d, lengthReceived=%d\n", cstr.data(), job->request().url().url().utf8().data(), length, lengthReceived);

#if ENABLE(FILESYSTEM)
	result = true;
	if (m_fp == NULL) 
	{
		pos = m_filename.reverseFind('/'); 
		String path = m_filename.substring(0, pos);
		if (!fileExists(path))
		{
			makeAllDirectories(path); 
		}
		CString cstr =  m_filename.utf8();

		//printf("%s:%d orig-filename=%s\n", __FILE__, __LINE__, cstr.data());
		snprintf(file, sizeof(file)-1, "%s",
				cstr.data());
		// m_fp = openFile(file, "w+");
		// closeFile(m_fp);
		//printf("%s:%d filename=%s\n",  __FILE__, __LINE__, file);
		//m_fp = openFile(file, "a+");
		if (!m_overExist)
		{
			m_fp = openFile(file, "w+");
			m_overExist = true;
		}
		else
		{
			m_fp = openFile(file, "a+");
		}
		if (!m_fp)
		{
			printf("open file error  %s\n", file ); 
			return ;
		}
	}
	//printf("%s\n", ptr); 
	//int ret =  writeFile(ptr,1,  length, m_fp);
	//ret =  fwrite(ptr,1,  length, m_fp);
	ret =  writeFile((void*)ptr,1,  length, m_fp);
	if (m_fp)
	{
		//fclose(m_fp);
		closeFile(m_fp);
	}
	//closeFile(m_fp);
	m_fp = NULL;
#endif
    if (!result)
        cancel(job);
}

void  DownloadResourceClient::didFinishLoading(ResourceHandle* job)
{
    ResourceHandleInternal* d = job->getInternal();
    String filename = d->m_response.suggestedFilename();
    String mimeType = d->m_response.mimeType();
    CString cstr = filename.utf8();
    CString cstrType = mimeType.utf8();
#if ENABLE(FILESYSTEM)
	if (m_fp)
		closeFile(m_fp);
	m_fp = NULL;
#endif
    destroy(job);
}

void  DownloadResourceClient::didFail(ResourceHandle* job, const ResourceError&)
{
    didFinishLoading(job);
}

}
