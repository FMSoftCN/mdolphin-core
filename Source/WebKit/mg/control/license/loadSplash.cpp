/*
 ** $Id$
 **
 ** loadSplash.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-25 
 */

#include "mdconfig.h"

#ifdef _MD_ENABLE_WATERMARK
#include "licenseHidden.h"
#include "c_files/watermark/watermark4.dat.c"
#endif

#ifdef  _MD_ENABLE_LOADSPLASH

#include "loadSplash.h"
#include "sharedTimerLoadSplash.h"
#include "c_files/loadsplash/_loadsplash_inner_res.c"
#include "c_files/loadsplash/loadsplash01.dat.c"
#include "c_files/loadsplash/loadsplash02.dat.c"
#include "c_files/loadsplash/loadsplash03.dat.c"
#include "c_files/loadsplash/loadsplash04.dat.c"
#include "c_files/loadsplash/loadsplash05.dat.c"
#include "c_files/loadsplash/loadsplash06.dat.c"
#include "c_files/loadsplash/loadsplash07.dat.c"
#include "c_files/loadsplash/loadsplash08.dat.c"
#include "c_files/loadsplash/loadsplash09.dat.c"
#include "c_files/loadsplash/loadsplash10.dat.c"
#include "c_files/loadsplash/loadsplash11.dat.c"
#include "c_files/loadsplash/loadsplash12.dat.c"
#include "c_files/loadsplash/loadsplash13.dat.c"

unsigned int loadSplash::intervalInMS  = 100;
HDC loadSplash::pictureDC = HDC_INVALID;
int loadSplash::frameCount = 0;
int loadSplash::splashWidth = 0;
int loadSplash::splashHeight = 0;

loadSplash::loadSplash(HWND hWnd, HDC hdc) 
    :m_hwnd(hWnd)
{
    m_isRunning = false;
    init(hdc);
    m_index = 0;
    resetTimer();
}

loadSplash::~loadSplash() 
{
    finish();

    DeleteMemDC(m_bgndDC);
#if 0
    DeleteMemDC(loadSplash::pictureDC);
#endif
}

void loadSplash::initMemDCIfNeed(HDC hdc) 
{
    static bool hadInit = false;
    if (hadInit)  {
        m_bgndDC = CreateCompatibleDC(loadSplash::pictureDC);
        int oldColor = SetBrushColor(m_bgndDC, PIXEL_lightwhite);
        FillBox(m_bgndDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight);
        SetBrushColor(m_bgndDC, oldColor);
        return;
    }

    unsigned char *data;
    int len;
    BITMAP bitmap;
    data = __md_loadsplash_loadsplash_inner_res[0].data;
    len = __md_loadsplash_loadsplash_inner_res[0].data_len;
    LoadBitmapFromMem(hdc, &bitmap, data, len, "png");
    loadSplash::splashWidth = bitmap.bmWidth;
    loadSplash::splashHeight = bitmap.bmHeight;

    loadSplash::pictureDC = CreateCompatibleDCEx(hdc, bitmap.bmWidth, bitmap.bmHeight);
    //loadSplash::pictureDC = CreateMemDCFromBitmap(hdc, &bitmap);
    m_bgndDC = CreateCompatibleDC(loadSplash::pictureDC);

    loadSplash::frameCount = TABLESIZE(__md_loadsplash_loadsplash_inner_res); // 13;
    int oldColor = SetBrushColor(m_bgndDC, PIXEL_lightwhite);
    FillBox(m_bgndDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight);
    SetBrushColor(m_bgndDC, oldColor);

    UnloadBitmap(&bitmap);

    hadInit = true;
}

bool loadSplash::init(HDC hdc)
{

    //FixMe: It is uncomfortable using hdc , but using Shared MemDC to save memory
    loadSplash::initMemDCIfNeed(hdc);
    return true;
#if 0
    RECT rc; 
    SetRect(&rc, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight);
    int w, h;
    int pitch;
    Uint8 *bits = LockDC(loadSplash::pictureDC, &rc, &w, &h, &pitch);
    memcpy (bits, bitmap.bmBits, pitch* h);
    UnlockDC(loadSplash::pictureDC);
#endif


    //FillBoxWithBitmap(loadSplash::pictureDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, &bitmap);
             
    //UnloadBitmap(&bitmap);
}

void loadSplash::calcOffsetPos() 
{
   RECT rect;
   GetClientRect(m_hwnd, &rect);

#ifdef LOADSPLASH_CENTER
   m_offsetX = (RECTW(rect) - loadSplash::splashWidth)/2.0;
   if (m_offsetX < 0)
       m_offsetX = 0;

   
   m_offsetY = (RECTH(rect) - loadSplash::splashHeight)/2.0;
   if (m_offsetY < 0)
       m_offsetY = 0;
#else  // right-top
   m_offsetX = RECTW(rect) - loadSplash::splashWidth;
   m_offsetX -= 20;  //scrollWidth
   if (m_offsetX < 0)
       m_offsetX = 0;

    m_offsetY = 0;
#endif
}

void loadSplash::updateBackgroundDC(HDC backGroundDC) 
{
    BitBlt(backGroundDC, m_offsetX, m_offsetY, loadSplash::splashWidth, loadSplash::splashHeight, m_bgndDC, 0, 0, -1);
}

void loadSplash::updateDC()  
{
    unsigned char *data;
    int len;
    BITMAP bitmap;
    data = __md_loadsplash_loadsplash_inner_res[m_index].data;
    len = __md_loadsplash_loadsplash_inner_res[m_index].data_len;
    LoadBitmapFromMem(loadSplash::pictureDC, &bitmap, data, len, "png");
    m_index = (m_index + 1) % loadSplash::frameCount;
    
#if 0
    FillBoxWithBitmap(loadSplash::pictureDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, &bitmap);
#else
    RECT rc; 
    SetRect(&rc, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight);
    int w, h;
    int pitch;

    gal_pixel colorKey;
    Uint8 *bits = LockDC(loadSplash::pictureDC, &rc, &w, &h, &pitch);
    memcpy (bits, bitmap.bmBits, pitch* h);
    colorKey = *(gal_pixel *)bits;
    UnlockDC(loadSplash::pictureDC);
#endif

    SetMemDCAlpha(loadSplash::pictureDC, MEMDC_FLAG_SRCALPHA, 160);
    //SetMemDCColorKey(loadSplash::pictureDC, MEMDC_FLAG_SRCCOLORKEY, RGBA2Pixel(loadSplash::pictureDC, 0xFF, 0xFF, 0xFF, 0x00));
    SetMemDCColorKey(loadSplash::pictureDC, MEMDC_FLAG_SRCCOLORKEY, colorKey);

    UnloadBitmap(&bitmap);
}

void loadSplash::updateToWindow() 
{
    updateDC();

    HDC hdc = GetClientDC(m_hwnd);
    PCLIPRGN pClipRgn = CreateClipRgn();
    RECT rect;

    GetClipRegion(hdc, pClipRgn);
    SetRect(&rect, m_offsetX, m_offsetY, m_offsetX + loadSplash::splashWidth, m_offsetY + loadSplash::splashHeight);
    IncludeClipRect(hdc, &rect);

#ifdef PERFORMANCE_FIRST
    BitBlt(m_bgndDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, hdc, m_offsetX, m_offsetY, -1);
    BitBlt(loadSplash::pictureDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, hdc, m_offsetX, m_offsetY, -1);
#else
    HDC copyDC = CreateCompatibleDC(m_bgndDC);
    BitBlt(m_bgndDC, 0, 0, 0, 0, copyDC, 0, 0, -1);
    BitBlt(loadSplash::pictureDC, 0, 0, 0, 0, copyDC, 0, 0, -1);
    BitBlt(copyDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, hdc, m_offsetX, m_offsetY, -1);
    DeleteMemDC(copyDC);
#endif

    SelectClipRegion(hdc, pClipRgn);
    DestroyClipRgn(pClipRgn);
    ReleaseDC(hdc);
}

#if 0
void loadSplash::drawContentAndBackground(HDC backGroundDC, int offsetX, int offsetY, int maxWidth, int maxHeight)
{
    unsigned char *data;
    int len;
    BITMAP bitmap;
    data = __md_loadsplash_loadsplash_inner_res[m_index].data;
    len = __md_loadsplash_loadsplash_inner_res[m_index].data_len;
    LoadBitmapFromMem(HDC_SCREEN, &bitmap, data, len, "png");
    m_index = (m_index + 1) % loadSplash::frameCount;
    HDC imageDC = CreateMemDCFromBitmap(HDC_SCREEN,&bitmap);

    BitBlt(backGroundDC, offsetX, offsetY, loadSplash::splashWidth, loadSplash::splashHeight, loadSplash::pictureDC, 0, 0, -1);
    SetMemDCAlpha(imageDC,MEMDC_FLAG_SRCALPHA, 0x80);
    BitBlt(imageDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, loadSplash::pictureDC, 0, 0, -1);
    DeleteMemDC(imageDC);
    UnloadBitmap(&bitmap);
}

void loadSplash::updateToDC(HDC hdc, int offsetX, int offsetY, int maxWidth, int maxHeight)
{
    PCLIPRGN pClipRgn = CreateClipRgn();
    RECT rect;

    GetClipRegion(hdc, pClipRgn);
    SetRect(&rect, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight);
    IncludeClipRect(hdc, &rect):
    BitBlt(loadSplash::pictureDC, 0, 0, loadSplash::splashWidth, loadSplash::splashHeight, hdc, offsetX, offsetY, -1);
    SelectClipRegion(hdc, pClipRgn);
    DestroyClipRgn(pClipRgn);
}
#endif

BOOL loadSplash::timerFired() 
{
    //printf("loadSplash:timerFired\n");
    if (!IsWindowVisible(m_hwnd)) {
        return TRUE;
    }
    updateToWindow();
    return TRUE;
}

bool loadSplash::isRuning()
{
    return m_isRunning;
}

bool loadSplash::start()
{
#if 0
    static unsigned int t_id = 1;
    while (IsTimerInstalled(m_hwnd, t_id))
        t_id++;

    // using shared  timer ?
    if (!SetTimerEx(m_hwnd, t_id, (loadSplash::intervalInMS)/10.0, loadSplash::timerFired)) {
        fprintf(stderr, "loadSplash: SetTimerEx failed !\n");
        m_timerID = 0;
        reture false;
    }

    m_timerID = t_id;
#endif
    if (!m_isRunning) {
        sharedTimerLoadSplash* sharedTimer = sharedTimerLoadSplash::sharedTimerLoadSplashInstance();
        sharedTimer->registerObject(this);
        m_isRunning = true;
    }
    return true;
}

bool loadSplash::finish()
{
#if 0
    if (m_timerID){
        KillTimer(m_hwnd, m_timerID);
        m_timerID = 0;
    }
#endif
    if (m_isRunning) {
        sharedTimerLoadSplash* sharedTimer = sharedTimerLoadSplash::sharedTimerLoadSplashInstance();
        sharedTimer->unRegisterObject(this);
        m_isRunning = false;
    }
    return true;

}
#endif //end _MD_ENABLE_LOADSPLASH
