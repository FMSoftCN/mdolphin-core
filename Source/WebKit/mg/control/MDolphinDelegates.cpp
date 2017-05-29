/*
 ** $Id: MDolphinDelegates.cpp 1054 2012-04-30 07:50:01Z weiym $
 **
 ** MDolphinDelegates.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-09Default 
 */
#include "minigui.h"
#include "config.h"

#include "MDolphinDelegates.h"
#include "MDDefaultWebUIDelegate.h"
#include "MDDefaultWebFrameLoadDelegate.h"
#include "MDDefaultWebDownloadDelegate.h"
#include "MDDefaultWebHistoryDelegate.h"
#include "MDDefaultWebCustomDelegate.h"
#include "MDDefaultWebPolicyDelegate.h"

// We put the following here purposely.
// Because we want to spread the code of watermark to avoid hack.
#ifdef _MD_ENABLE_WATERMARK
#include "license/licenseHidden.h"
#include "license/c_files/watermark/watermark3.dat.c"
#endif

using namespace Control;

namespace Control
{
void (*MDCB_MESSAGE_BOX)(HWND hWnd, const char* pszText, const char* pszCaption);  
BOOL (*MDCB_CONFIRM_BOX)(HWND hWnd, const char* pszText, const char* pszCaption);
char *(*MDCB_PROMPT_BOX)(HWND hWnd, const char* pszText, const char* pszDefault, const char* pszCaption);

void (*MDCB_SET_TITLE) (HWND hWnd, const char* pText);
void (*MDCB_SET_LOCATION) (HWND hWnd, const char* url);
void (*MDCB_SET_STATUS)(HWND hWnd, const char* url);
void (*MDCB_SET_LOADING_STATUS) (HWND hWnd, BOOL isLoading, unsigned int progress);
void (*MDCB_SET_HISTORY_STATUS) (HWND hWnd, unsigned int backwardListCount,
            unsigned int forwardListCount, unsigned int capacity);
void (*MDCB_ERROR)(HWND hWnd, int errCode, const char * url);
BOOL (*MDCB_SET_IME_STATUS) (HWND hWnd, BOOL isShowIME);
BOOL (*MDCB_CHOOSEFILE_BOX) (HWND hWnd, char* fileName, size_t fileNameMaxLen);
BOOL (*MDCB_SAVE_FILE_DATA) (int downloadId, const char* url, const void* dataStream,
            size_t dataStreamSize, unsigned long totalSize, const char* suggestedFileName, 
            const char* mimeType , BOOL isFinished);
BOOL (*MDCB_PROVIDE_AUTH) (const char* title, char *userName, size_t userNameMaxLen, 
        char * password, size_t passwordMaxLen);
BOOL (*MDCB_URL_IS_VISITED) (const char* url);
BOOL (*MDCB_CREATE_POPUP_MENU) (HWND hWnd, const POPUP_MENU_DATA *data);
HWND (*MDCB_OPEN_WINDOW) (const char* url, DWORD styles, int x, int y, int width, int height);
void (*MDCB_CLOSE_WINDOW) (HWND hWnd);
void (*MDCB_SET_TOOLTIP) (HWND hWnd, int x, int y, const char *message, BOOL isShow);
const char *(*MDCB_CUSTOM_USERAGENT) (const char *url);
void (*MDCB_AFTER_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);
void (*MDCB_BEFORE_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);
BOOL  (*MDCB_SAVE_AS_FILE_DATA) (char *FileName);
#if defined(ENABLE_SSL) && ENABLE_SSL
BOOL (*MDCB_PROVIDE_CLIENT_CERT) (MD_CERT_DATA **x509, MD_SSL_PKEY **pkey, 
                const MD_CERT_NAME **names, int count);
int  (*MDCB_VERIFY_SERVER_CERT) (int result, MD_CERT_DATA *x509);
#endif
void (*MDCB_GET_CARET_RECT) (const RECT* caret);
}



//the global delegates

IMDWebUIDelegate*             gMDWebUIDelegate = NULL;
IMDWebFrameLoadDelegate*      gMDWebFrameLoadDelegate = NULL;
IMDWebPolicyDelegate*         gMDWebPolicyDelegate = NULL;
IMDWebCustomDelegate*         gMDWebCustomDelegate = NULL;
IMDWebHistoryDelegate*        gMDWebHistoryDelegate = NULL;
IMDWebDownloadDelegate*       gMDWebDownloadDelegate = NULL;

//define the default deletates
static MDDefaultWebUIDelegate*             _defMDWebUIDelegate;
static MDDefaultWebFrameLoadDelegate*      _defMDWebFrameLoadDelegate;
static MDDefaultWebPolicyDelegate*         _defMDWebPolicyDelegate;
static MDDefaultWebCustomDelegate*         _defMDWebCustomDelegate;
static MDDefaultWebHistoryDelegate*        _defMDWebHistoryDelegate;
static MDDefaultWebDownloadDelegate*       _defMDWebDownloadDelegate;




static IUnknown * set_delegate(IUnknown** pgDelegate, IUnknown * newDelege, IUnknown * defDelegate)
{
    if(*pgDelegate == newDelege && newDelege!= NULL)
        return *pgDelegate;

    IUnknown* old_delegate = *pgDelegate;

    if( *pgDelegate)
        (*pgDelegate)->release();

    if( !newDelege)
        newDelege = defDelegate;
    
    *pgDelegate = newDelege;
    (*pgDelegate)->addRef();
    return old_delegate;
}

#define SET_DELEGETE(Delegate)  IMD##Delegate* mdSet##Delegate(IMD##Delegate *delegate) { \
    return (IMD##Delegate*) set_delegate((IUnknown**)&gMD##Delegate, (IUnknown*)delegate, (IUnknown*) _defMD##Delegate); }

SET_DELEGETE(WebUIDelegate)
SET_DELEGETE(WebFrameLoadDelegate)
SET_DELEGETE(WebCustomDelegate)
SET_DELEGETE(WebDownloadDelegate)
SET_DELEGETE(WebHistoryDelegate)
SET_DELEGETE(WebPolicyDelegate)


void init_global_delegates()
{
    //init old handlers
    MDCB_MESSAGE_BOX              = NULL;
    MDCB_CONFIRM_BOX          = NULL;
    MDCB_PROMPT_BOX           = NULL;
    MDCB_SET_TITLE            = NULL;
    MDCB_SET_LOCATION         = NULL;
    MDCB_SET_STATUS           = NULL;
    MDCB_SET_LOADING_STATUS   = NULL;
    MDCB_SET_HISTORY_STATUS   = NULL;
    MDCB_SET_IME_STATUS       = NULL;
    MDCB_ERROR                = NULL;
    MDCB_CHOOSEFILE_BOX       = NULL;
    MDCB_SAVE_FILE_DATA       = NULL;
    MDCB_PROVIDE_AUTH         = NULL; 
    MDCB_URL_IS_VISITED       = NULL;
    MDCB_CREATE_POPUP_MENU    = NULL; 
    MDCB_OPEN_WINDOW          = NULL; 
    MDCB_CLOSE_WINDOW         = NULL; 
    MDCB_SET_TOOLTIP          = NULL;
    MDCB_CUSTOM_USERAGENT     = NULL;
    MDCB_AFTER_DRAWING        = NULL;
    MDCB_BEFORE_DRAWING           = NULL;
#if defined(ENABLE_SSL) && ENABLE_SSL
    MDCB_PROVIDE_CLIENT_CERT  = NULL;
    MDCB_VERIFY_SERVER_CERT   = NULL;
#endif
    MDCB_GET_CARET_RECT       = NULL;

    _defMDWebUIDelegate = MDDefaultWebUIDelegate::createInstance();
    _defMDWebFrameLoadDelegate = MDDefaultWebFrameLoadDelegate::createInstance();
    _defMDWebPolicyDelegate = MDDefaultWebPolicyDelegate::createInstance();
    _defMDWebCustomDelegate = MDDefaultWebCustomDelegate::createInstance();
    _defMDWebHistoryDelegate = MDDefaultWebHistoryDelegate::createInstance();
    _defMDWebDownloadDelegate = MDDefaultWebDownloadDelegate::sharedInstance();
    //init golbal delegates
    mdSetWebUIDelegate(NULL);
    mdSetWebFrameLoadDelegate(NULL);
    mdSetWebCustomDelegate(NULL);
    mdSetWebDownloadDelegate(NULL);
    mdSetWebPolicyDelegate(NULL);
    mdSetWebHistoryDelegate(NULL);
}

void finish_global_delegates(void)
{
    _defMDWebUIDelegate->release();
    _defMDWebFrameLoadDelegate->release();
    _defMDWebPolicyDelegate->release();
    _defMDWebCustomDelegate->release();
    _defMDWebHistoryDelegate->release();
    _defMDWebDownloadDelegate->release();

    _defMDWebUIDelegate = NULL;
    _defMDWebFrameLoadDelegate = NULL;
    _defMDWebPolicyDelegate = NULL;
    _defMDWebCustomDelegate = NULL;
    _defMDWebHistoryDelegate = NULL;
    _defMDWebDownloadDelegate = NULL;
}

#undef SET_DELEGETE

void set_global_callbacks(const MDCBInfo *cb)
{
    MDCB_MESSAGE_BOX          = cb->MDCB_MESSAGE_BOX ;
    MDCB_CONFIRM_BOX          = cb->MDCB_CONFIRM_BOX ;
    MDCB_PROMPT_BOX           = cb->MDCB_PROMPT_BOX ;
    MDCB_SET_TITLE            = cb->MDCB_SET_TITLE ;
    MDCB_SET_LOCATION         = cb->MDCB_SET_LOCATION ;
    MDCB_SET_STATUS           = cb->MDCB_SET_STATUS ;
    MDCB_SET_LOADING_STATUS   = cb->MDCB_SET_LOADING_STATUS;
    MDCB_SET_HISTORY_STATUS   = cb->MDCB_SET_HISTORY_STATUS;
    MDCB_CHOOSEFILE_BOX       = cb->MDCB_CHOOSEFILE_BOX;
    MDCB_SAVE_FILE_DATA       = cb->MDCB_SAVE_FILE_DATA;
    MDCB_SET_IME_STATUS       = cb->MDCB_SET_IME_STATUS;
    MDCB_ERROR                = cb->MDCB_ERROR;
    MDCB_PROVIDE_AUTH         = cb->MDCB_PROVIDE_AUTH; 
    MDCB_URL_IS_VISITED       = cb->MDCB_URL_IS_VISITED;
    MDCB_CREATE_POPUP_MENU    = cb->MDCB_CREATE_POPUP_MENU; 
    MDCB_OPEN_WINDOW          = cb->MDCB_OPEN_WINDOW; 
    MDCB_CLOSE_WINDOW         = cb->MDCB_CLOSE_WINDOW; 
    MDCB_SET_TOOLTIP          = cb->MDCB_SET_TOOLTIP;
    MDCB_CUSTOM_USERAGENT     = cb->MDCB_CUSTOM_USERAGENT;
    MDCB_AFTER_DRAWING        = cb->MDCB_AFTER_DRAWING;
    MDCB_BEFORE_DRAWING       = cb->MDCB_BEFORE_DRAWING;
    MDCB_SAVE_AS_FILE_DATA    = cb->MDCB_SAVE_AS_FILE_DATA;
#if defined(ENABLE_SSL) && ENABLE_SSL
    MDCB_PROVIDE_CLIENT_CERT  = cb->MDCB_PROVIDE_CLIENT_CERT;
    MDCB_VERIFY_SERVER_CERT   = cb->MDCB_VERIFY_SERVER_CERT;
#endif
    MDCB_GET_CARET_RECT       = cb->MDCB_GET_CARET_RECT;
}

#if defined(ENABLE_SSL) && ENABLE_SSL
typedef BOOL (*MDClientCertFunc) (MD_CERT_DATA **x509, MD_SSL_PKEY **evpKey, 
        const MD_CERT_NAME **certNames, int count);
typedef int (*MDServerCertFunc) (int x509result, MD_CERT_DATA *x509);

MDClientCertFunc mdGetClientCertCallback(void)
{
    return Control::MDCB_PROVIDE_CLIENT_CERT;
}

MDServerCertFunc mdGetServerCertCallback(void)
{
    return Control::MDCB_VERIFY_SERVER_CERT;
}
#endif
