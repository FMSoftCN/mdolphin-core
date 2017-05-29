/*
** $Id: ImageBufferData.h 29 2010-09-27 06:51:43Z lijiangwei $
**
** ImageBufferMg.cpp: imageBuffer implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software.
**   
** Current maintainer: lvlei 
**  
** Create date: 06/04/2010 
*/

#ifndef ImageBufferData_h
#define ImageBufferData_h

#include "config.h"

#include "minigui.h"
#include "Image.h"
#include "ImageSource.h"
#include "MDBitmap.h"
#if ENABLE(CAIRO_MG)
#include <cairo.h>
#include "PlatformContextCairo.h"
#endif

namespace WebCore {

class IntSize;

class ImageBufferData {
public:
    ImageBufferData(const IntSize&);

    RefPtr<MDBitmap> m_mdbitmap;
#if ENABLE(CAIRO_MG)
    PlatformContextCairo m_platformContext;
#endif
};

}  // namespace WebCore
#endif  // ImageBufferData_h
