/*
 ** $Id$
 **
 ** MDResourceRequest.cpp:
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

#include "MDResourceRequest.h"

#include "Logging.h"
#include "ResourceRequest.h"
#include "MDCommonFunc.h"

using namespace WebCore;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BOOL mdReqIsNull(MDHRequest req)
{
    ASSERT(req);
    bool ret = ((ResourceRequest*)req)->isNull();
    return ret ? TRUE : FALSE;
}

BOOL mdReqIsEmpty(MDHRequest req)
{
    ASSERT(req);
    bool ret = ((ResourceRequest*)req)->isEmpty();
    return ret ? TRUE : FALSE;
}

char* mdReqGetURL(MDHRequest req)
{
    ASSERT(req);

    return copyStringToUtf8Char(((ResourceRequest*)req)->url().string());
}

void mdReqSetURL(MDHRequest req, const char* url)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setURL(KURL(ParsedURLString, url));
}

MDEResourceRequestCachePolicy mdReqGetCachePolicy(MDHRequest req)
{
    ASSERT(req);
    return (MDEResourceRequestCachePolicy)(((ResourceRequest*)req)->cachePolicy());
}

void mdReqSetCachePolicy(MDHRequest req, MDEResourceRequestCachePolicy policy)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setCachePolicy((ResourceRequestCachePolicy)policy);
}

char* mdReqGetHTTPMethod(MDHRequest req)
{
    ASSERT(req);
    return copyStringToUtf8Char(((ResourceRequest*)req)->httpMethod());
}

void mdReqSetHTTPMethod(MDHRequest req, const char* method)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setHTTPMethod(String(method));
}

char* mdReqGetHTTPHeaderField(MDHRequest req, const char* name)
{
    ASSERT(req);
    return copyStringToUtf8Char(((ResourceRequest*)req)->httpHeaderField(name));
}

void mdReqSetHTTPHeaderField(MDHRequest req, const char* name, const char* value)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setHTTPHeaderField(name, String(value));
}

void mdReqAddHTTPHeaderField(MDHRequest req, const char* name, const char* value)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->addHTTPHeaderField(name, String(value));
}

BOOL mdReqIsAllowCookies(MDHRequest req)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->allowCookies();
}

void mdReqSetAllowCookies(MDHRequest req, BOOL allow)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setAllowCookies(allow);
}

MDETargetType mdReqGetTargetType(MDHRequest req)
{
    ASSERT(req);
    return (MDETargetType)(((ResourceRequest*)req)->targetType());
}

void mdReqSetTargetType(MDHRequest req, MDETargetType type)
{
    ASSERT(req);
    return ((ResourceRequest*)req)->setTargetType((WebCore::ResourceRequestBase::TargetType)type);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
