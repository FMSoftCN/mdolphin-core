/*
 ** $Id: MDBitmap.cpp 698 2011-06-23 02:34:55Z xwyan $
 **
 ** MDBitmap.cpp: 
 **
 ** Copyright (C) 2003 ~ 2011 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2011-05-20 
 */

#include "MDBitmap.h"

#include "GraphicsContext.h"
#if ENABLE(CAIRO_MG)
#include "GraphicsContextCairo.h"
#endif
#include "Image.h"
#include "ImageBuffer.h"
#include "ImageBufferData.h"

namespace WebCore {

PassRefPtr<MDBitmap> MDBitmap::create(int width, int height, bool hasAlpha, unsigned char* imgBits)
{
    RefPtr<MDBitmap> resultantBitmap = adoptRef(new MDBitmap(width, height, hasAlpha, imgBits));
    if (resultantBitmap && !resultantBitmap->bytes())
        return 0;
    return resultantBitmap.release();
}

PassRefPtr<MDBitmap> MDBitmap::create(PassRefPtr<MDBitmap> other)
{
    RefPtr<MDBitmap> resultantBitmap = adoptRef(new MDBitmap(other->width(), other->height(), other->hasAlpha()));
    if (resultantBitmap && !resultantBitmap->bytes())
        return 0;

    //copy bitmap data
    memcpy(resultantBitmap->bytes(), other->bytes(), other->bmpSize());
    return resultantBitmap.release();
}

MDBitmap::MDBitmap(int width, int height, bool hasAlpha, unsigned char* imgBits)
#if ENABLE(CAIRO_MG)
    : m_surface(NULL)
#endif
{
    flags = MYBMP_TYPE_RGB;
    flags |= MYBMP_FLOW_DOWN | MYBMP_RGBSIZE_4;
    if (hasAlpha)
        flags |= MYBMP_ALPHA;

    frames = 1;
    w = width;
    h = height;

#if ENABLE(LOWPIXELIMAGESUPPORT)
    depth = 16; 
    pitch = w * 2;
#else
    depth = 32; 
    pitch = w * 4;
#endif
    size = pitch * h;
    if (!imgBits) {
        bits = (unsigned char *)calloc(1, size);
        m_allocBits  = true;
    }
    else {
        bits = imgBits;
        m_allocBits  = false;
    }
}

HDC MDBitmap::createMemDC(bool useSoftSurface, int offx, int offy, int width, int height)
{
    if (!bits)
        return HDC_INVALID;

    int Rmask = 0, Gmask = 0, Bmask = 0, Amask = 0;
    unsigned char* curBits = bits;

    //if not use default value, offset bits
    if (offx != 0 || offy != 0)
        curBits += offy * pitch + offx*((depth+7)/8);

    if (width == 0)
        width = w;
    if (height == 0)
        height = h;

#if ENABLE(LOWPIXELIMAGESUPPORT)
    //depth is 16
    Rmask = 0x0F00;
    Gmask = 0x00F0;
    Bmask = 0x000F;
    Amask = 0xF000;
#else
    //depth is 32, color format should be compatible with backingStoreDC.
    Rmask = 0x00FF0000;
    Gmask = 0x0000FF00;
    Bmask = 0x000000FF;
    if (hasAlpha())
        Amask = 0xFF000000;
    else
        Amask = 0x00000000;
#endif
    return CreateMemDCEx (width, height, depth, 
            useSoftSurface ? MEMDC_FLAG_SWSURFACE : MEMDC_FLAG_HWSURFACE, 
            Rmask, Gmask, Bmask, Amask, curBits, pitch);
}

#if ENABLE(CAIRO_MG)
cairo_surface_t* MDBitmap::surface()
{
    if (m_surface == NULL) {
        cairo_format_t format = CAIRO_FORMAT_ARGB32;
        int stride = cairo_format_stride_for_width (format, w);

        m_surface = cairo_image_surface_create_for_data(bits, format, w, h, stride);
    }
    return m_surface.get();
}
#endif

MDBitmap::~MDBitmap()
{
    if (m_allocBits) {
        free(bits);
    }
}

} //namespace WebCore
