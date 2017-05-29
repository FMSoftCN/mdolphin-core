/*
** $Id$
**
** ImageDecoderMg.cpp: The ImageDecode implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software.
**   
** Current maintainer: lvlei 
**  
** Create date: 06/03/2010 
*/

#include "config.h"
#include "minigui.h"

#include "ImageDecoder.h"
#include "ImageBufferData.h"

namespace WebCore {

NativeImagePtr ImageFrame::asNewNativeImage() const
{
    unsigned char* bits =reinterpret_cast<unsigned char*>(const_cast<PixelData*>(m_bytes)); 
    return MDBitmap::create(width(), height(), hasAlpha(), bits);
}

} // namespace WebCore
