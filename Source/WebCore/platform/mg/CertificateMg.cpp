/*
 ** $Id: CertificateMg.cpp 1058 2012-07-25 06:16:45Z xueshuming $
 **
 ** CertificateMg.cpp:
 **
 ** Copyright (C) 2003 ~ 2011 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: Yan Xiaowei
 **
 ** Create date: 2012-04-13
 */


#include "config.h"
#include "CertificateMg.h"

#if ENABLE(SSL)
typedef int (*MDClientCertFunc) (void **x509, void **evpKey,
    const void **certNames, int count);
typedef int (*MDServerCertFunc) (int x509result, void *x509);

extern MDClientCertFunc mdGetClientCertCallback(void);
extern MDServerCertFunc mdGetServerCertCallback(void);
#endif

namespace WebCore
{
#if ENABLE(SSL)
/*ssl Cipher list to be used*/
#define CIPHER_LIST "RC4-MD5"

static int ssl_client_cert_callback(SSL *ssl, X509 **x509, EVP_PKEY **pkey)
{
    int result = 0;
    MDClientCertFunc cb = (MDClientCertFunc)mdGetClientCertCallback();
    if (cb) {
        X509_NAME **dnames = NULL;
        STACK_OF(X509_NAME) *sk;
        int i, count=0;
        sk = SSL_get_client_CA_list(ssl);
        count = sk ? sk_X509_NAME_num(sk): 0;
        if (count > 0) {
            dnames = (X509_NAME **)malloc(count * sizeof(X509_NAME *));
            if (!dnames)
                return result;

            for (i =0; i < count; i++)
                dnames[i] = sk_X509_NAME_value(sk, i);
        }
        result = cb((void**)x509,
                (void**)pkey, (const void **)dnames, count);
        if (count)
            free(dnames);
    }
    return result;
}

static int ssl_app_verify_callback(X509_STORE_CTX *ctx, void *obj)
{
    int ok, err, newErr;
    ok= X509_verify_cert(ctx);
    err = X509_STORE_CTX_get_error(ctx);
    MDServerCertFunc cb = (MDServerCertFunc)mdGetServerCertCallback();
    if (cb) {
        newErr = cb(err, X509_STORE_CTX_get_current_cert(ctx));
        if (newErr != err) {
            if (newErr == X509_V_OK)
                ok =1;
            X509_STORE_CTX_set_error(ctx, newErr);
        }
    }
    return(ok);
}

CURLcode sslctxfun(CURL * curl, void * sslctx, void * obj)
{
    SSL_CTX * ctx = (SSL_CTX *) sslctx ;
    if (!ctx) {
        LOG_ERROR("SSL :Error creating the context.\n");
        goto err;
    }

    SSL_CTX_set_client_cert_cb(ctx, ssl_client_cert_callback);

    SSL_CTX_set_quiet_shutdown(ctx, 1);
    if (SSL_CTX_set_cipher_list(ctx, CIPHER_LIST) <= 0) {
        LOG_ERROR("SSL :Error setting the cipher list.\n");
        goto err;
    }
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    //SSL_CTX_set_cipher_list(ctx, "!ADH:RC4+RSA:HIGH:MEDIUM:LOW:EXP:+SSLv2:+EXP");
    SSL_CTX_set_cipher_list(ctx, "ALL");

    /* Set for server verification*/
    SSL_CTX_set_verify_depth(ctx, 2);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_cert_verify_callback(ctx, ssl_app_verify_callback, obj);
    return CURLE_OK ;

err:
    return CURLE_SSL_CERTPROBLEM;
}

#if ENABLE(SSLFILE)
static CString _caPath;

const CString& caPath(void)
{
    return _caPath;
}

void setCAPath(const char* caPath)
{
    _caPath = CString(caPath);
}
#endif

#endif
}
