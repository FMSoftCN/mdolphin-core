/* 
** $Id: SimpleFontDataMg.cpp 702 2011-06-27 09:26:14Z xwyan $
** 
** SimpleFontDataMg.cpp: The platform implementation for SimpleFontData. 
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
#include "SimpleFontData.h"

#include "Font.h"
#include "FontCache.h"
#include "FloatRect.h"
#include "FontDescription.h"
#include <wtf/MathExtras.h>

#include "GlyphBuffer.h"

#if ENABLE(CAIRO_MG)
#include "cairo-ft.h"
#include "cairo.h"
#include "fontconfig/fcfreetype.h"
#endif

namespace WebCore {

void SimpleFontData::platformInit()
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        cairo_font_extents_t font_extents;
        cairo_text_extents_t text_extents;
        cairo_scaled_font_extents(m_platformData.scaledFont(), &font_extents);

        m_fontMetrics.setAscent(font_extents.ascent);
        m_fontMetrics.setDescent(font_extents.descent);

        // There seems to be some rounding error in cairo (or in how we
        // use cairo) with some fonts, like DejaVu Sans Mono, which makes
        // cairo report a height smaller than ascent + descent, which is
        // wrong and confuses WebCore's layout system. Workaround this
        // while we figure out what's going on.
        float lineSpacing = font_extents.height;
        if (lineSpacing < font_extents.ascent + font_extents.descent)
            lineSpacing = font_extents.ascent + font_extents.descent;

        m_fontMetrics.setLineSpacing(lroundf(lineSpacing));
        m_fontMetrics.setLineGap(lineSpacing - font_extents.ascent - font_extents.descent);

        cairo_scaled_font_text_extents(m_platformData.scaledFont(), "x", &text_extents);
        m_fontMetrics.setXHeight(text_extents.height);

        cairo_scaled_font_text_extents(m_platformData.scaledFont(), " ", &text_extents);
        m_spaceWidth = static_cast<float>(text_extents.x_advance);

        m_syntheticBoldOffset = m_platformData.syntheticBold() ? 1.0f : 0.f;

        return;
    }
#endif
    FONTMETRICS fontMetrics;
    PLOGFONT    oldFont;
    SIZE        size;

    GetFontMetrics(m_platformData.hfont(), &fontMetrics);

    int lineSpacing = fontMetrics.font_height;
    int ascent = fontMetrics.ascent;
    int descent = ABS(fontMetrics.descent);
    
    if (lineSpacing < ascent + descent)
        lineSpacing = ascent + descent;
    
    int lineGap = lineSpacing - ascent - descent;

    oldFont = SelectFont(HDC_SCREEN, m_platformData.hfont());

    m_fontMetrics.setAscent(ascent);
    m_fontMetrics.setDescent(descent);
    m_fontMetrics.setLineGap(lineGap);
    m_fontMetrics.setLineSpacing(lineSpacing);
    
    //get xHeight
    GetTextExtent(HDC_SCREEN, "x", 1, &size);
    int xHeight;
    if (size.cy > 0)
        xHeight = size.cy;
    else
        xHeight = ascent * 0.56f;  // Best guess for xHeight for non-Truetype fonts.

    m_fontMetrics.setXHeight(xHeight);

    // FIXME
    m_fontMetrics.setUnitsPerEm(1);
    
    SelectFont(HDC_SCREEN, oldFont); 
}

void SimpleFontData::platformCharWidthInit()
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        m_avgCharWidth = 0.f;
        m_maxCharWidth = 0.f;
        initCharWidths();
        return;
    }
#endif
    /* Note:maximum and average width from MiniGUI cann't renderer 'input' 
     * element with size correctly, recalculate it here.*/
    m_avgCharWidth = 0.f;
    m_maxCharWidth = 0.f;
    initCharWidths();
}

void SimpleFontData::platformDestroy()
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        return;
    }
#endif
}

SimpleFontData* SimpleFontData::scaledFontData(const FontDescription& fontDescription, float scaleFactor) const
{
    LOGFONT logFont;
    PLOGFONT hfont;

    memcpy(&logFont, m_platformData.hfont(), sizeof(LOGFONT));
    logFont.size = scaleFactor * fontDescription.computedSize();

    hfont = CreateLogFontIndirect(&logFont);
    return new SimpleFontData(FontPlatformData(hfont, logFont.size), isCustomFont(), false);
}

SimpleFontData* SimpleFontData::smallCapsFontData(const FontDescription& fontDescription) const
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
#if USE(FREETYPE)
        if (!m_derivedFontData)
            m_derivedFontData = DerivedFontData::create(isCustomFont());
        // FIXME: I think we want to ask FontConfig for the right font again.
        if (!m_derivedFontData->smallCaps){
            //m_derivedFontData->smallCaps = scaledFontData(fontDescription, .7);
            float scaleFactor = .7;
            m_derivedFontData->smallCaps = new SimpleFontData(FontPlatformData(cairo_scaled_font_get_font_face(m_platformData.scaledFont()),
                        scaleFactor * fontDescription.computedSize(), m_platformData.syntheticBold(), m_platformData.syntheticOblique()),
                    isCustomFont(), false);
        }

        return m_derivedFontData->smallCaps.get();
#endif
    }
#endif
    if (!m_derivedFontData)
        m_derivedFontData = DerivedFontData::create(isCustomFont());
    // FIXME: I think we want to ask FontConfig for the right font again.
    if (!m_derivedFontData->smallCaps)
        m_derivedFontData->smallCaps = scaledFontData(fontDescription, .7);

    return m_derivedFontData->smallCaps.get();
#if 0
    if (!m_smallCapsFontData) {
        LOGFONT logFont;
        PLOGFONT hfont;

        memcpy(&logFont, m_platformData.hfont(), sizeof(LOGFONT));
        //get logical font size
        logFont.size = lroundf(0.70f * fontDescription.computedSize());

        hfont = CreateLogFontIndirect(&logFont);
        m_smallCapsFontData =
            new SimpleFontData(FontPlatformData(hfont, logFont.size));
    }
    return m_smallCapsFontData;
#endif
}

SimpleFontData* SimpleFontData::emphasisMarkFontData(const FontDescription& fontDescription) const
{
    if (!m_derivedFontData)
        m_derivedFontData = DerivedFontData::create(isCustomFont());
    if (!m_derivedFontData->emphasisMark)
        m_derivedFontData->emphasisMark = scaledFontData(fontDescription, .5);

    return m_derivedFontData->emphasisMark.get();
}

bool SimpleFontData::containsCharacters(const UChar* characters, int length) const
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        FT_Face face = cairo_ft_scaled_font_lock_face(m_platformData.scaledFont());

        if (!face)
            return false;

        for (int i = 0; i < length; i++) {
            if (FcFreeTypeCharIndex(face, characters[i]) == 0) {
                cairo_ft_scaled_font_unlock_face(m_platformData.scaledFont());
                return false;
            }
        }

        cairo_ft_scaled_font_unlock_face(m_platformData.scaledFont());

        return true;
    }
#endif
    return false;
}

void SimpleFontData::determinePitch()
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        m_treatAsFixedPitch = m_platformData.isFixedPitch();
        return;
    }
#endif
    m_treatAsFixedPitch = false;
}

FloatRect SimpleFontData::platformBoundsForGlyph(Glyph) const
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        return FloatRect();
    }
#endif
    return FloatRect();
}

float SimpleFontData::platformWidthForGlyph(Glyph glyph) const
{
#if ENABLE(CAIRO_MG)
    if (m_platformData.useCairo()) {
        ASSERT(m_platformData.scaledFont());

        cairo_glyph_t cglyph = { glyph, 0, 0 };
        cairo_text_extents_t extents;
        cairo_scaled_font_glyph_extents(m_platformData.scaledFont(), &cglyph, 1, &extents);

        float w = (float)m_spaceWidth;
        if (cairo_scaled_font_status(m_platformData.scaledFont()) == CAIRO_STATUS_SUCCESS && extents.x_advance)
            w = (float)extents.x_advance;

        return w;    
    }
#endif

	/*FIXME: font css style size is zero.*/
    if(!m_platformData.size())
        return 0;

	SIZE size;
	int width;
    PLOGFONT oldFont;
   
    oldFont = SelectFont(HDC_SCREEN, m_platformData.hfont());

    GetTextExtent (HDC_SCREEN, (char*)&glyph, 2, &size);
    width = size.cx;

    SelectFont(HDC_SCREEN, oldFont);

    return width;
}

}  // namespace WebCore
