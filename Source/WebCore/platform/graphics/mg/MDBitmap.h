/*
** $Id: MDBitmap.h 698 2011-06-23 02:34:55Z xwyan $
**
** MDBitmap.cpp: MDBitmap definition file.
**
** Copyright (C) 2003 ~ 2011 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software.
**   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2011-05-20 
*/
#ifndef MDBitmap_h
#define MDBitmap_h

#include "config.h"

#include "minigui.h"
#include <wtf/RefPtr.h>
#include <wtf/RefCounted.h>
#if ENABLE(CAIRO_MG)
#include <cairo.h>
#endif

namespace WebCore {

class MDBitmap: public MYBITMAP, public RefCounted<MDBitmap> {
public:
    static PassRefPtr<MDBitmap> create(int width, int height, bool hasAlpha = false, unsigned char* imgBits = 0);
    static PassRefPtr<MDBitmap> create(PassRefPtr<MDBitmap>);
    ~MDBitmap();

    bool hasAlpha() { return flags & MYBMP_ALPHA; }
    int stride() { return pitch; }
    unsigned char* bytes() { return bits; }
    int width() { return w; }
    int height() { return h; }
    int bmpDepth() { return depth; }
    int bmpSize() { return size; }

    HDC createMemDC(bool useSoftSurface = false, int offx = 0, int offy = 0, int w = 0, int h = 0);

#if ENABLE(CAIRO_MG)
    cairo_surface_t* surface();
#endif

private:
    MDBitmap(int width, int height, bool hasAlpha = false, unsigned char* imgBits = 0);
    MDBitmap(const MDBitmap&);
#if ENABLE(CAIRO_MG)
    RefPtr<cairo_surface_t> m_surface;
#endif
    bool m_allocBits;
};

}  // namespace WebCore
#endif  // MDBitmap_h
