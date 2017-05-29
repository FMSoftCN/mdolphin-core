/*
 ** $Id: main.c 122 2010-06-19 06:03:13Z xwyan $
 **
 ** mdhello.c:  first test example.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software
 **
 ** Current maintainer: lijiangwei
 **
 ** Create date: 2010-06-07
 */

/* MiniGUI header file.*/
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <string.h>
/* mDolphin header file.*/
#include "mdolphin.h"
#include "IMDWebView.h"

#define IDC_MDOLPHIN 101
HWND mdWnd;

//static const char * home_url = "http://www.minigui.com/";
static const char * home_url = "http://www.numion.com/Stopwatch/index.html";
static HWND my_create_new_window(const char* url, DWORD styles, int x, int y, int width, int height)
{
	return mdWnd;
}

static void my_message_callback (HWND parent, const char * text, const char * caption)
{
    MessageBox (parent, text, caption, MB_OK);
}

static BOOL my_confirm_callback (HWND parent, const char * text, const char * caption)
{
    if (MessageBox (parent, text, caption, MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        return TRUE;
    return FALSE;
}

static void set_callback_info(HWND hWnd)
{
    MDCBInfo cbInfo;
    memset (&cbInfo, 0, sizeof(CB_INFO));
    
    cbInfo.MDCB_MESSAGE_BOX = my_message_callback;
    cbInfo.MDCB_CONFIRM_BOX = my_confirm_callback;
    cbInfo.MDCB_OPEN_WINDOW = my_create_new_window;
	mdSetCallbackInfo(hWnd, &cbInfo);
}

static void init_setup(HWND hwnd)
{
    mdSetIntProperty(hwnd, MD_INT_MEDIUM_FONT_SIZE, 16);
}

static int MDolphinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            break;

        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }
    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* argv[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;
    RECT rc;

    if (argv[1])
        home_url = argv[1];

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mDolphin" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_CAPTION | WS_BORDER;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "mDolphin";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MDolphinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = RECTW(g_rcScr);
    CreateInfo.by = RECTH(g_rcScr);
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    GetClientRect(hMainWnd, &rc);
    IMDWebView *view = mdCreateWebViewInstance(rc, IDC_MDOLPHIN, hMainWnd);
	mdWnd = view->viewWindow();

	set_callback_info(mdWnd);
	init_setup(mdWnd);
    view->loadURL(home_url);

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
