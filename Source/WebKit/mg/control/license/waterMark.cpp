/*
 ** $Id$
 **
 ** waterMark.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-30 
 */

#include "mdconfig.h"
#ifdef _MD_ENABLE_WATERMARK

#include "minigui.h"
#include "waterMark.h"
#include "rc4Decode.h"

#include <string.h>


#include "licenseHidden.h"

#include "c_files/watermark/watermark5.dat.c"
#include "c_files/watermark/_watermark_inner_res.c"
#include "c_files/watermark/watermark_key.c"

HDC waterMark::waterMarkDC = HDC_INVALID;

bool waterMark::initWaterMark(HDC hdc)
{
    if (waterMark::waterMarkDC != HDC_INVALID)
        return true;
    int i = 0;
    unsigned char *data;
    int len;
    rc4Decode* decode = rc4Decode::rc4DecodeInstance();
    BITMAP  waterBitmaps[TABLESIZE(__md_watermark_watermark_inner_res)];
   

    int dcWidth = 0;
    int dcHeight = 0;

    for (i=0; i< (int)TABLESIZE(__md_watermark_watermark_inner_res); i++) {
        rc4Key  key(watermark_crypto_key, sizeof(watermark_crypto_key));
        data =__md_watermark_watermark_inner_res[i].data;
        len = __md_watermark_watermark_inner_res[i].data_len;
        len = decode->decode(key, data, len);

        LoadBitmapFromMem(hdc, &waterBitmaps[i], data, len, "png");
        dcWidth += waterBitmaps[i].bmWidth;
    }

    dcHeight = waterBitmaps[0].bmHeight;

    waterMark::waterMarkDC = CreateCompatibleDCEx(hdc, dcWidth, dcHeight);
    
#if 0
    RECT rc; 
    SetRect(&rc, 0, 0, dcWidth, dcHeight);
    int w, h;
    int pitch;

    Uint8 *bits = LockDC(waterMark::waterMarkDC, &rc, &w, &h, &pitch);
    memset (bits, 0, pitch* h);
    UnlockDC(waterMark::waterMarkDC);
#endif

    //SetMemDCAlpha(waterMark::waterMarkDC, MEMDC_FLAG_SRCPIXELALPHA, 0);

    int offsetXPos = 0;
    for (i=0; i< (int)TABLESIZE(__md_watermark_watermark_inner_res); i++) {
       //waterBitmaps[i].bmType |=  BMP_TYPE_COLORKEY;
       //waterBitmaps[i].bmColorKey = RGBA2Pixel(waterMark::waterMarkDC, 0xFF, 0xFF, 0xFF, 0x00);
       FillBoxWithBitmap(waterMark::waterMarkDC, offsetXPos, 0, waterBitmaps[i].bmWidth, dcHeight, &waterBitmaps[i]); 
       offsetXPos += waterBitmaps[i].bmWidth;

       UnloadBitmap(&waterBitmaps[i]);
    }


    SetMemDCAlpha(waterMark::waterMarkDC, MEMDC_FLAG_SRCALPHA, 0xC0);
    gal_pixel colorKey = waterMark::getColorKey(waterMark::waterMarkDC);
    SetMemDCColorKey(waterMark::waterMarkDC, MEMDC_FLAG_SRCCOLORKEY, colorKey);
    return true;
}

gal_pixel waterMark::getColorKey(HDC hdc)
{

    RECT rc; 
    SetRect(&rc, 0, 0, 4, 1);
    int pitch;

    gal_pixel colorKey;
    Uint8 *bits = LockDC(waterMark::waterMarkDC, &rc, NULL, NULL, &pitch);
    colorKey =  *(gal_pixel *)bits;
    UnlockDC(waterMark::waterMarkDC);
    
    return  colorKey;

}

bool waterMark::waterMarkSize(int &width, int &height)
{
    if ( waterMark::waterMarkDC == HDC_INVALID)
        return false;

    width = GetGDCapability(waterMark::waterMarkDC, GDCAP_HPIXEL);
    height = GetGDCapability(waterMark::waterMarkDC, GDCAP_VPIXEL);

    return true;
}

void waterMark::fillWatmark(HDC memdc, int x, int top)
{
    BitBlt(waterMark::waterMarkDC, 0, 0, 0, 0, memdc, x, top, 0);
}

#endif
