/*
 ** $Id$
 **
 ** waterMark.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-25 
 */

#ifndef waterMark_H
#define waterMark_H

#ifdef _MD_ENABLE_WATERMARK


class waterMark {
public:
    static HDC waterMarkDC;
    static bool initWaterMark(HDC hdc);
    static void fillWatmark(HDC hdc, int x, int top);
    static bool waterMarkSize(int &width, int &height);
    static gal_pixel getColorKey(HDC hdc);
private:
    waterMark() {};
    ~waterMark() {};

};

#endif

#endif
