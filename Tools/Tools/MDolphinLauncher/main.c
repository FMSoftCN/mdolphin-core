/*
 ** $Id: main.c 528 2011-03-18 08:10:33Z xwyan $
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

/* mDolphin header file.*/
#include "mdolphin.h"

#define IDC_MDOLPHIN 101

static const char * home_url = "http://www.numion.com/Stopwatch/index.html";

int MDolphinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE: {
        RECT rc;
        GetClientRect(hWnd, &rc);
        /* Create a mDolphin control class instance. */
        HWND mdolphin_hwnd = CreateWindow (MDOLPHIN_CTRL,
                                           "",
                                           WS_VISIBLE | WS_CHILD,
                                           IDC_MDOLPHIN,
                                           0, 0, RECTW(rc), RECTH(rc), hWnd, 0);

        mdSetIntProperty(mdolphin_hwnd, MD_INT_ENABLE_JAVASCRIPT, TRUE);
        mdSetIntProperty(mdolphin_hwnd, MD_INT_ENABLE_PLUGIN, TRUE);
        mdNavigate(mdolphin_hwnd, MD_NAV_GOTO, home_url);
    }
    break;

    case MSG_CSIZECHANGED:
    {
        RECT newRect;
        SetRect(&newRect, 0, 0, wParam, lParam);
        SendMessage(GetDlgItem(hWnd, IDC_MDOLPHIN), 
                MSG_SIZECHANGED, (DWORD)&newRect, (DWORD)&newRect);
        return 0;
    }

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

    if (argv[1])
        home_url = argv[1];

    mdRegisterMDolphinControl();
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

    HWND hMainWnd;
    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    mdUnregisterMDolphinControl();
    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
