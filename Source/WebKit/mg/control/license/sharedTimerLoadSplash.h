/*
 ** $Id$
 **
 ** sharedTimerLoadSplash.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-26 
 */

#ifndef sharedTimerLoadSplash_h
#define sharedTimerLoadSplash_h

#ifdef  _MD_ENABLE_LOADSPLASH

#include "minigui.h"
#include "loadSplash.h"
#include <wtf/HashSet.h>

#define USE_THREAD_ANIMATION

#ifdef USE_THREAD_ANIMATION
#include "ctimer.h"
#endif

class MD_LOCAL sharedTimerLoadSplash {
public:
    static sharedTimerLoadSplash*  sharedTimerLoadSplashInstance();
    bool registerTimer();
    bool unRegisterTimer();

    bool registerObject(loadSplash *);
    bool unRegisterObject(loadSplash *);

    static BOOL checkAllObject(HWND, int, DWORD);// callback function of timer
    static unsigned int sharedTimerTicks;

    HashSet<loadSplash*>* loadSplashQueue() { return &m_loadSplashQueue; }

#ifdef USE_THREAD_ANIMATION
    void lock();
    void unlock();
#endif

private:
    sharedTimerLoadSplash();
    ~sharedTimerLoadSplash();

     HashSet<loadSplash*>  m_loadSplashQueue;
     DWORD m_timerID;

#ifdef USE_THREAD_ANIMATION
    class CTimer *m_ctimer;
    pthread_mutex_t m_mutex;
#endif
};

#endif //end _MD_ENABLE_LOADSPLASH

#endif //sharedTimerLoadSplash_h
