/*
 * Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2008-2009 Torch Mobile, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef GraphicsContextCairo_h
#define GraphicsContextCairo_h

#include "GraphicsContext.h"
#include <cairo.h>


namespace WebCore {

class AffineTransform;
class GraphicsContextPlatformPrivateCairo;
class ImageBuffer;
class KURL;
class Path;
class ContextShadow;
class PlatformContextCairo;

class GraphicsContextCairo : public GraphicsContext{
public:
    GraphicsContextCairo(cairo_t* cr);
    GraphicsContextCairo(PlatformContextCairo*);
    ~GraphicsContextCairo();

    void platformInit(PlatformContextCairo* platformContext);
    void platformDestroy();
    bool isCairoCanvas() {return true;}

    static void calculateShadowBufferDimensions(IntSize& shadowBufferSize, FloatRect& shadowRect, float& kernelSize, const FloatRect& sourceRect, const IntSize& shadowSize, int shadowBlur);
    AffineTransform getCTM() const;
    HDC* platformContext() const;
    void savePlatformState();
    void restorePlatformState();
    void drawRect(const IntRect& rect);
    void drawLine(const IntPoint& point1, const IntPoint& point2);
    void drawEllipse(const IntRect& rect);
    void strokeArc(const IntRect& rect, int startAngle, int angleSpan);
    void drawConvexPolygon(size_t npoints, const FloatPoint* points, bool shouldAntialias);
    void fillPath(const Path&);
    void strokePath(const Path&);
    void fillRect(const FloatRect& rect);
    void fillRect(const FloatRect& rect, const Color& color, ColorSpace colorSpace);
    void clip(const FloatRect& rect);
    void clipPath(const Path&, WindRule);
    void drawFocusRing(const Path& paths, int width, int offset, const Color& color);
    void drawFocusRing(const Vector<IntRect>& rects, int width, int /* offset */, const Color& color);
    void drawLineForText(const FloatPoint& origin, float width, bool printing);
    void drawLineForMisspellingOrBadGrammar(const IntPoint& origin, int width, bool grammar);
    FloatRect roundToDevicePixels(const FloatRect& frect, RoundingMode);
    void translate(float x, float y);
    IntPoint origin();
    void setPlatformFillColor(const Color& col, ColorSpace colorSpace);
    void setPlatformStrokeColor(const Color& col, ColorSpace colorSpace);
    void setPlatformStrokeThickness(float strokeThickness);
    void setPlatformStrokeStyle(StrokeStyle strokeStyle);
    void setURLForRect(const KURL& link, const IntRect& destRect);
    void concatCTM(const AffineTransform& transform);
    void addInnerRoundedRectClip(const IntRect& rect, int thickness);
    void clipToImageBuffer(const FloatRect& rect, const ImageBuffer* imageBuffer);
    void setPlatformShadow(const FloatSize& size, float, const Color&, ColorSpace);
    void createPlatformShadow(PassOwnPtr<ImageBuffer> buffer, const Color& shadowColor, const FloatRect& shadowRect, float kernelSize);
    void clearPlatformShadow();
    void beginTransparencyLayer(float opacity);
    void endTransparencyLayer();
    void clearRect(const FloatRect& rect);
    void strokeRect(const FloatRect& rect, float width);
    void setLineCap(LineCap lineCap);
    void setLineDash(const DashArray& dashes, float dashOffset);
    void setLineJoin(LineJoin lineJoin);
    void setMiterLimit(float miter);
    void setAlpha(float alpha);
    float getAlpha();
    void setCompositeOperation(CompositeOperator op);
    void beginPath();
    void addPath(const Path& path);
    void clip(const Path& path);
    void canvasClip(const Path& path);
    void clipOut(const Path& path);
    void rotate(float radians);
    void scale(const FloatSize& size);
    void clipOut(const IntRect& r);
    void clipOutEllipseInRect(const IntRect& r);
    void fillRoundedRect(const IntRect& r, const IntSize& topLeft, const IntSize& topRight, const IntSize& bottomLeft, const IntSize& bottomRight, const Color& color, ColorSpace colorSpace);
    void setPlatformShouldAntialias(bool enable);
    void setImageInterpolationQuality(InterpolationQuality);
    InterpolationQuality imageInterpolationQuality() const;

    //added for upgrade
    ContextShadow* contextShadow();
    void clipConvexPolygon(size_t numPoints, const FloatPoint*, bool antialias = true);
    void drawLineForTextChecking(const FloatPoint&, float width, TextCheckingLineStyle);
    void setCTM(const AffineTransform&);
    void setPlatformCompositeOperation(CompositeOperator);
private:
    GraphicsContextPlatformPrivateCairo *m_pdata;
};

} // namespace WebCore

#endif // GraphicsContextCairo_h

