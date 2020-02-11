/*
** $Id$
**
** SharedTimerMg.cpp: share timer implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei 
**
** Create date: 2010-06-01
*/

#include "minigui.h"

#include "config.h"
#include "SharedTimer.h"
#include <wtf/CurrentTime.h>

#include <math.h>
#include <string.h>

namespace WebCore {

static DWORD timerID = 0;
static void (*sharedTimerFiredFunction)() = 0;

void setSharedTimerFiredFunction(void (*f)())
{
    sharedTimerFiredFunction = f;
}


//static BOOL timerFired(HWND, int, DWORD)
static BOOL timerFired(HWND, LINT, DWORD)   // gengyue
{
    if (sharedTimerFiredFunction)
        sharedTimerFiredFunction();
    return TRUE;
}

void setSharedTimerFireTime(double fireTime)
{
    static unsigned int t_id = 1;
    double interval = fireTime - currentTime();
    unsigned int intervalInMS;
    //printf("interval %s  %f\n", __FUNCTION__ , interval);
    if (interval < 0) {
        intervalInMS = 0;
    } else {
        interval *= 100;
        if (((unsigned int)interval) > 0x7FFFFFFFU)
            intervalInMS = 0x7FFFFFFFU;
        else
            intervalInMS = (unsigned int)ceil(interval);
    }

    stopSharedTimer();

    while (IsTimerInstalled(HWND_NULL, t_id))
        t_id++;
    
    // for Minigui bugs: SetTimerEx( 0 ) will wait 200ms when interval time is 0
    // so set interval time to minimum
    if( 0 == intervalInMS ){
        //printf("wwwwwwww : set intervalInMS to 1\n");
        intervalInMS = 1;
    }

    //printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww  %d\n", intervalInMS);
    if (SetTimerEx(HWND_NULL, t_id, intervalInMS, timerFired)){
        timerID = t_id;
        t_id++;
    }else
        fprintf(stderr, "mDolphin: SetTimerEx failed !\n");

}

void stopSharedTimer()
{
    if (timerID) {
        KillTimer(HWND_NULL, timerID);
        timerID = 0;
    }
}

}
