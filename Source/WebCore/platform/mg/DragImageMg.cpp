/*
 ** $Id: DragImageMg.cpp 53 2010-06-04 08:11:26Z xwyan $
 **
 ** DragImageMg.cpp: The platform implementation for DragImage.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "minigui.h"

#include "config.h"
#include "DragImage.h"

#include "CachedImage.h"
#include "GraphicsContext.h"
#include "Image.h"

#include "NotImplemented.h"

namespace WebCore {

//on MDolphin platform, typedef PBITMAP DragImageRef
IntSize dragImageSize(DragImageRef image)
{
    if (!image)
        return IntSize();
    
    PBITMAP pBitmap = (PBITMAP)image;

    return IntSize(pBitmap->bmWidth, pBitmap->bmHeight);
}

void deleteDragImage(DragImageRef image)
{
    if (image)
        UnloadBitmap((PBITMAP)image);
}

DragImageRef scaleDragImage(DragImageRef image, FloatSize scale)
{
    notImplemented();
    return image;
}

DragImageRef dissolveDragImageToFraction(DragImageRef image, float)
{
    notImplemented();
    return image;
}
        
DragImageRef createDragImageFromImage(Image* image)
{
    notImplemented();
    return 0;
}

DragImageRef createDragImageIconForCachedImage(CachedImage* image)
{
    if (!image)
        return 0;

    notImplemented();
    return 0;
}
    
}
