/*
 * =====================================================================================
 *
 *       Filename:  ctime.cpp
 *       Copyright (C) 2004 ~ 2007 Feynman Software. 
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年10月16日 12时14分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lvlei (mn), lvlei@minigui.com
 *        Company:  Feynman Software
 *
 * =====================================================================================
 */

#include "mdconfig.h"
#ifdef  _MD_ENABLE_LOADSPLASH

#include "ctimer.h"
#include <iostream>
#include <sys/select.h>
#include <time.h>
#include <pthread.h>

#include "sharedTimerLoadSplash.h"
using namespace std;
//////////////////////////public methods//////////////////////////
CTimer::CTimer():
    m_second(0), m_microsecond(0)
{
    pthread_mutex_init(&m_mutex, NULL);
}

CTimer::CTimer(long second, long microsecond) :
    m_second(second), m_microsecond(microsecond)
{
    pthread_mutex_init(&m_mutex, NULL);
}

CTimer::~CTimer()
{
    //FixMe:  need unlock it??
    pthread_mutex_unlock(&m_mutex);

    pthread_cancel(thread_timer);
    pthread_join(thread_timer, NULL); //wait the thread stopped
}

void CTimer::setTimerInterval(long second, long microsecond)
{
    m_second = second;
    m_microsecond = microsecond;
}

void CTimer::startTimer()
{
    pthread_create(&thread_timer, NULL, OnTimer_stub, this);
}

void CTimer::stopTimer()
{
    pthread_mutex_lock(&m_mutex);
}

void CTimer::resumeTimer()
{
    pthread_mutex_unlock(&m_mutex);
}


//////////////////////////private methods//////////////////////////
void CTimer::thread_proc()
{
    while (true)
    {
        pthread_mutex_lock(&m_mutex); // allow user stop Timer  using mutex

        //FixMe: unlock mutex as soon 
        pthread_mutex_unlock(&m_mutex);
        OnTimer();
        pthread_testcancel();
        struct timeval tempval;
        tempval.tv_sec = m_second;
        tempval.tv_usec = m_microsecond;
        select(0, NULL, NULL, NULL, &tempval);
    }
}

void CTimer::OnTimer()
{
    //cout<<"Timer once..."<<endl;
    
    sharedTimerLoadSplash *splash = sharedTimerLoadSplash::sharedTimerLoadSplashInstance();
#ifdef USE_THREAD_ANIMATION
    splash->lock();
#endif
    sharedTimerLoadSplash::sharedTimerLoadSplashInstance()->checkAllObject(0, 0, 0);
#ifdef USE_THREAD_ANIMATION
    splash->unlock();
#endif
} 

#endif
