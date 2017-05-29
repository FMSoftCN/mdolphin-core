/*
 * =====================================================================================
 *
 *       Filename:  ctime.h
 *       Copyright (C) 2004 ~ 2007 Feynman Software. 
 *    Description:  i
 *
 *        Version:  1.0
 *        Created:  2009年10月16日 12时14分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lvlei (mn), lvlei@minigui.com
 *        Company:  Feynman Software
 *
 * =====================================================================================
 */


//////////////////////////////////////////////////////////////////////////
// This class provide a timer to finish some works.
// Call setTimerInterval() to set the timer_interval. Call StartTimer()
// to enable it and call StopTimer() to stop it.
// The work you want to do should be written on OnTimer
// function.
//////////////////////////////////////////////////////////////////////////

#ifndef CTIMER_H_
#define CTIMER_H_

#ifdef  _MD_ENABLE_LOADSPLASH
#include <pthread.h>
#include <sys/time.h>

class CTimer
{
private:
    pthread_t thread_timer;
    long m_second, m_microsecond;
    static void *OnTimer_stub(void *p)
    {
        (static_cast<CTimer*>(p))->thread_proc();
        return NULL;
    }
    void thread_proc();
    void OnTimer();
public:
    CTimer();
    CTimer(long second, long microsecond);
    virtual ~CTimer();
    void setTimerInterval(long second,long microsecond);
    void startTimer();
    void stopTimer();
    void resumeTimer();
private:
    pthread_mutex_t m_mutex;

};
#endif /* CTIMER_H_ */ 

#endif
