/*
** $Id: MainThreadMg.cpp 760 2011-08-18 07:22:16Z xueshuming $
**
** MainThreadMg.cpp: Not implemented.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2010-06-07 
*/

#include "config.h"

#include "MainThread.h"
#include "Threading.h"
#include <string.h>
#include <stdio.h>

#include "minigui.h"

namespace WTF {

#define MSG_MDTIMER 0xef20
static WNDPROC oldDefWndProc;
static BOOL isInitialized = FALSE;

static int ThreadingWindowWndProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message){
        case MSG_MDTIMER:
        {
            dispatchFunctionsFromMainThread();
            return 0;
        }
    }
    return oldDefWndProc(hwnd, message, wParam, lParam);
 }

void initializeMainThreadPlatform()
{
    if (!isInitialized) {
        oldDefWndProc = __mg_def_proc[0];
        __mg_def_proc[0] = ThreadingWindowWndProc;
        isInitialized = TRUE;
    }
}

HWND getUIRootWindow (void)
{
    HWND hWnd = HWND_NULL;

    do {
        hWnd = GetNextMainWindow(hWnd);
        if (GetHosting(hWnd) == HWND_DESKTOP)
            return hWnd;
    } while (hWnd != HWND_INVALID && hWnd != HWND_NULL);

    return HWND_NULL;
}

void scheduleDispatchFunctionsOnMainThread()
{
    SendNotifyMessage(getUIRootWindow(), MSG_MDTIMER, 0, 0);
}

} // namespace WTF

