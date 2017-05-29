/*
 ** $Id: MDResourceResponse.h 145 2010-07-14 03:15:02Z lijiangwei$
 **
 ** MDResourceResponse.h: header of resource response.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-14
 */
#ifndef MDResourceResponse_h
#define MDResourceResponse_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef void *MDHResponse;

/**
 * \fn BOOL mdRespIsNull(MDHResponse resp)
 * \brief Judge if the resource response is null.
 *
 * \param resp The handle of resource response.
 * \return TRUE if the resource response is null, FALSE otherwise.
 *
 */
BOOL mdRespIsNull(MDHResponse resp);

/**
 * \fn BOOL mdRespIsHTTP(MDHResponse resp)
 * \brief Judge if the resource response is HTTP.
 *
 * \param resp The handle of resource response.
 * \return TRUE if the resource response is HTTP, FALSE otherwise.
 *
 */
BOOL mdRespIsHTTP(MDHResponse resp);

/**
 * \fn const char* mdRespGetURL(MDHResponse resp)
 * \brief Get the URL from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The URL, the pointer returned must be freed using freeUtf8Char.   
 *
 */
char* mdRespGetURL(MDHResponse resp);

/**
 * \fn const char* mdRespGetMIMEType(MDHResponse resp)
 * \brief Get the MIME type from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The MIME type, the pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdRespGetMIMEType(MDHResponse resp);

/**
 * \fn long long mdRespGetExpectedContentLength(MDHResponse resp)
 * \brief Get the expected content length from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The expected content length.
 *
 */
long long mdRespGetExpectedContentLength(MDHResponse resp);

/**
 * \fn const char* mdRespGetTextEncodingName(MDHResponse resp)
 * \brief Get the text encoding name from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The text encoding name, the pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdRespGetTextEncodingName(MDHResponse resp);

/**
 * \fn const char* mdRespGetHTTPStatusText(MDHResponse resp)
 * \brief Get the HTTP status text from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The HTTP status text, the pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdRespGetHTTPStatusText(MDHResponse resp);

/**
 * \fn const int mdRespGetHTTPStatusCode(MDHResponse resp)
 * \brief Get the HTTP status code from the resource response.
 *
 * \param resp The handle of resource response.
 * \return The HTTP status code.
 *
 */
int mdRespGetHTTPStatusCode(MDHResponse resp);

/**
 * \fn const char* mdRespGetHTTPHeaderField(MDHResponse resp, const char* name)
 * \brief Get the HTTP header field value from the resource response.
 *
 * \param resp The handle of resource response.
 * \param name The standard HTTP header field name.
 * \return The HTTP header field value, the pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdRespGetHTTPHeaderField(MDHResponse resp, const char* name);

/**
 * \fn BOOL mdRespIsCacheControlContainsNoCache(MDHResponse resp)
 * \brief Judge if the HTTP header field "Cache-Control"contains "no-cache".
 *
 * \param resp The handle of resource response.
 * \return TRUE if "Cache-Control" contains "no-cache", FALSE otherwise.
 *
 */
BOOL mdRespIsCacheControlContainsNoCache(MDHResponse resp);

/**
 * \fn BOOL mdRespIsCacheControlContainsNoStore(MDHResponse resp)
 * \brief Judge if the HTTP header field "Cache-Control"contains "no-store".
 *
 * \param resp The handle of resource response.
 * \return TRUE if "Cache-Control" contains "no-store", FALSE otherwise..
 *
 */
BOOL mdRespIsCacheControlContainsNoStore(MDHResponse resp);

/**
 * \fn BOOL mdRespIsCacheControlContainsMustRevalidate(MDHResponse resp)
 * \brief Judge if the HTTP header field "Cache-Control"contains information for validate. 
 *
 * \param resp The handle of resource response.
 * \return TRUE if "Cache-Control" contains information for validate, FALSE otherwise.
 *
 */
BOOL mdRespIsCacheControlContainsMustRevalidate(MDHResponse resp);

/**
 * \fn double mdRespGetCacheControlMaxAge(MDHResponse resp);
 * \brief Get the "max-age" value from "cache-control".
 *
 * \param resp The handle of resource response.
 * \return The value of max-age.
 *
 */
double mdRespGetCacheControlMaxAge(MDHResponse resp);

/**
 * \fn double mdRespGetDate(MDHResponse resp);
 * \brief Get the "date" value from HTTP header.
 *
 * \param resp The handle of resource response.
 * \return The value of date.
 *
 */
double mdRespGetDate(MDHResponse resp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
