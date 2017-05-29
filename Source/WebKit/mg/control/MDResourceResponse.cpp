/*
** $Id$
**
** MDResourceResponse.cpp: 
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2010-07-14 
*/

#include "minigui.h"
#include "config.h"

#include "MDResourceResponse.h"

#include "Logging.h"
#include "ResourceResponse.h"
#include "MDCommonFunc.h"

using namespace WebCore;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


BOOL mdRespIsNull(MDHResponse resp)
{
    ASSERT(resp);
    bool ret = ((ResourceResponse*)resp)->isNull();
    return ret ? TRUE : FALSE;
}

BOOL mdRespIsHTTP(MDHResponse resp)
{
    ASSERT(resp);
    bool ret = ((ResourceResponse*)resp)->isHTTP();
    return ret ? TRUE : FALSE;
}

char* mdRespGetURL(MDHResponse resp)
{
    ASSERT(resp);
    return copyStringToUtf8Char(((ResourceResponse*)resp)->url().string());
}

char* mdRespGetMIMEType(MDHResponse resp)
{
    ASSERT(resp);
    return copyStringToUtf8Char(((ResourceResponse*)resp)->mimeType());
}

long long mdRespGetExpectedContentLength(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->expectedContentLength(); 
}

char* mdRespGetTextEncodingName(MDHResponse resp)
{
    ASSERT(resp);
    return copyStringToUtf8Char(((ResourceResponse*)resp)->textEncodingName());
}

char* mdRespGetHTTPStatusText(MDHResponse resp)
{
    ASSERT(resp);
    return copyStringToUtf8Char(((ResourceResponse*)resp)->httpStatusText());
}

int mdRespGetHTTPStatusCode(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->httpStatusCode(); 
}

char* mdRespGetHTTPHeaderField(MDHResponse resp, const char* name)
{
    ASSERT(resp);
    return copyStringToUtf8Char(((ResourceResponse*)resp)->httpHeaderField(name));
}

BOOL mdRespIsCacheControlContainsNoCache(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->cacheControlContainsNoCache(); 
}

BOOL mdRespIsCacheControlContainsNoStore(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->cacheControlContainsNoStore(); 
}

BOOL mdRespIsCacheControlContainsMustRevalidate(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->cacheControlContainsMustRevalidate(); 
}

double mdRespGetCacheControlMaxAge(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->cacheControlMaxAge(); 
}

double mdRespGetDate(MDHResponse resp)
{
    ASSERT(resp);
    return ((ResourceResponse*)resp)->date(); 
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
