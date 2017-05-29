/*
 ** $Id$
 **
 ** loadSplash.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-25 
 */

#ifndef loadSplash_h
#define loadSplash_h


#ifdef  _MD_ENABLE_LOADSPLASH
#include "licenseHidden.h"
#include "minigui.h"


class MD_LOCAL loadSplash {
public:
#if 0
static loadSplash* loadSplashInstance() 
{
    static loadSplash splashInstance;
    static bool hadInit = false;
    if (!hadInit) {
        splashInstance.init();
    }
    return &splashInstance;
}
#endif

    loadSplash(HWND hWnd, HDC hdc);
    ~loadSplash();
    bool init(HDC hdc);
    void reset(void) {m_index = 0;}
    bool start();
    bool finish();
    int offsetX() { return m_offsetX; }
    int offsetY() { return m_offsetY; }
    int width() { return loadSplash::splashWidth; }
    int height() { return loadSplash::splashHeight;}
#if 0
    void drawContentAndBackground(HDC backGroundDC, int offsetX, int offsetY, int maxWidth = 0, int maxHeight = 0);
    void updateToDC(HDC hdc, int offsetX, int offsetY, int maxWidth = 0, int maxHeight = 0);
#endif
    static unsigned int intervalInMS;
    BOOL timerFired();
    bool isRuning();
    void calcOffsetPos();
    void updateBackgroundDC(HDC backGroundDC);

    //timer
    void resetTimer() { m_timerCount = loadSplash::intervalInMS; }
    void decTimerCunt(int value) { m_timerCount -= value; }
    int timerCount() { return m_timerCount; }

    static HDC pictureDC;
    static int frameCount;
    static int splashWidth;
    static int splashHeight;

    void updateToWindow();
private:
    void initMemDCIfNeed(HDC hdc);
    void updateDC();

    bool m_isRunning;
    // about  animation
    int  m_index;

    // about  dc
    int m_offsetX;
    int m_offsetY;
    HDC m_bgndDC;



    HWND m_hwnd;

    //DWORD m_timerID;
    //int  m_timerIntervalMS;
    int  m_timerCount;
};

#endif //end _MD_ENABLE_LOADSPLASH


#endif // end  loadSplash_h
