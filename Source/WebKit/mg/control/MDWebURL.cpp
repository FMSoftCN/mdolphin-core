/* 
 ** $Id$
 **
 ** MDWebURL.cpp
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: 
 **  
 ** Create date: 2010-12-13
*/

#include "minigui.h"
#include "config.h"

#include "MDWebURL.h"
#include "CString.h"
#include "KURL.h"
#include "FileSystem.h"

using namespace WebCore;

class MDWebURLImpl {
public:
    MDWebURLImpl(const char* sUrl)
    {
        m_url = new KURL(ParsedURLString, sUrl);
    }

    ~MDWebURLImpl()
    {
        delete(m_url);
        if (m_curl)
            free(m_curl);
    }

    char* standardURL();
    
private:
    KURL* m_url;
    char* m_curl;
};

MDWebURL::MDWebURL(const char* sUrl)
{
    m_impl = new MDWebURLImpl(sUrl);
}

MDWebURL::~MDWebURL()
{
    if (m_impl)
        delete m_impl;
}

char* MDWebURL::standardURL()
{
    return m_impl->standardURL();
}

static bool standardLocalFile(String& url)
{
    if (url.isEmpty()) {
        return false;
    }

    if (url.startsWith("/")) {
        url = "file://";
        return true;
    }

    bool ret = false;
#if ENABLE(FILESYSTEM)
    // FIXME: Maybe we should throw this to app.
    // Should we expand FileSystem to get current dir name?
    char* path = get_current_dir_name();
    String fullPath = path; 
    fullPath.append("/");
    fullPath.append(url);
    if (fileExists(fullPath)) {
        url = "file://";
        url.append(path);
        url.append("/");
        ret = true;
    }
    free(path);
#endif

    return ret;
}

char* MDWebURLImpl::standardURL()
{
    String url = m_url->string();
    
    if (m_url->isValid()) {
        // deal with empty host
        // Note we ignore file protocol
        if (!m_url->isLocalFile() && m_url->host().isEmpty()) {                
            unsigned start = m_url->hostStart();
            unsigned i = m_url->hostStart();
            for (; i < url.length() - start; ++i) {
                if (url[i] != '/')
                    break;
            }
            url.remove(start, i - start);
            url.insert("//", m_url->hostStart());
        }
    } else {
        if (m_url->protocol().isEmpty()) {
            if (!standardLocalFile(url)) {
                url = "http://";
            } 
            url.append(m_url->string());
        }
    }
    
    m_curl = strdup(url.utf8().data());
    return m_curl;
}
