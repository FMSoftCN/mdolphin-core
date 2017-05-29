/*
** $Id: ImageMg.cpp 24 2010-09-26 11:21:19Z lijiangwei $
**
** ImageMg.cpp: image rending implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei
** 
** Create date: 06/03/2010
*/

#include "minigui.h"
#include "config.h"
#include "Image.h"

#include "AffineTransform.h"
#include "ImageObserver.h"
#include "BitmapImage.h"
#include "wtf/MathExtras.h"
#include "PlatformString.h"
#include "GraphicsContext.h"
#include "NotImplemented.h"
#include "SharedBuffer.h"
#include "ImageBuffer.h"
#include "MDResourceManager.h"

#if ENABLE(CAIRO_MG)
#include "CairoUtilities.h"
#include "GraphicsContextCairo.h"
#include "cairo.h"
#include "cairo-minigui.h"
#include "ContextShadow.h"
#endif

// This function loads resources from mdolphin
PassRefPtr<WebCore::SharedBuffer> loadResourceIntoBuffer(const char*);

namespace WebCore {

#ifndef MEMDC_FLAG_SRCPIXELALPHA
#define MEMDC_FLAG_SRCPIXELALPHA MEMDC_FLAG_NONE
#endif

bool FrameData::clear(bool clearMetadata)
{
    if (clearMetadata)
        m_haveMetadata = false;

    if (m_frame) {
        m_frame = 0;
        return true;
    }

    return false;
}


BitmapImage::BitmapImage(NativeImagePtr imagePtr, ImageObserver* observer)
    : Image(observer)
    , m_currentFrame(0)
    , m_frames(0)
    , m_frameTimer(0)
    , m_repetitionCount(cAnimationNone)
    , m_repetitionCountStatus(Unknown)
    , m_repetitionsComplete(0)
    , m_isSolidColor(false)
    , m_checkedForSolidColor(false)
    , m_animationFinished(true)
    , m_allDataReceived(true)
    , m_haveSize(true)
    , m_sizeAvailable(true)
    , m_decodedSize(0)
    , m_decodedPropertiesSize(0)
    , m_haveFrameCount(true)
    , m_frameCount(1)
{
    initPlatformData();
    
    RefPtr<MDBitmap> mdBitmap = imagePtr;
    int width = mdBitmap->width();
    int height = mdBitmap->height();
    m_decodedSize = mdBitmap->bmpSize();
    m_size = IntSize(width, height);

    m_frames.grow(1);
    m_frames[0].m_frame = imagePtr;
    m_frames[0].m_hasAlpha = mdBitmap->hasAlpha();
    m_frames[0].m_haveMetadata = true;
    checkForSolidColor();
}

void BitmapImage::initPlatformData()
{
}

void BitmapImage::invalidatePlatformData()
{
}

PassRefPtr<SharedBuffer> loadResourceIntoBuffer(const char* name)
{
	MDResourceManager*res=MDResourceManager::getSharedInstance();
	return res->loadResource(name);
}

PassRefPtr<Image> Image::loadPlatformResource(const char *name)
{
    RefPtr<SharedBuffer> buffer = loadResourceIntoBuffer(name);
    RefPtr<BitmapImage> img = BitmapImage::create();
    img->setData(buffer.release(), true);
    return img.release();
}

void Image::drawPattern(GraphicsContext* context, const FloatRect& tileRect, const AffineTransform& patternTransform,
                        const FloatPoint& p, ColorSpace, CompositeOperator op, const FloatRect& dst)
{
    RefPtr<MDBitmap> mdBitmap = nativeImageForCurrentFrame();
    if (!mdBitmap || !mdBitmap->bytes())
        return;

#if ENABLE(CAIRO_MG)
    if (context->isCairoCanvas()) {
        cairo_surface_t* surface = mdBitmap->surface();
        if (!surface) // If it's too early we won't have an image yet.
            return;

        cairo_t* cr = (cairo_t* )(((PlatformContextCairo* )context->platformContext())->cr());
        drawPatternToCairoContext(cr, surface, size(), tileRect, patternTransform, p, toCairoOperator(op), dst);

        if (imageObserver())
            imageObserver()->didDraw(this);

        return;
    }
#endif

    HDC origDC = mdBitmap->createMemDC(true, (int)tileRect.x(), (int)tileRect.y(), 
            (unsigned)tileRect.width(), (unsigned)tileRect.height());

    if (origDC != HDC_INVALID) {
        FloatRect destRect = context->getCTM().mapRect(dst);
        FloatPoint phase = context->getCTM().mapPoint(p);
        FloatRect stRect = context->getCTM().mapRect(tileRect);
        FloatRect zoomRect =  patternTransform.mapRect(tileRect);

        HDC hdc = *(context->platformContext()); 
        HDC scaledDC = 0;

        int leftwidth = (unsigned int)tileRect.width();
        int leftheight = (unsigned int)tileRect.height();
        int origx = (int)roundf(phase.x());
        int origy = (int)roundf(phase.y());
        int scaledwidth, scaledheight;

        if ((stRect.width() != zoomRect.width()) || (stRect.height() != zoomRect.height())) {
            scaledwidth = (int)roundf(zoomRect.width());
            scaledheight = (int)roundf(zoomRect.height());
            if (!scaledwidth) scaledwidth = 1;
            if (!scaledheight) scaledheight = 1;

            scaledDC = CreateCompatibleDCEx(origDC, scaledwidth, scaledheight);
            if (scaledDC == HDC_INVALID) {
                DeleteMemDC(origDC);
                return;
            }

            StretchBlt(origDC, 0, 0, leftwidth, leftheight, scaledDC, 0, 0, 
                    scaledwidth, scaledheight, 0);
        }
        else {
            scaledwidth = (unsigned int)tileRect.width();
            scaledheight = (unsigned int)tileRect.height();
            scaledDC = origDC;
        }
    
        if (mdBitmap->hasAlpha())
            SetMemDCAlpha(scaledDC, MEMDC_FLAG_SRCPIXELALPHA, 0);

        int ox = (int)roundf(destRect.x());
        int oy = (int)roundf(destRect.y());

        int drawy = (oy - origy) % (scaledheight);
        if (drawy < 0)
            drawy = -drawy;

        int drawx = (ox - origx) % (scaledwidth);
        if (drawx < 0)
            drawx = -drawx;

        int endx = ox +  (int)roundf(destRect.width());
        int endy = oy +  (int)roundf(destRect.height());
        
        int x, y;
        for (y = oy; y < endy;) {
            int offy;

            leftheight = endy - y;
            if (y == oy) {
                int drawh = scaledheight - drawy;
                offy =  drawy;
                leftheight = (leftheight > drawh) ? drawh : leftheight;
            }
            else {
                leftheight = (leftheight > scaledheight) ? scaledheight : leftheight;
                offy = 0;
            }

            for (x = ox; x < endx; ) {
                int offx;

                leftwidth = endx - x;
                if (x == ox) {
                    int draww = scaledwidth - drawx;
                    offx = drawx;
                    leftwidth = (leftwidth > draww) ? draww : leftwidth;
                }
                else {
                    leftwidth = (leftwidth > scaledwidth) ? scaledwidth : leftwidth;
                    offx = 0;
                }

                BitBlt (scaledDC, offx, offy, leftwidth, leftheight, hdc, x, y, 0);
                x += leftwidth;
            }

            y += leftheight;
        }

        DeleteMemDC(origDC);
        if (origDC != scaledDC)
            DeleteMemDC(scaledDC);
    }
}

void BitmapImage::draw(GraphicsContext* context, const FloatRect& dst, 
                const FloatRect& srcRect, ColorSpace styleColorSpace, CompositeOperator op)
{
    if (dst.width() == 0.0f || dst.height() == 0.0f ||
            srcRect.width() == 0.0f || srcRect.height() == 0.0f)
        return;

#if ENABLE(CAIRO_MG)
    if (context->isCairoCanvas()) {
        startAnimation();

        RefPtr<MDBitmap> pmdbmp = frameAtIndex(m_currentFrame);
        if (!pmdbmp)
            return;

        cairo_surface_t* image = pmdbmp->surface();
        if (!image) // If it's too early we won't have an image yet.
            return;
        
        FloatRect dstRect(dst);
        if (mayFillWithSolidColor()) {
            fillWithSolidColor((GraphicsContextCairo* )context, dstRect, solidColor(), styleColorSpace, op);
            return;
        }

        IntSize selfSize = size();
        
        cairo_t* cr = (cairo_t* )(((PlatformContextCairo* )context->platformContext())->cr());
        context->save();

        // Set the compositing operation.
        if (op == CompositeSourceOver && !frameHasAlphaAtIndex(m_currentFrame))
            context->setCompositeOperation(CompositeCopy);
        else
            context->setCompositeOperation(op);

        // If we're drawing a sub portion of the image or scaling then create
        // a pattern transformation on the image and draw the transformed pattern.
        // Test using example site at http://www.meyerweb.com/eric/css/edge/complexspiral/demo.html
        cairo_pattern_t* pattern = cairo_pattern_create_for_surface(image);

        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);

        float scaleX = srcRect.width() / dstRect.width();
        float scaleY = srcRect.height() / dstRect.height();
        cairo_matrix_t matrix = { scaleX, 0, 0, scaleY, srcRect.x(), srcRect.y() };
        cairo_pattern_set_matrix(pattern, &matrix);

        // Draw the shadow
        ContextShadow* shadow = ((GraphicsContextCairo* )context)->contextShadow();
        ASSERT(shadow);
        if (shadow->m_type != ContextShadow::NoShadow) {
            cairo_t* shadowContext = shadow->beginShadowLayer(context, dstRect);
            if (shadowContext) {
                cairo_translate(shadowContext, dstRect.x(), dstRect.y());
                cairo_set_source(shadowContext, pattern);
                cairo_rectangle(shadowContext, 0, 0, dstRect.width(), dstRect.height());
                cairo_fill(shadowContext);
                shadow->endShadowLayer(context);
            }
        }

        // Draw the image.
        cairo_translate(cr, dstRect.x(), dstRect.y());
        cairo_set_source(cr, pattern);
        cairo_pattern_destroy(pattern);
        cairo_rectangle(cr, 0, 0, dstRect.width(), dstRect.height());
        cairo_clip(cr);

        cairo_paint_with_alpha(cr, context->getAlpha());

        context->restore();

        if (imageObserver())
            imageObserver()->didDraw(this);
        return;
    }
#endif

    RefPtr<MDBitmap> mdBitmap = nativeImageForCurrentFrame();

    if (mdBitmap && mdBitmap->bytes()) {
        HDC hdc = *(context->platformContext()); 

#if ENABLE(HIGHQUALITYZOOM)
        SetBitmapScalerType(hdc, BITMAP_SCALER_BILINEAR);
#endif
        if (mayFillWithSolidColor()) {
            fillWithSolidColor(context, dst, solidColor(), styleColorSpace, op);
            return;
        }

        int x = 0, y = 0;
        int dx = 0, dy = 0;
        FloatRect dstRect = context->getCTM().mapRect(dst);
        float scaleX = dstRect.width() / srcRect.width();
        float scaleY = dstRect.height() / srcRect.height();

        int width = (int)roundf(srcRect.width());
        int height = (int)roundf(srcRect.height());
        x = (int)roundf(srcRect.x());
        y = (int)roundf(srcRect.y());
        dx = (int)roundf(dstRect.x());
        dy = (int)roundf(dstRect.y());

        HDC memdc = mdBitmap->createMemDC(true);
        if (memdc != HDC_INVALID) {
            if (mdBitmap->hasAlpha())
                SetMemDCAlpha(memdc, MEMDC_FLAG_SRCPIXELALPHA, 0);

            if (scaleX == 1.0f && scaleY == 1.0f) {
                BitBlt(memdc, x, y, width, height, hdc, dx, dy, 0);
            } else {
                int scaledwidth =  (int)roundf(dstRect.width());
                int scaledheight =  (int)roundf(dstRect.height());

                if (scaledwidth > 0 && scaledheight > 0) {
                    HDC scaledDc = CreateCompatibleDCEx(memdc, scaledwidth, scaledheight);
                    if (scaledDc != HDC_INVALID) {
                        StretchBlt(memdc, x, y, width, height, scaledDc, 0, 0, scaledwidth, scaledheight, 0);
                        BitBlt(scaledDc, 0, 0, scaledwidth, scaledheight, hdc, dx, dy, 0); 
                        DeleteMemDC(scaledDc);
                    }
                    else
                        printf ("create compatible dc for scaled failure!\n");
                }
            }
            DeleteMemDC(memdc);
        }
        else {
            printf("%s-%d: image display error! \n", __FILE__, __LINE__);
        }

        startAnimation();
#if ENABLE(HIGHQUALITYZOOM)
        SetBitmapScalerType(hdc, BITMAP_SCALER_DDA);
#endif
    }
}

void BitmapImage::checkForSolidColor()
{
    m_isSolidColor = false;
    m_checkedForSolidColor = true;

    if (frameCount() > 1)
        return;

    RefPtr<MDBitmap> mdBitmap = frameAtIndex(0);
    if (!mdBitmap || mdBitmap->width() != 1 || mdBitmap->height() != 1)
        return;

    m_isSolidColor = true;
    //FIXEME Get BitmapImage's RGB color at (0,0) to m_solidColor
    unsigned* pixelColor = reinterpret_cast<unsigned*>(mdBitmap->bytes());
    m_solidColor = colorFromPremultipliedARGB(*pixelColor);
}

}
