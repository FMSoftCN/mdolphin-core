/*
 ** $Id: MDResourceRequest.h 145 2010-07-14 03:15:02Z lijiangwei$
 **
 ** MDResourceRequest.h: header of resource request.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2010-07-14
 */
#ifndef MDResourceRequest_h
#define MDResourceRequest_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef void *MDHRequest;

typedef enum {
    MD_TargetIsMainFrame,
    MD_TargetIsSubframe,
    MD_TargetIsSubresource,  // Resource is a generic subresource.  (Generally a specific type should be specified)
    MD_TargetIsStyleSheet,
    MD_TargetIsScript,
    MD_TargetIsFontResource,
    MD_TargetIsImage,
    MD_TargetIsObject,
    MD_TargetIsMedia,
    MD_TargetIsWorker,
    MD_TargetIsSharedWorker
} MDETargetType;

typedef enum {
    MD_UseProtocolCachePolicy, // normal load
    MD_ReloadIgnoringCacheData, // reload
    MD_ReturnCacheDataElseLoad, // back/forward or encoding change - allow stale data
    MD_ReturnCacheDataDontLoad, // results of a post - allow stale data and only use cache
}MDEResourceRequestCachePolicy ;

/**
 * \fn BOOL mdReqIsNull(MDHRequest req)
 * \brief Judge if the resource request is null.
 *
 * \param req The handle of resource request.
 * \return TRUE if the resource request is null, FALSE otherwise.
 *
 */
BOOL mdReqIsNull(MDHRequest req);

/**
 * \fn BOOL mdReqIsEmpty(MDHRequest req)
 * \brief Judge if the resource request is empty.
 *
 * \param req The handle of resource request.
 * \return TRUE if the resource request is empty, FALSE otherwise.
 *
 */
BOOL mdReqIsEmpty(MDHRequest req);

/**
 * \fn const char* mdReqGetURL(MDHRequest req)
 * \brief Get the URL from the resource request.
 *
 * \param req The handle of resource request.
 * \return The URL. The pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdReqGetURL(MDHRequest req);

/**
 * \fn void mdReqSetURL(MDHRequest req, const char* url)
 * \brief Set the URL of the resource request.
 *
 * \param req The handle of resource request.
 * \param url The URL.
 *
 */
void mdReqSetURL(MDHRequest req, const char* url);

/**
 * \fn MDEResourceRequestCachePolicy mdReqGetCachePolicy(MDHRequest req);
 * \brief Get the cache policy from the resource request.
 *
 * \param req The handle of resource request.
 * \return The URL.
 *
 */
MDEResourceRequestCachePolicy mdReqGetCachePolicy(MDHRequest req);

/**
 * \fn void mdReqSetCachePolicy(MDHRequest req, int policy)
 * \brief Set the cache policy of the resource request.
 *
 * \param req The handle of resource request.
 * \param policy The cache policy.
 *
 */
void mdReqSetCachePolicy(MDHRequest req, MDEResourceRequestCachePolicy policy);

/**
 * \fn const char* mdReqGetHTTPMethod(MDHRequest req)
 * \brief Get the HTTP method from the resource request.
 *
 * \param req The handle of resource request.
 * \return The HTTP method. The pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdReqGetHTTPMethod(MDHRequest req);

/**
 * \fn void mdReqSetCachePolicy(MDHRequest req, int policy)
 * \brief Set the HTTP method of the resource request.
 *
 * \param req The handle of resource request.
 * \param method The HTTP method.
 *
 */
void mdReqSetHTTPMethod(MDHRequest req, const char* method);

/**
 * \fn const char* mdReqGetHTTPHeaderField(MDHRequest req, const char* name)
 * \brief Get the HTTP header field from the resource request.
 *
 * \param req The handle of resource request.
 * \param name The HTTP header field.
 * \return The HTTP header field value. The pointer returned must be freed using freeUtf8Char.
 *
 */
char* mdReqGetHTTPHeaderField(MDHRequest req, const char* name);

/**
 * \fn void mdReqSetHTTPHeaderField(MDHRequest req, const char* name, const char* value)
 * \brief Set the HTTP method of the resource request.
 *
 * \param req The handle of resource request.
 * \param name The HTTP header field name.
 * \param value The HTTP header field value.
 *
 */
void mdReqSetHTTPHeaderField(MDHRequest req, const char* name, const char* value);

/**
 * \fn void mdReqAddHTTPHeaderField(MDHRequest req, const char* name, const char* value)
 * \brief Set the HTTP method of the resource request.
 *
 * \param req The handle of resource request.
 * \param name The HTTP header field name.
 * \param value The HTTP header field value.
 *
 */
void mdReqAddHTTPHeaderField(MDHRequest req, const char* name, const char* value);

/**
 * \fn BOOL mdReqIsAllowCookies(MDHRequest req)
 * \brief Judge if the resource request allows cookies.
 *
 * \param req The handle of resource request.
 * \return TRUE if the resource request allows cookies, FALSE otherwise.
 *
 */
BOOL mdReqIsAllowCookies(MDHRequest req);

/**
 * \fn void mdReqSetAllowCookies(MDHRequest req, BOOL allow);
 * \brief Set the resource request allows cookies.
 *
 * \param req The handle of resource request.
 * \param allow Allow cookies.
 *
 */
void mdReqSetAllowCookies(MDHRequest req, BOOL allow);

/**
 * \fn MDETargetType mdReqGetTargetType(MDHRequest req)
 * \brief Get the target type of the resource rqeust.
 *
 * \param req The handle of resource request.
 *
 */
MDETargetType mdReqGetTargetType(MDHRequest req);

/**
 * \fn void mdReqSetTargetType(MDHRequest req, MDETargetType type)
 * \brief Set the target type of the resource rqeust.
 *
 * \param req The handle of resource request.
 * \param type The target type.
 *
 */
void mdReqSetTargetType(MDHRequest req, MDETargetType type);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
