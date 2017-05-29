/*
 ** $Id: MDolphinDelegates.h 617 2011-04-29 09:55:31Z lijiangwei $
 **
 ** MDolphinDelegates.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-09 
 */

#ifndef _MDOLPHIN_DELEGATES_
#define _MDOLPHIN_DELEGATES_

#include "mdolphin.h"
#include "IMDWebUIDelegate.h"
#include "IMDWebFrameLoadDelegate.h"
#include "IMDWebDownloadDelegate.h"
#include "IMDWebHistoryDelegate.h"
#include "IMDWebCustomDelegate.h"
#include "IMDWebPolicyDelegate.h"

//define the old handlers
namespace Control
{
extern void (*MDCB_MESSAGE_BOX)(HWND hWnd, const char* pszText, const char* pszCaption);  
extern BOOL (*MDCB_CONFIRM_BOX)(HWND hWnd, const char* pszText, const char* pszCaption);
extern char *(*MDCB_PROMPT_BOX)(HWND hWnd, const char* pszText, const char* pszDefault, const char* pszCaption);

extern void (*MDCB_SET_TITLE) (HWND hWnd, const char * pText);
extern void (*MDCB_SET_LOCATION) (HWND hWnd, const char* url);
extern void (*MDCB_SET_STATUS)(HWND hWnd, const char* url);
extern void (*MDCB_SET_LOADING_STATUS) (HWND hWnd, BOOL isLoading, unsigned int progress);
extern void (*MDCB_SET_HISTORY_STATUS) (HWND hWnd, unsigned int backwardListCount,
            unsigned int forwardListCount, unsigned int capacity);
extern void (*MDCB_ERROR)(HWND hWnd, int errCode, const char * url);
extern BOOL (*MDCB_SET_IME_STATUS) (HWND hWnd, BOOL isShowIME);
extern BOOL (*MDCB_CHOOSEFILE_BOX) (HWND hWnd, char* fileName, size_t fileNameMaxLen);
extern BOOL (*MDCB_SAVE_FILE_DATA) (int downloadId, const char* url, const void* dataStream,
            size_t dataStreamSize, unsigned long totalSize, const char* suggestedFileName, 
            const char* mimeType , BOOL isFinished);
extern BOOL (*MDCB_PROVIDE_AUTH) (const char* title, char *userName, size_t userNameMaxLen, 
        char *password, size_t passwordMaxLen);
extern BOOL (*MDCB_URL_IS_VISITED) (const char* url);
extern BOOL (*MDCB_CREATE_POPUP_MENU) (HWND hWnd, const MDPopupMenu* menuInfo);
extern HWND (*MDCB_OPEN_WINDOW) (const char* url, DWORD styles, int x, int y, int width, int height);
extern void (*MDCB_CLOSE_WINDOW) (HWND hWnd);
extern void (*MDCB_SET_TOOLTIP) (HWND hWnd, int x, int y, const char *message, BOOL isShow);
extern const char *(*MDCB_CUSTOM_USERAGENT) (const char *url);
extern void (*MDCB_AFTER_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);
extern void (*MDCB_BEFORE_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);
extern BOOL  (*MDCB_SAVE_AS_FILE_DATA) (char *FileName);
extern void (*MDCB_GET_CARET_RECT) (const RECT* caret);
}


//extern the global delegates

extern IMDWebUIDelegate*             gMDWebUIDelegate;
extern IMDWebFrameLoadDelegate*      gMDWebFrameLoadDelegate;
extern IMDWebPolicyDelegate*         gMDWebPolicyDelegate;
extern IMDWebCustomDelegate*         gMDWebCustomDelegate;
extern IMDWebHistoryDelegate*        gMDWebHistoryDelegate;
extern IMDWebDownloadDelegate*       gMDWebDownloadDelegate;

void init_global_delegates(void);
void finish_global_delegates(void);
void set_global_callbacks(const MDCBInfo *cb);

#endif


