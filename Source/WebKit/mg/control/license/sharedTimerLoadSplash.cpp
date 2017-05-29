/*
 ** $Id$
 **
 ** sharedTimerLoadSplash.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-26 
 */

#include "mdconfig.h"
#ifdef  _MD_ENABLE_LOADSPLASH

#include "sharedTimerLoadSplash.h"

#ifdef USE_THREAD_ANIMATION
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef USE_THREAD_ANIMATION
    pthread_t sg_mdolphin_pid;
#endif

unsigned int sharedTimerLoadSplash::sharedTimerTicks = 40; // 40ms

#ifdef USE_THREAD_ANIMATION
/*
static int _traceback_signal = -1;

static void tb_sig_handler_usr1(int sig, siginfo_t *info, void *ucontext)
{

}

void initSigHandlerUsr1() 
{
    struct sigaction act;
    _traceback_signal = SIGUSR1;
    act.sa_sigaction = tb_sig_handler_usr1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(_traceback_signal, &act, NULL);
}
*/
#endif

sharedTimerLoadSplash::sharedTimerLoadSplash()
{
    m_timerID = 0;
#ifdef USE_THREAD_ANIMATION
    m_ctimer = new CTimer(0, 40*1000);
    sg_mdolphin_pid = pthread_self();
    m_ctimer->startTimer(); 
    pthread_mutex_init(&m_mutex, NULL);
#endif
}

 sharedTimerLoadSplash::~sharedTimerLoadSplash()
{
#ifdef USE_THREAD_ANIMATION
    delete m_ctimer;
    pthread_mutex_destroy(&m_mutex);
#endif
}

sharedTimerLoadSplash*  sharedTimerLoadSplash::sharedTimerLoadSplashInstance()
{
#if 0
    static sharedTimerLoadSplash *sharedInstance = NULL;
    if (!sharedInstance) {
        printf("55555555555555555555555555555555555\n");
        sharedInstance = new sharedTimerLoadSplash();
        //sharedInstance->registerTimer();
    }
    return sharedInstance;
#else
    static sharedTimerLoadSplash sharedInstance;
    return &sharedInstance;
#endif

}

bool sharedTimerLoadSplash::registerTimer()
{
#ifndef USE_THREAD_ANIMATION
    static unsigned int t_id = 1;
    while (IsTimerInstalled(HWND_NULL, t_id))
        t_id++;

    // using shared  timer ?
    //if (!SetTimerEx(HWND_NULL, t_id, (sharedTimerLoadSplash::sharedTimerTicks)/10.0, sharedTimerLoadSplash::checkAllObject)) {
    if (!SetTimerEx(HWND_NULL, t_id, 4, sharedTimerLoadSplash::checkAllObject)) {
        fprintf(stderr, "loadSplash: SetTimerEx failed !\n");
        m_timerID = 0;
        return false;
    }

    m_timerID = t_id;
#endif

    return true;
}

bool sharedTimerLoadSplash::unRegisterTimer()
{
#ifndef USE_THREAD_ANIMATION
    if (m_timerID){
        KillTimer(HWND_NULL, m_timerID);
        m_timerID = 0;
    }
#endif
    return true;
}


bool sharedTimerLoadSplash::registerObject(loadSplash *splash)
{
    if (!splash)
        return false;

#ifdef USE_THREAD_ANIMATION
    if (!m_loadSplashQueue.size())
        m_ctimer->resumeTimer(); 
#else
    if (!m_loadSplashQueue.size())
        registerTimer();
#endif

    //add loadSplash object
#ifdef USE_THREAD_ANIMATION
    lock();
#endif

    m_loadSplashQueue.add(splash);

#ifdef USE_THREAD_ANIMATION
    unlock();
#endif
    return true;
}

bool sharedTimerLoadSplash::unRegisterObject(loadSplash *splash)
{
    if (!splash)
        return false;

    //remove loadSplash object
#ifdef USE_THREAD_ANIMATION
    lock();
#endif

    m_loadSplashQueue.remove(splash);

#ifdef USE_THREAD_ANIMATION
    unlock();
#endif

#ifdef USE_THREAD_ANIMATION
    if (!m_loadSplashQueue.size())
        m_ctimer->stopTimer(); 
#else
    if (!m_loadSplashQueue.size())
        unRegisterTimer();
#endif
    return true;
}


BOOL sharedTimerLoadSplash::checkAllObject(HWND, int, DWORD)
{
    sharedTimerLoadSplash* sharedTimer = sharedTimerLoadSplash::sharedTimerLoadSplashInstance();
    HashSet<loadSplash*>* queue = sharedTimer->loadSplashQueue();
    HashSet<loadSplash*>::iterator end = (*queue).end();
    for (HashSet<loadSplash*>::iterator itr = (*queue).begin(); itr != end; ++itr) {
        (*itr)->decTimerCunt(sharedTimerLoadSplash::sharedTimerTicks); 
        if ((*itr)->timerCount() <= 0) {
            (*itr)->timerFired();
            (*itr)->resetTimer();
        }
        
    }

    return TRUE;
}

#ifdef USE_THREAD_ANIMATION
void sharedTimerLoadSplash::lock() 
{
    pthread_mutex_lock(&m_mutex);
}

void sharedTimerLoadSplash::unlock() 
{
    pthread_mutex_unlock(&m_mutex);
}
#endif

#endif  //_MD_ENABLE_LOADSPLASH
