/* 
** $Id: FontMg.cpp 737 2011-07-11 08:47:48Z xwyan $
** 
** FontMg.cpp: The platform implementation for Font.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: XiaoweiYan
**
** Create date: 2010-06-01
*/

#include "minigui.h"

#include "config.h"
#include "Font.h"

#include "GlyphBuffer.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "SimpleFontData.h"

#include "Gradient.h"
#include "ImageBuffer.h"
#include "Pattern.h"
#define SYNTHETIC_OBLIQUE_ANGLE 14

#if ENABLE(CAIRO_MG)
#include "PlatformContextCairo.h" 
#include "GraphicsContextCairo.h"
#include "CairoUtilities.h"
#include <cairo.h>
#include <cairo-minigui.h>
#endif

namespace WebCore {

bool Font::canReturnFallbackFontsForComplexText()
{
    return false;
}

void Font::drawComplexText(GraphicsContext* context, const TextRun& run, const FloatPoint& point, int from, int to) const
{
    return drawSimpleText(context, run, point, from, to);
}
void Font::drawEmphasisMarksForComplexText(GraphicsContext* /* context */, const TextRun& /* run */, 
        const AtomicString& /* mark */, const FloatPoint& /* point */, int /* from */, int /* to */) const
{
}
float Font::floatWidthForComplexText(const TextRun& run, HashSet<const SimpleFontData*>* fallbackFonts, GlyphOverflow* glyphOverflow) const
{
    return floatWidthForSimpleText(run, 0, fallbackFonts, glyphOverflow);
}

int Font::offsetForPositionForComplexText(const TextRun& run, float position, bool includePartialGlyphs) const
{
    return offsetForPositionForSimpleText(run, position, includePartialGlyphs);
}

FloatRect Font::selectionRectForComplexText(const TextRun& run, const FloatPoint& point, int h, int from, int to) const
{
    return selectionRectForSimpleText(run, point, h, from, to);
}

#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
static void prepareContextForGlyphDrawing(cairo_t* context, const SimpleFontData* font, const FloatPoint& point)
{
    static const float syntheticObliqueSkew = -tanf(14 * acosf(0) / 90);
    if (font->platformData().scaledFont())
        cairo_set_scaled_font(context, font->platformData().scaledFont());

    if (font->platformData().syntheticOblique()) {
        cairo_matrix_t mat = {1, 0, syntheticObliqueSkew, 1, point.x(), point.y()};
        cairo_transform(context, &mat);
    } else
        cairo_translate(context, point.x(), point.y());
}

static void drawGlyphsToContext(cairo_t* context, const SimpleFontData* font, GlyphBufferGlyph* glyphs, int numGlyphs)
{
    cairo_show_glyphs(context, (cairo_glyph_t* )glyphs, numGlyphs);
    if (font->syntheticBoldOffset()) {
        // We could use cairo_save/cairo_restore here, but two translations are likely faster.
        cairo_translate(context, font->syntheticBoldOffset(), 0);
        cairo_show_glyphs(context, (cairo_glyph_t* )glyphs, numGlyphs);
        cairo_translate(context, -font->syntheticBoldOffset(), 0);
    }
}

static void drawGlyphsShadow(GraphicsContext* graphicsContext, const FloatPoint& point, 
        const SimpleFontData* font, GlyphBufferGlyph* glyphs, int numGlyphs, ContextShadow *mgShadow)
{
    ContextShadow* shadow;
    if (mgShadow && mgShadow->m_type != ContextShadow::NoShadow) {
        shadow = mgShadow;
    }
    else 
        shadow = ((GraphicsContextCairo* )graphicsContext)->contextShadow();

    ASSERT(shadow);

    if (!(graphicsContext->textDrawingMode() & TextModeFill) || shadow->m_type == ContextShadow::NoShadow)
        return;

    if (!shadow->mustUseContextShadow(graphicsContext)) {
        // Optimize non-blurry shadows, by just drawing text without the ContextShadow.
        cairo_t* context = ((PlatformContextCairo* )graphicsContext->platformContext())->cr();
        cairo_save(context);
        cairo_translate(context, shadow->m_offset.width(), shadow->m_offset.height());
        setSourceRGBAFromColor(context, shadow->m_color);
        prepareContextForGlyphDrawing(context, font, point);
        cairo_show_glyphs(context, (cairo_glyph_t* )glyphs, numGlyphs);
        cairo_restore(context);
        return;
    }

    cairo_text_extents_t extents;
    cairo_scaled_font_glyph_extents(font->platformData().scaledFont(), (cairo_glyph_t* )glyphs, numGlyphs, &extents);
    FloatRect fontExtentsRect(point.x(), point.y() - extents.height, extents.width, extents.height);
    cairo_t* shadowContext = shadow->beginShadowLayer(graphicsContext, fontExtentsRect);
    if (shadowContext) {
        prepareContextForGlyphDrawing(shadowContext, font, point);
        drawGlyphsToContext(shadowContext, font, glyphs, numGlyphs);
        shadow->endShadowLayer(graphicsContext);
    }
}

static inline void cairoDrawGlyphs(GraphicsContext* context, const SimpleFontData* font,
        const GlyphBuffer& glyphBuffer, int from, int numGlyphs, const FloatPoint& point, 
        ContextShadow *shadow)
{
    cairo_glyph_t* glyphs = (cairo_glyph_t* )new char[numGlyphs*sizeof(cairo_glyph_t)];

    float offset = 0.0f;
    for (int i = 0; i < numGlyphs; i++) {
        glyphs[i].index = *(glyphBuffer.glyphs(from+i));
        glyphs[i].x = offset;
        glyphs[i].y = 0.0f;
        offset += glyphBuffer.advanceAt(from + i);
    }

    drawGlyphsShadow(context, point, font, (GlyphBufferGlyph* )glyphs, numGlyphs, shadow);

    cairo_t* cr = ((PlatformContextCairo* )context->platformContext())->cr();
    cairo_save(cr);

    prepareContextForGlyphDrawing(cr, font, point);

    if (context->textDrawingMode() & TextModeFill) {
        if (context->fillGradient()) {

            cairo_set_source(cr, context->fillGradient()->platformGradient());
            if (context->getAlpha() < 1.0f) {
                cairo_push_group(cr);
                cairo_paint_with_alpha(cr, context->getAlpha());
                cairo_pop_group_to_source(cr);
            }
        } else if (context->fillPattern()) {
            AffineTransform affine;
            cairo_pattern_t* pattern = context->fillPattern()->createPlatformPattern(affine);
            cairo_set_source(cr, pattern);
            if (context->getAlpha() < 1.0f) {
                cairo_push_group(cr);
                cairo_paint_with_alpha(cr, context->getAlpha());
                cairo_pop_group_to_source(cr);
            }
            cairo_pattern_destroy(pattern);
        } else {
            float red, green, blue, alpha;
            context->fillColor().getRGBA(red, green, blue, alpha);
            cairo_set_source_rgba(cr, red, green, blue, alpha * context->getAlpha());
        }
        drawGlyphsToContext(cr, font, (GlyphBufferGlyph* )glyphs, numGlyphs);
    }

    // Prevent running into a long computation within cairo. If the stroke width is
    // twice the size of the width of the text we will not ask cairo to stroke
    // the text as even one single stroke would cover the full wdth of the text.
    //  See https://bugs.webkit.org/show_bug.cgi?id=33759.
    if (context->textDrawingMode() & TextModeStroke && context->strokeThickness() < 2 * offset) {
        if (context->strokeGradient()) {
            cairo_set_source(cr, context->strokeGradient()->platformGradient());
            if (context->getAlpha() < 1.0f) {
                cairo_push_group(cr);
                cairo_paint_with_alpha(cr, context->getAlpha());
                cairo_pop_group_to_source(cr);
            }
        } else if (context->strokePattern()) {
            AffineTransform affine;
            cairo_pattern_t* pattern = context->strokePattern()->createPlatformPattern(affine);
            cairo_set_source(cr, pattern);
            if (context->getAlpha() < 1.0f) {
                cairo_push_group(cr);
                cairo_paint_with_alpha(cr, context->getAlpha());
                cairo_pop_group_to_source(cr);
            }
            cairo_pattern_destroy(pattern);
        } else {
            float red, green, blue, alpha;
            context->strokeColor().getRGBA(red, green, blue, alpha);
            cairo_set_source_rgba(cr, red, green, blue, alpha * context->getAlpha());
        }
        cairo_glyph_path(cr, (cairo_glyph_t* )glyphs, numGlyphs);
        cairo_set_line_width(cr, context->strokeThickness());
        cairo_stroke(cr);
    }

    cairo_restore(cr);
    delete[] glyphs;

    return;
}
#else
static inline void cairoDrawGlyphs(GraphicsContext* context, const SimpleFontData* font,
        const GlyphBuffer& glyphBuffer, int from, int numGlyphs, const FloatPoint& point,
        ContextShadow *shadow)
{
}
#endif

#endif

void Font::drawGlyphs(GraphicsContext* context, const SimpleFontData* font, 
        const GlyphBuffer& glyphBuffer, int from, int numGlyphs, 
        const FloatPoint& point) const
{
#if ENABLE(CAIRO_MG)
    if (context->isCairoCanvas()) {
        if (font->platformData().useCairo())
            cairoDrawGlyphs(context, font, glyphBuffer, from, numGlyphs, point, NULL);
        return;
    } 
    else if (font->platformData().useCairo()) {
        GraphicsContextCairo* cairoContext = context->newCairoContext(context);
        cairoDrawGlyphs(cairoContext, font, glyphBuffer, from, numGlyphs, point, context->contextShadow());
        context->deleteCairoContext(cairoContext);
        return;
    }
#endif

    /*font css style size is zero, we should return.*/
    if (!font->platformData().size())
        return;

    HDC hdc = *(context->platformContext());
    if (!hdc)
        return;

    Color color = context->fillColor();

    PLOGFONT curFont = font->platformData().hfont();
    PLOGFONT oldFont = SelectFont(hdc, curFont);
    gal_pixel fgColor, oldFgColor;

    fgColor = RGBA2Pixel(hdc, (BYTE)color.red(), 
            (BYTE)color.green(), (BYTE)color.blue(), (BYTE)color.alpha());

    oldFgColor = SetTextColor(hdc, fgColor);
    SetTextAlign (hdc, TA_LEFT | TA_BASELINE);
    SetBkMode (hdc, BM_TRANSPARENT);

    FloatPoint xPoint = context->getCTM().mapPoint(point);
    int x = (int)ceil(xPoint.x());
    int y = (int)ceil(xPoint.y());

    for (int i = 0; i < numGlyphs; i++) {

        Glyph32 glyph = GetGlyphValue(curFont, 
                (const char*)glyphBuffer.glyphs(from + i), 2/*sizeof(UChar)*/, NULL, 0);
        DrawGlyph(hdc, x, y, glyph, NULL, NULL);
        x += glyphBuffer.advanceAt(from + i);
    };

    SelectFont(hdc, oldFont);
    SetTextColor(hdc, oldFgColor);
}

bool Font::canExpandAroundIdeographsInComplexText()
{
        return false;
}

}
