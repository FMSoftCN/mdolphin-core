/*
 ** $Id: CertificateMg.h 1053 2012-04-20 06:38:44Z xwyan $
 **
 ** CertificateMg.h: 
 **
 ** Copyright (C) 2003 ~ 2011 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2012-04-13 
 */

#ifndef CertificateMg_H
#define CertificateMg_H

#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/lhash.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <curl/curl.h>

#include <wtf/text/CString.h>
namespace WebCore {
#if ENABLE(SSL)
    CURLcode sslctxfun(CURL * curl, void * sslctx, void * obj);
#if ENABLE(SSLFILE)
    const CString& caPath(void);	
    void setCAPath(const char *caPath);
#endif
#endif
}

#endif //CertificateMg_H
