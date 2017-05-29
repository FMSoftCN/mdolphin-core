/*
** $Id: GraphicsContextMg.cpp 24 2010-09-26 11:21:19Z lijiangwei $
**
** GraphicsContextMg.cpp: graphics context implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei
** 
** Create date: 2010-06-01
*/

#include "minigui.h"

#include "config.h"
#include <wtf/MathExtras.h>

#include "Path.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "FloatRect.h"
#include "Font.h"
#include "FontData.h"
#include "IntRect.h"

#include "NotImplemented.h"

#if ENABLE(CAIRO_MG)
#include <cairo.h>
#include <cairo-minigui.h>
#include "ContextShadow.h"
#include "GraphicsContextCairo.h"
#endif

namespace WebCore {

#ifndef MEMDC_FLAG_SRCPIXELALPHA
#define MEMDC_FLAG_SRCPIXELALPHA MEMDC_FLAG_NONE
#endif

class GraphicsContextPlatformPrivate {
public:
    GraphicsContextPlatformPrivate();
    ~GraphicsContextPlatformPrivate();

    Vector<AffineTransform> stack;

    AffineTransform matrix;
    HDC viewdc;
    HDC context;

    Vector<HDC> layers;
#if ENABLE(CAIRO_MG)
    ContextShadow shadow;
    Vector<ContextShadow> shadowStack;
#endif
};

static inline void setPenColor(HDC dc, const Color& col)
{
    SetPenColor (dc, RGBA2Pixel(dc, (BYTE)col.red(), (BYTE)col.green(), (BYTE)col.blue(), (BYTE)col.alpha()));
}

static inline void setBrushColor(HDC dc, const Color& col)
{
    SetBrushColor (dc, RGBA2Pixel(dc, (BYTE)col.red(), (BYTE)col.green(), (BYTE)col.blue(), (BYTE)col.alpha()));
}

// A fillRect helper
static inline void fillRectSourceOver(HDC dc, const FloatRect& rect, const Color& col)
{
#ifdef SHOW_BUG4317
    setBrushColor(dc, col);
    SetRasterOperation (dc, (col.alpha() == 0xFF) ? ROP_SET : ROP_AND);
    FillBox (dc, (int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height());
    SetRasterOperation (dc, ROP_SET);
#else
    /*
     * Becareful! if with or height <=0 , the FillBox think the width or height is the whole DC
     */
    if ((int)rect.width()<=0 || (int)rect.height() <= 0)
        return;

    if (col.alpha() == 0xFF) {
//        printf("---- fill rect=%d,%d,%d,%d, color=%d,%d,%d\n",(int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height(), (BYTE)col.red(),(BYTE)col.green(), (BYTE)col.blue());
        setBrushColor (dc, col);
        SetRasterOperation (dc, ROP_SET);
        FillBox (dc, (int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height());
    }
    else {
        int width = (int)rect.width();
        int x = (int)rect.x();
        int y = (int)rect.y();
        HDC memdc = CreateCompatibleDCEx (dc, width, 1);
        setBrushColor (memdc, col);
        FillBox (memdc, 0, 0, width, 1);
        SetMemDCAlpha (memdc, MEMDC_FLAG_SRCPIXELALPHA, 255);
        for (int i = 0; i < (int)rect.height(); i++) {
            BitBlt (memdc, 0, 0, width, 1, dc, x, y++, 0);
        }
        DeleteMemDC (memdc);
    }
#endif
}

GraphicsContextPlatformPrivate::GraphicsContextPlatformPrivate()
    :  viewdc(0)
    , context(0)
{
}

GraphicsContextPlatformPrivate::~GraphicsContextPlatformPrivate()
{
}

void GraphicsContext::platformInit(PlatformGraphicsContext* context)
{
    m_data = new GraphicsContextPlatformPrivate;
    if (context) {
        m_data->viewdc = *context;
        m_data->context = m_data->viewdc;
    }
}

void GraphicsContext::platformDestroy()
{
    delete m_data;
}

HDC *GraphicsContext::platformContext() const
{
    return &m_data->context;
}

#if ENABLE(CAIRO_MG) 
GraphicsContextCairo* GraphicsContext::newCairoContext(GraphicsContext *context)
{
    if (context && !context->isCairoCanvas()) {
        HDC hdc = *(HDC* )context->platformContext();
        RefPtr<cairo_surface_t> mgSurface = cairo_minigui_surface_create(hdc);
        RefPtr<cairo_t> mgCr = adoptRef(cairo_create(mgSurface.get()));
        GraphicsContextCairo *cairoContext = new GraphicsContextCairo(mgCr.get());
        if (cairoContext) {
            if (context->fillGradient()) {
                cairoContext->setFillGradient(context->fillGradient());
            } else if (context->fillPattern()) {
                cairoContext->setFillPattern(context->fillPattern());
            } else {
                cairoContext->setFillColor(context->fillColor(), ColorSpaceDeviceRGB);
            }

            cairoContext->setCTM(context->getCTM());
        }
        return cairoContext;
    }
    return NULL;
}

void GraphicsContext::deleteCairoContext(GraphicsContextCairo *context)
{
    if (context && context->isCairoCanvas())
        delete context;
}
#endif

void GraphicsContext::savePlatformState()
{
    SaveDC (m_data->context);
    m_data->stack.append(m_data->matrix);
#if ENABLE(CAIRO_MG)
    m_data->shadowStack.append(m_data->shadow);
#endif
}

void GraphicsContext::restorePlatformState()
{
#if ENABLE(CAIRO_MG)
    if (m_data->shadowStack.isEmpty())
        m_data->shadow = ContextShadow();
    else {
        m_data->shadow = m_data->shadowStack.last();
        m_data->shadowStack.removeLast();
    }
#endif
    m_data->matrix = m_data->stack.last();
    m_data->stack.removeLast();
    RestoreDC (m_data->context, -1);
}

// Draws a filled rectangle with a stroked border.
void GraphicsContext::drawRect(const IntRect& r)
{
	if (paintingDisabled())
		return;	

    IntRect rect = m_data->matrix.mapRect(r);

	HDC hdc = m_data->context;
    if (fillColor().alpha())
        fillRectSourceOver(hdc, rect, fillColor());

    if (strokeStyle() != NoStroke) {
        setPenColor(hdc, strokeColor());
        FloatRect r(rect);
        r.inflate(-.5f);
        SetPenWidth (hdc, static_cast<int>(strokeThickness()));
	    Rectangle(hdc, (int)r.x(), (int)r.y(), (int)(r.x()+r.width()), (int)(r.y()+r.height()));
    }
}

// This is only used to draw borders.
void GraphicsContext::drawLine(const IntPoint& po1, const IntPoint& po2)
{
    if (paintingDisabled())
        return;	

    IntPoint point1 = m_data->matrix.mapPoint(po1);
    IntPoint point2 = m_data->matrix.mapPoint(po2);

	HDC hdc = m_data->context;

    StrokeStyle penStyle = strokeStyle();
    if (penStyle == NoStroke)
        return;

    SaveDC(hdc);   

    float width = strokeThickness();
    if (width < 1)
        width = 1;

    FloatPoint p1 = point1;
    FloatPoint p2 = point2;
    bool isVerticalLine = (p1.x() == p2.x());
    
    adjustLineToPixelBoundaries(p1, p2, width, penStyle);
    SetPenWidth (hdc, static_cast<int>(width));

    int patWidth = 0;
    switch (penStyle) {
    case NoStroke:
    case SolidStroke:
        SetPenType( hdc, PT_SOLID); 
        break;
    case DottedStroke:
        patWidth = static_cast<int>(width);
        SetPenType( hdc, PT_ON_OFF_DASH); 
        break;
    case DashedStroke:
        patWidth = 3*static_cast<int>(width);
        SetPenType( hdc, PT_ON_OFF_DASH); 
        break;
    }

    setPenColor (hdc, strokeColor());
//to set antialias to false

    if (patWidth) {
        // Do a rect fill of our endpoints.  This ensures we always have the
        // appearance of being a border.  We then draw the actual dotted/dashed line.
        if (isVerticalLine) {
            fillRectSourceOver(hdc, FloatRect(p1.x()-width/2, p1.y()-width, width, width), strokeColor());
            fillRectSourceOver(hdc, FloatRect(p2.x()-width/2, p2.y(), width, width), strokeColor());
        } else {
            fillRectSourceOver(hdc, FloatRect(p1.x()-width, p1.y()-width/2, width, width), strokeColor());
            fillRectSourceOver(hdc, FloatRect(p2.x(), p2.y()-width/2, width, width), strokeColor());
        }
        
        // Example: 80 pixels with a width of 30 pixels.
        // Remainder is 20.  The maximum pixels of line we could paint
        // will be 50 pixels.
        int distance = (isVerticalLine ? (point2.y() - point1.y()) : (point2.x() - point1.x())) - 2*static_cast<int>(width);
        int remainder = distance%patWidth;
        int coverage = distance-remainder;
        int numSegments = coverage/patWidth;

        float patternOffset = 0;
        // Special case 1px dotted borders for speed.
        if (patWidth == 1)
            patternOffset = 1.0;
        else {
            bool evenNumberOfSegments = numSegments%2 == 0;
            if (remainder)
                evenNumberOfSegments = !evenNumberOfSegments;
            if (evenNumberOfSegments) {
                if (remainder) {
                    patternOffset += patWidth - remainder;
                    patternOffset += remainder/2;
                }
                else
                    patternOffset = patWidth/2;
            }
            else if (!evenNumberOfSegments) {
                if (remainder)
                    patternOffset = (patWidth - remainder)/2;
            }
        }

       unsigned char pattern[2] = {(unsigned char)patWidth, (unsigned char)patWidth};
       SetPenDashes( hdc, (int)patternOffset, pattern, 2);
    }

	LineEx(hdc, (int)p1.x(), (int)p1.y(), (int)p2.x(), (int)p2.y());
    RestoreDC(hdc, -1);
}

// This method is only used to draw the little circles used in lists.
void GraphicsContext::drawEllipse(const IntRect& r)
{
    if (paintingDisabled())
        return;	

    IntRect rect = m_data->matrix.mapRect(r);

	HDC hdc = m_data->context;
    SaveDC(hdc);

    if (fillColor().alpha()) {
        setBrushColor(hdc, fillColor()); //FIXME: should be alpha blend
        FillEllipse (hdc, rect.x()+rect.width()/2, rect.y()+rect.height()/2, rect.width()/2, rect.height()/2);
    }

    if (strokeStyle() != NoStroke) {
        setPenColor(hdc, strokeColor());
        SetPenWidth (hdc, strokeThickness());
	    Ellipse(hdc, rect.x()+rect.width()/2, rect.y()+rect.height()/2, rect.width()/2, rect.height()/2);
    }
    RestoreDC(hdc, -1);
}

void GraphicsContext::strokeArc(const IntRect& r, int startAngle, int angleSpan)
{
    if (paintingDisabled())
        return;	

    IntRect rc = m_data->matrix.mapRect(r);

	HDC hdc = m_data->context;
    SaveDC(hdc);   
    if (strokeStyle() != NoStroke) {
        setPenColor(hdc, strokeColor());
        SetPenWidth(hdc, strokeThickness());
        ArcEx(hdc, rc.x(), rc.y(), rc.width(), rc.height(), startAngle * 64, angleSpan * 64);
    }
    RestoreDC(hdc, -1);
}

void GraphicsContext::drawConvexPolygon(size_t numPoints, const FloatPoint* points, bool shouldAntialias)
{
    if (paintingDisabled())
        return;

    if (numPoints <= 1)
        return;

	HDC hdc = m_data->context;

    POINT *sp = new POINT[numPoints];
    if (!sp) 
        return;

    SaveDC(hdc);   
    
    for (size_t i = 0; i < numPoints; i++) {
        double x, y;
        m_data->matrix.map(points[i].x(), points[i].y(), x, y);
        sp[i].x = (int)lround(x); 
        sp[i].y = (int)lround(y); 
    }

    if (fillColor().alpha()) {
        setBrushColor(hdc, fillColor());
        FillPolygon (hdc, sp, (int)numPoints);
    }

    if (strokeStyle() != NoStroke) {
        setPenColor(hdc, strokeColor());
        SetPenWidth(hdc, static_cast<int>(strokeThickness()));
        PolyLineEx (hdc, sp, static_cast<int>(numPoints));
    }

    RestoreDC(hdc, -1);

    delete [] sp;
}

void GraphicsContext::fillRect(const FloatRect& r)
{
    if (paintingDisabled())
        return;	

	fillRect(r, m_state.fillColor, ColorSpaceDeviceRGB);
}

void GraphicsContext::fillRect(const FloatRect& r, const Color& color, ColorSpace)
{
    if (paintingDisabled())
        return;	

    FloatRect rect = m_data->matrix.mapRect(r);
#if ENABLE(CAIRO_MG)
    if (hasShadow()) {
        GraphicsContextCairo* context = newCairoContext(this);
        m_data->shadow.drawRectShadow(context, enclosingIntRect(rect));
        deleteCairoContext(context);
    }
#endif

    if (color.alpha()) 
        fillRectSourceOver(m_data->context, rect, color);
}

void GraphicsContext::clip(const FloatRect& r)
{
    if (paintingDisabled())
        return;	

    IntRect rect  = enclosingIntRect(r);

    HDC hdc = m_data->context;
    RECT rc = m_data->matrix.mapRect(rect);
    ClipRectIntersect(hdc, &rc);
}

void GraphicsContext::drawLineForText(const FloatPoint& origin, float width, bool printing)
{
    if (paintingDisabled())
        return;
    
    StrokeStyle savedStrokeStyle = strokeStyle();
    setStrokeStyle(SolidStroke);

    //IntPoint origin = point;
    //origin.setY(point.y()-1);
    FloatPoint endPoint = origin + FloatSize(width, 0);
    
    // FIXME: Loss of precision here. Might consider rounding.
    drawLine(IntPoint(origin.x(), origin.y()), IntPoint(endPoint.x(), endPoint.y()));
    
    setStrokeStyle(savedStrokeStyle);
}

void GraphicsContext::translate(float x, float y)
{
    if (paintingDisabled())
        return;

    m_data->matrix.translate((float)x, (float)y);
}


void GraphicsContext::strokeRect(const FloatRect& rect, float width) 
{
//This function paints a line of the specified width along the path of a rectangle. 
//The line surrounds the center of the path, with half of the total width on either side. 
//As a side effect when you call this function, Quartz clears the current path.
	HDC hdc = m_data->context;
	unsigned int oldwidth = SetPenWidth(hdc, width);
    IntRect intrect((int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height());
	drawRect(intrect);
	(void)SetPenWidth(hdc, oldwidth);
}

void GraphicsContext::setPlatformStrokeThickness(float width) 
{
    if (paintingDisabled())
        return;

	HDC hdc = m_data->context;
	(void)SetPenWidth(hdc, width);
}

void GraphicsContext::setLineCap(LineCap cap) 
{
	if (paintingDisabled())
        return;

	HDC hdc = m_data->context;
	(void)SetPenCapStyle(hdc, cap);
}

void GraphicsContext::setLineJoin(LineJoin join) 
{
    if (paintingDisabled())
        return;	

	HDC hdc = m_data->context;
	(void)SetPenJoinStyle(hdc, join);
}

void GraphicsContext::setAlpha(float opacity) 
{
    if (paintingDisabled())
        return;	

	HDC hdc = m_data->context;
    SetMemDCAlpha(hdc, MEMDC_FLAG_SRCALPHA, (BYTE)(opacity*255));
   
}

void GraphicsContext::clip(const Path& path)
{
    if (paintingDisabled())
        return;	

    clip(enclosingIntRect(path.boundingRect()));
}

void GraphicsContext::scale(const FloatSize& size) 
{
    m_data->matrix.scale (size.width(), size.height());
}

FloatRect GraphicsContext::roundToDevicePixels(const FloatRect& r, RoundingMode)
{
    FloatRect frect = m_data->matrix.mapRect(r);
    FloatRect result;
    POINT p;
    p.x = (int)frect.x();
    p.y = (int)frect.y();
    HDC hdc = m_data->context;
    LPtoDP (hdc, &p);
    p.x = (int)roundf((float)p.x);
    p.y = (int)roundf((float)p.y);
    DPtoLP (hdc, &p);
    result.setX((float)p.x);
    result.setY((float)p.y);
    p.x = (int)(frect.x()+frect.width());
    p.y = (int)(frect.y()+frect.height());
    LPtoDP (hdc, &p);
    p.x = (int)roundf((float)p.x);
    p.y = (int)roundf((float)p.y);
    DPtoLP (hdc, &p);
    result.setWidth((float)(p.x - result.x()));
    result.setHeight((float)(p.y - result.y()));
    return result; 
}

void GraphicsContext::setPlatformFillColor(const Color& fill, ColorSpace)
{
    if (paintingDisabled())
        return;
    
    HDC hdc = m_data->context;
    setBrushColor(hdc, fill);
    
}

void GraphicsContext::setPlatformStrokeColor(const Color& color, ColorSpace)
{
    if (paintingDisabled())
        return;
    
    HDC hdc = m_data->context;
    setPenColor(hdc, color);
}

AffineTransform GraphicsContext::getCTM() const
{
    return AffineTransform(m_data->matrix.a(), m_data->matrix.b(), m_data->matrix.c(),
                           m_data->matrix.d(), m_data->matrix.e(), m_data->matrix.f());
}

void GraphicsContext::setCTM(const AffineTransform & matrix)
{
    m_data->matrix = matrix;
}

//not implements
void GraphicsContext::strokePath(const Path& path)
{
    notImplemented();
}

void GraphicsContext::fillPath (const Path& path)
{
    notImplemented();
}

void GraphicsContext::clipOut(const IntRect& rect)
{
    HDC hdc = m_data->context;
    RECT rc = m_data->matrix.mapRect(rect);
    ExcludeClipRect(hdc, &rc);
}

static void fillRoundCorner(HDC hdc, RECT& clipRect, RECT &rectWin, 
        const IntSize& size, AffineTransform& matrix)
{
    IntRect cornerRect(0, 0, size.width(), size.height());
    cornerRect = matrix.mapRect(cornerRect);

    int rw = cornerRect.width();
    int rh = cornerRect.height();
    rw = rw > (RECTW(rectWin)>>1) ? (RECTW(rectWin)>>1) : rw;
    rh = rh > (RECTH(rectWin)>>1) ? (RECTH(rectWin)>>1) : rh;

    SaveDC(hdc);
    ClipRectIntersect(hdc, &clipRect);
    RoundRect(hdc, rectWin.left, rectWin.top, rectWin.right, rectWin.bottom, rw, rh);
    RestoreDC(hdc, -1);
} 

void GraphicsContext::fillRoundedRect(const IntRect& r, const IntSize& topLeft,
        const IntSize& topRight, const IntSize& bottomLeft, 
        const IntSize& bottomRight, const Color& color, ColorSpace space)
{
#if ENABLE(CAIRO_MG)
    GraphicsContextCairo* context = newCairoContext(this);
    context->fillRoundedRect(r, topLeft, topRight, bottomLeft, bottomRight, color, space);
    deleteCairoContext(context);
#else
    IntRect dstRect = r;
    dstRect = m_data->matrix.mapRect(dstRect);
    RECT rectWin = dstRect;

    IntPoint centerPoint = IntPoint(rectWin.left + (RECTW(rectWin)>>1), rectWin.top + (RECTH(rectWin)>>1));
    int centerX = centerPoint.x();
    int centerY = centerPoint.y();

    HDC hdc = m_data->context;
    setBrushColor (hdc, color);

    // Draw top left half
    RECT clipRect(rectWin);
    clipRect.right = centerX;
    clipRect.bottom = centerY;
    fillRoundCorner(hdc, clipRect, rectWin, topLeft, m_data->matrix);

    // Draw top right
    clipRect = rectWin;
    clipRect.left = centerX;
    clipRect.bottom = centerY;
    fillRoundCorner(hdc, clipRect, rectWin, topRight, m_data->matrix);

     // Draw bottom left
    clipRect = rectWin;
    clipRect.right = centerX;
    clipRect.top = centerY;
    fillRoundCorner(hdc, clipRect, rectWin, bottomLeft, m_data->matrix);

    // Draw bottom right
    clipRect = rectWin;
    clipRect.left = centerX;
    clipRect.top = centerY;
    fillRoundCorner(hdc, clipRect, rectWin, bottomRight, m_data->matrix);
#endif

}

void GraphicsContext::setURLForRect(const KURL& link, const IntRect& destRect)
{
    notImplemented();
}

void GraphicsContext::rotate(float) 
{
    notImplemented();
}

void GraphicsContext::addInnerRoundedRectClip(const IntRect& rect, int thickness)
{
    notImplemented();
}

void GraphicsContext::setPlatformShadow(FloatSize const& size, float blur, Color const& color, ColorSpace)
{
#if ENABLE(CAIRO_MG)
    // Cairo doesn't support shadows natively, they are drawn manually in the draw* functions
    if (m_state.shadowsIgnoreTransforms) {
        // Meaning that this graphics context is associated with a CanvasRenderingContext
        // We flip the height since CG and HTML5 Canvas have opposite Y axis
        m_state.shadowOffset = FloatSize(size.width(), -size.height());
        m_data->shadow = ContextShadow(color, blur, FloatSize(size.width(), -size.height()));
    } else
        m_data->shadow = ContextShadow(color, blur, FloatSize(size.width(), size.height()));

    m_data->shadow.setShadowsIgnoreTransforms(m_state.shadowsIgnoreTransforms);
#endif
}

#if ENABLE(CAIRO_MG)
ContextShadow* GraphicsContext::contextShadow()
{
    return &m_data->shadow;
}
#endif

void GraphicsContext::clearPlatformShadow() 
{
#if ENABLE(CAIRO_MG)
    m_data->shadow.clear();
#endif
}

void GraphicsContext::beginTransparencyLayer(float opacity) 
{ 
    if (paintingDisabled())
        return;

    HDC memdc = CreateCompatibleDC(m_data->viewdc);
    m_data->layers.append(memdc);

    SetMemDCAlpha(memdc, MEMDC_FLAG_SRCALPHA, opacity*255);
    m_data->context = memdc;
}

void GraphicsContext::endTransparencyLayer() 
{
    if (paintingDisabled())
        return;

    HDC memdc = m_data->layers.last();
    m_data->layers.removeLast();

    if (!m_data->layers.isEmpty()) {
        m_data->context = m_data->layers.last();
    }
    else {
        m_data->context = m_data->viewdc;
    }

    BitBlt(memdc, 0, 0, 0, 0, m_data->context, 0, 0, 0);
    DeleteCompatibleDC(memdc);
}

void GraphicsContext::clearRect(const FloatRect&) 
{
    notImplemented();
}

void GraphicsContext::setMiterLimit(float) 
{
    notImplemented();
}

void GraphicsContext::canvasClip(const Path&)
{
    notImplemented();
}

void GraphicsContext::clipOut(const Path&)
{
    notImplemented();
}

void GraphicsContext::concatCTM(const AffineTransform&)
{
    notImplemented();
}

void GraphicsContext::drawFocusRing(const Vector<IntRect>& rects, int width, int loffset, const Color& color)
{
    if (paintingDisabled() || !color.isValid())
        return;

    unsigned rectCount = rects.size();

    if (!rects.size())
        return;

    HDC hdc = m_data->context;
    int radius = (width - 1) / 2;
    int offset = radius + loffset;

    IntRect finalFocusRect;
    for (unsigned i = 0; i < rectCount; i++) {
        IntRect focusRect = rects[i];
        focusRect.inflate(offset);
        finalFocusRect.unite(focusRect);
    }

    IntRect rc = m_data->matrix.mapRect(finalFocusRect);
#if !ENABLE(FOCUSRING_TV)  
    RECT rect(rc);
    GetDefaultWindowElementRenderer()->draw_focus_frame(hdc, 
            &rect, color.rgb());
#else
    const Color col(0xFF, 0x00, 0x00);
    POINT sp[5];
    sp[0].x = rc.x();
    sp[0].y = rc.y();
    sp[1].x = rc.x() + rc.width();
    sp[1].y = rc.y();
    sp[2].x = rc.x() + rc.width();
    sp[2].y = rc.y() + rc.height();
    sp[3].x = rc.x();
    sp[3].y = rc.y() + rc.height();
    sp[4].x = rc.x();
    sp[4].y = rc.y();

    int oldPen = SetPenType(hdc, PT_SOLID); 
    int oldWidth = SetPenWidth(hdc, 3);
    setPenColor(hdc, col);
    PolyLineEx(hdc, sp, 5);
    SetPenType(hdc, oldPen);
    SetPenWidth(hdc, oldWidth);
#endif
}

void GraphicsContext::drawFocusRing(const Path& path, int width, int /* offset */, const Color& color)
{
    notImplemented();
}


void GraphicsContext::setImageInterpolationQuality(InterpolationQuality)
{
    notImplemented();
}

InterpolationQuality GraphicsContext::imageInterpolationQuality() const
{
    return InterpolationDefault;
}

void GraphicsContext::setPlatformShouldAntialias(bool b)
{
    notImplemented();
}

void GraphicsContext::clipPath(const Path& path, WindRule clipRule)
{
    notImplemented();
}

void GraphicsContext::setLineDash(const DashArray&, float dashOffset)
{
    notImplemented();
}

void GraphicsContext::clipConvexPolygon(size_t numPoints, const FloatPoint* points, bool antialiased)
{
    notImplemented();
}

void GraphicsContext::drawLineForTextChecking(const FloatPoint& origin, float width, TextCheckingLineStyle style)
{
    notImplemented();
}

void GraphicsContext::setPlatformCompositeOperation(CompositeOperator op)
{
    //TODO
    if (paintingDisabled())
        return;	

	HDC hdc = m_data->context;

    switch (op)
    {
        case CompositeClear:
            //return CAIRO_OPERATOR_CLEAR;
            break;
        case CompositeCopy:
            //return CAIRO_OPERATOR_SOURCE;
            break;
        case CompositeSourceOver:
            SetRasterOperation (hdc, ROP_SET);
            break;
        case CompositeSourceIn:
            //return CAIRO_OPERATOR_IN;
            break;
        case CompositeSourceOut:
            //return CAIRO_OPERATOR_OUT;
            break;
        case CompositeSourceAtop:
            //return CAIRO_OPERATOR_ATOP;
            break;
        case CompositeDestinationOver:
            //return CAIRO_OPERATOR_DEST_OVER;
            break;
        case CompositeDestinationIn:
            //return CAIRO_OPERATOR_DEST_IN;
            break;
        case CompositeDestinationOut:
            //return CAIRO_OPERATOR_DEST_OUT;
            break;
        case CompositeDestinationAtop:
            //return CAIRO_OPERATOR_DEST_ATOP;
            break;
        case CompositeXOR:
            SetRasterOperation (hdc, ROP_XOR);
            break;
        case CompositePlusDarker:
            SetRasterOperation (hdc, ROP_SET);
            break;
        case CompositeHighlight:
            SetRasterOperation (hdc, ROP_SET);
            break;
        case CompositePlusLighter:
            SetRasterOperation (hdc, ROP_SET);
            break;
        default:
            SetRasterOperation (hdc, ROP_SET);
            break;
    }
}

} //end nameSpace WebCore
