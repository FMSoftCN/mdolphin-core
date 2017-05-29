/**
 * \file mdolphin_errcode.h
 * \author Applications team of Feynman Software
 * \date 2007/12/21
 * 
 * This file includes necessary error code definitions of mDolphin.
 *
 \verbatim

    Copyright (C) 2007 Feynman Software.

    All rights reserved by Feynman Software.

    This file is part of mDolphin, a full-featured and high-end embedded web browser.

 \endverbatim
 */


#ifndef _MDOLPHIN_ERRCODE_H_
#define _MDOLPHIN_ERRCODE_H_

#include "mdconfig.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * \defgroup mdolphin_errcode mDolphin Error Code 
 * @{
 */

/** 
 * There are most, if not all, available error codes in mDolphin. 
 * \sa CB_ERROR
 */
typedef enum {
    /** The following error codes are network error codes.*/
    /** It's not error code, just as a base number for network error code.*/
    MDEC_NET_BASE = 400000,

    /** The following error codes are network normal error codes.*/
    /** It's not error code, just as a base number for network normal error code.*/
    MDEC_NET_NORMAL_BASE = MDEC_NET_BASE +0,
    /** The URL was not properly formatted. */
    MDEC_NET_URL_ERROR = MDEC_NET_NORMAL_BASE +1,
    /** The protocol was not supported. */
    MDEC_NET_UNSUPPORTED_PROTOCOL,
    /** Couldn't resolve the host. */
    MDEC_NET_DNS_ERROR,
    /** Failed to connect() to host or proxy. */
    MDEC_NET_COULDNT_CONNECT,
    /** It's canceled by WebKit */
    MDEC_NET_OPERATION_CANCELED,
    /** The request is blocked */
    MDEC_NET_REQUEST_BLOCKED,
    /** Interrupt for Policy Change */
    MDEC_NET_INTERRUPTED_FOR_POLICY_CHANGE,
    /** Unknown MIME type */
    MDEC_NET_UNKNOWN_MIMETYPE,
    /** File doesnot exist */
    MDEC_NET_FILE_DOESNOT_EXIST,
    /** It's an unknown network error.*/
    MDEC_NET_UNKNOWN_ERROR,

    /** The following error codes are network HTTP error codes.*/
    /** It's not error code, just as a base number for network HTTP error.*/
    MDEC_NET_HTTP_BASE = MDEC_NET_BASE +1000,
#ifdef ENABLE_NATIVEERROR
    /** It's the same as the HTTP server response code 400. */
    MDEC_NET_HTTP_400 = MDEC_NET_HTTP_BASE +400,
    /** It's the same as the HTTP server response code 401. */
    MDEC_NET_HTTP_401,
    /** It's the same as the HTTP server response code 402. */
    MDEC_NET_HTTP_402,
    /** It's the same as the HTTP server response code 403. */
    MDEC_NET_HTTP_403,
    /** It's the same as the HTTP server response code 404. */
    MDEC_NET_HTTP_404,
    /** It's the same as the HTTP server response code 405. */
    MDEC_NET_HTTP_405,
    /** It's the same as the HTTP server response code 406. */
    MDEC_NET_HTTP_406,
    /** It's the same as the HTTP server response code 407. */
    MDEC_NET_HTTP_407,
    /** It'sthe same as the HTTP server response code 408. */
    MDEC_NET_HTTP_408,
    /** It's the same as the HTTP server response code 409. */
    MDEC_NET_HTTP_409,
    /** It's the same as the HTTP server response code 410. */
    MDEC_NET_HTTP_410,
    /** It's the same as the HTTP server response code 411. */
    MDEC_NET_HTTP_411,
    /** It's the same as the HTTP server response code 412. */
    MDEC_NET_HTTP_412,
    /** It's the same as the HTTP server response code 413. */
    MDEC_NET_HTTP_413,
    /** It's the same as the HTTP server response code 414. */
    MDEC_NET_HTTP_414,
    /** It's the same as the HTTP server response code 415. */
    MDEC_NET_HTTP_415,
    /** It's the same as the HTTP server response code 416. */
    MDEC_NET_HTTP_416,
    /** It's the same as the HTTP server response code 417. */
    MDEC_NET_HTTP_417,
    /** It's the same as the HTTP server response code 500. */
    MDEC_NET_HTTP_500 = MDEC_NET_HTTP_BASE +500,
    /** It's the same as the HTTP server response code 501. */
    MDEC_NET_HTTP_501,
    /** It's the same as the HTTP server response code 502. */
    MDEC_NET_HTTP_502,
    /** It's the same as the HTTP server response code 503. */
    MDEC_NET_HTTP_503,   
    /** It's the same as the HTTP server response code 504. */
    MDEC_NET_HTTP_504,
    /** It's the same as the HTTP server response code 505. */
    MDEC_NET_HTTP_505,
#endif

    /** The following error codes are network FTP error codes.*/
    /** It's not error code, just as a base number for network FTP error.*/
    MDEC_NET_FTP_BASE = MDEC_NET_BASE +2000,
    /** It's the same as the FTP server response code 421. */
    MDEC_NET_FTP_421 = MDEC_NET_FTP_BASE +421,
    /** It's the same as the FTP server response code 425. */
    MDEC_NET_FTP_425 = MDEC_NET_FTP_BASE +425,
    /** It's the same as the FTP server response code 426. */
    MDEC_NET_FTP_426,
    /** It's the same as the FTP server response code 450. */
    MDEC_NET_FTP_450 = MDEC_NET_FTP_BASE +450,
    /** It's the same as the FTP server response code 451. */
    MDEC_NET_FTP_451,
    /** It's the same as the FTP server response code 452. */
    MDEC_NET_FTP_452,
    /** It's the same as the FTP server response code 500. */
    MDEC_NET_FTP_500 = MDEC_NET_FTP_BASE +500,
    /** It's the same as the FTP server response code 501. */
    MDEC_NET_FTP_501,
    /** It's the same as the FTP server response code 502. */
    MDEC_NET_FTP_502,
    /** It's the same as the FTP server response code 503. */
    MDEC_NET_FTP_503,  
    /** It's the same as the FTP server response code 504. */
    MDEC_NET_FTP_504,
    /** It's the same as the FTP server response code 530. */
    MDEC_NET_FTP_530 = MDEC_NET_FTP_BASE +530,
    /** It's the same as the FTP server response code 532. */
    MDEC_NET_FTP_532 = MDEC_NET_FTP_BASE +532,
    /** It's the same as the FTP server response code 550. */
    MDEC_NET_FTP_550 = MDEC_NET_FTP_BASE +550,
    /** It's the same as the FTP server response code 551. */
    MDEC_NET_FTP_551,
    /** It's the same as the FTP server response code 552. */
    MDEC_NET_FTP_552,
    /** It's the same as the FTP server response code 553. */
    MDEC_NET_FTP_553,
    /** It's an unknown FTP error. */
    MDEC_NET_FTP_UNKNOWN_ERROR = MDEC_NET_FTP_BASE +900,

    /** The following error codes are network FILE error codes.*/
    /** It's not error code, just as a base number for network FILE error.*/
    MDEC_NET_FILE_BASE = MDEC_NET_BASE +3000,
    /** It's a FILE error. A file given with FILE:// couldn't be opened.*/
    MDEC_NET_FILE_READ_ERROR = MDEC_NET_FILE_BASE +1,
    
    /** The following error codes are network SSL error codes.*/
    /** It's not error code, just as a base number for network SSL error.*/
    MDEC_NET_SSL_BASE = MDEC_NET_BASE +4000,
    /** It's an SSL error which occurred somewhere in the SSL/TLS handshake. */
    MDEC_NET_SSL_CONNECT_ERROR = MDEC_NET_SSL_BASE +1,
    /** The remote server's SSL certificate was deemed not OK.*/
    MDEC_NET_SSL_PEER_CERTIFICATE,
    /** The specified crypto engine wasn't found.*/
    MDEC_NET_SSL_ENGINE_NOTFOUND,
    /** Failed setting the selected SSL crypto engine as default.*/
    MDEC_NET_SSL_ENGINE_SETFAILED,
    /** Problem with the local client certificate.*/
    MDEC_NET_SSL_CERTPROBLEM,
    /** Couldn't use specified cipher.*/
    MDEC_NET_SSL_CIPHER,
    /** Peer certificate cannot be authenticated with known CA certificates.*/
    MDEC_NET_SSL_CACERT,
    /** Requested FTP SSL level failed.*/
    MDEC_NET_SSL_FTP_ERROR,
    /** Initiating the SSL Engine failed.*/
    MDEC_NET_SSL_ENGINE_INITFAILED,
    /** Problem with reading the SSL CA cert (path? access rights?)*/
    MDEC_NET_SSL_CACERT_BADFILE,

    /** The following error codes are network PROXY error codes.*/
    /** It's not error code, just as a base number for network PROXY error.*/
    MDEC_NET_PROXY_BASE = MDEC_NET_BASE +5000,
    /** It's a PROXY error. */
    MDEC_NET_PROXY_ERROR = MDEC_NET_PROXY_BASE +1,
} ERROR_CODE;

/** @} end of mdolphin_errcode */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
