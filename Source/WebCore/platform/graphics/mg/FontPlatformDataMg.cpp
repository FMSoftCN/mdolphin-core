/* 
** $Id: FontPlatformDataMg.cpp 702 2011-06-27 09:26:14Z xwyan $
** 
** FontPlatformDataMg.cpp: The platform implementation for FontPlatformData.
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
#include "FontPlatformData.h"
#include "PlatformString.h"

#if ENABLE(CAIRO_MG)
#include <wtf/text/CString.h>

#if USE(FREETYPE)
#include <cairo-ft.h>
#include <cairo.h>
#include <fontconfig/fcfreetype.h>
#endif

#endif

//Note: we only implement FREETYPE backend!!!
namespace WebCore {

FontPlatformData::FontPlatformData(WTF::HashTableDeletedValueType)
    : m_mgfont(hashTableDeletedFontValue())
    , m_size(0)
    , m_useCairo(false)
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    , m_pattern(0)
    , m_fallbacks(0)
#elif USE(PANGO)
    , m_context(0)
    , m_pangofont(hashTableDeletedCairoFontValue())
#endif
    , m_syntheticBold(false)
    , m_syntheticOblique(false)
    , m_scaledFont(hashTableDeletedCairoFontValue())
#endif
{
}

FontPlatformData::FontPlatformData()
    : m_mgfont(0)
    , m_size(0)
    , m_useCairo(false)
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    , m_pattern(0)
    , m_fallbacks(0)
#elif USE(PANGO)
    : m_context(0)
    , m_pangofont(0)
#endif
    , m_syntheticBold(false)
    , m_syntheticOblique(false)
    , m_scaledFont(0)
#endif
{
}

FontPlatformData::FontPlatformData(PLOGFONT font, int size)
    : m_mgfont(RefCountedHFONT::create(font))
    , m_size(size)
    , m_useCairo(false)
{
}

FontPlatformData::FontPlatformData(float size, bool bold, bool italic)
    : m_mgfont(0)
    , m_size(size)
    , m_useCairo(true)
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    , m_pattern(0)
    , m_fallbacks(0)
    , m_fixedWidth(false)
    , m_syntheticBold(bold)
    , m_syntheticOblique(italic)
    , m_scaledFont(0)
#endif
#endif
{
}

FontPlatformData::FontPlatformData(const FontPlatformData& other)
    : m_mgfont(other.m_mgfont)
    , m_size(other.m_size)
    , m_useCairo(other.m_useCairo)
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    , m_pattern(0)
    , m_fallbacks(0)
    , m_scaledFont(0)
#endif
#endif
{
    *this = other;
}

FontPlatformData& FontPlatformData::operator=(const FontPlatformData& other)
{
    if (this == &other)
        return *this;

    m_size = other.m_size;
    if (!useCairo()) {
        m_mgfont = other.m_mgfont;
        m_useCairo = other.m_useCairo;
        return *this;
    }

#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    m_syntheticBold = other.m_syntheticBold;
    m_syntheticOblique = other.m_syntheticOblique;
    m_fixedWidth = other.m_fixedWidth;
    m_pattern = other.m_pattern;
    if (m_fallbacks) {
        FcFontSetDestroy(m_fallbacks);
        // This will be re-created on demand.
        m_fallbacks = 0;
    }

    if (m_scaledFont && m_scaledFont != hashTableDeletedCairoFontValue())
        cairo_scaled_font_destroy(m_scaledFont);
    m_scaledFont = cairo_scaled_font_reference(other.m_scaledFont);

    return *this;
#endif
#endif

    return *this;
}

bool FontPlatformData::operator==(const FontPlatformData& other) const
{
    if(!useCairo()) 
        return m_mgfont == other.m_mgfont && m_size == other.m_size;

#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    if (m_pattern == other.m_pattern)
        return true;
    if (!m_pattern || !other.m_pattern)
        return false;

    return FcPatternEqual(m_pattern.get(), other.m_pattern.get())
        && m_scaledFont == other.m_scaledFont && m_size == other.m_size
        && m_syntheticOblique == other.m_syntheticOblique && m_syntheticBold == other.m_syntheticBold; 
#elif defined(USE_PANGO)
    if (m_pangofont == other.m_pangofont)
        return true;
    if (m_pangofont == 0 || m_pangofont == reinterpret_cast<PangoFont*>(-1)
            || other.m_pangofont == 0 || other.m_pangofont == reinterpret_cast<PangoFont*>(-1))
        return false;
    PangoFontDescription* thisDesc = pango_font_describe(m_pangofont);
    PangoFontDescription* otherDesc = pango_font_describe(other.m_pangofont);
    bool result = pango_font_description_equal(thisDesc, otherDesc);
    pango_font_description_free(otherDesc);
    pango_font_description_free(thisDesc);
    return result;
#endif
#endif
    return false;
}

FontPlatformData::~FontPlatformData()
{
    if(!useCairo())
        return;
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    if (m_fallbacks) {
        FcFontSetDestroy(m_fallbacks);
        m_fallbacks = 0;
    }

    if (m_scaledFont && m_scaledFont != hashTableDeletedCairoFontValue())
        cairo_scaled_font_destroy(m_scaledFont);
#endif
#endif
}

FontPlatformData::RefCountedHFONT::~RefCountedHFONT()
{
    if (m_hfont && m_hfont != reinterpret_cast<PLOGFONT>(-1)) {
        DestroyLogFont(m_hfont);
        m_hfont = reinterpret_cast<PLOGFONT>(-1);
    }
}

FontPlatformData::RefCountedHFONT* FontPlatformData::hashTableDeletedFontValue()
{
    static RefPtr<RefCountedHFONT> deletedValue =
        RefCountedHFONT::create(reinterpret_cast<PLOGFONT>(-1));
    return deletedValue.get();
}

#ifndef NDEBUG
String FontPlatformData::description() const
{
    return String();
}
#endif

#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
cairo_subpixel_order_t convertFontConfigSubpixelOrder(int fontConfigOrder)
{
    switch (fontConfigOrder) {
    case FC_RGBA_RGB:
        return CAIRO_SUBPIXEL_ORDER_RGB;
    case FC_RGBA_BGR:
        return CAIRO_SUBPIXEL_ORDER_BGR;
    case FC_RGBA_VRGB:
        return CAIRO_SUBPIXEL_ORDER_VRGB;
    case FC_RGBA_VBGR:
        return CAIRO_SUBPIXEL_ORDER_VBGR;
    case FC_RGBA_NONE:
    case FC_RGBA_UNKNOWN:
        return CAIRO_SUBPIXEL_ORDER_DEFAULT;
    }
    return CAIRO_SUBPIXEL_ORDER_DEFAULT;
}

cairo_hint_style_t convertFontConfigHintStyle(int fontConfigStyle)
{
    switch (fontConfigStyle) {
    case FC_HINT_NONE:
        return CAIRO_HINT_STYLE_NONE;
    case FC_HINT_SLIGHT:
        return CAIRO_HINT_STYLE_SLIGHT;
    case FC_HINT_MEDIUM:
        return CAIRO_HINT_STYLE_MEDIUM;
    case FC_HINT_FULL:
        return CAIRO_HINT_STYLE_FULL;
    }
    return CAIRO_HINT_STYLE_NONE;
}

void setCairoFontOptionsFromFontConfigPattern(cairo_font_options_t* options, FcPattern* pattern)
{
    FcBool booleanResult;
    int integerResult;

    if (FcPatternGetInteger(pattern, FC_RGBA, 0, &integerResult) == FcResultMatch) {
        cairo_font_options_set_subpixel_order(options, convertFontConfigSubpixelOrder(integerResult));

        // Based on the logic in cairo-ft-font.c in the cairo source, a font with
        // a subpixel order implies that is uses subpixel antialiasing.
        if (integerResult != FC_RGBA_NONE)
            cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_SUBPIXEL);
    }

    if (FcPatternGetBool(pattern, FC_ANTIALIAS, 0, &booleanResult) == FcResultMatch) {
        // Only override the anti-aliasing setting if was previously turned off. Otherwise
        // we'll override the preference which decides between gray anti-aliasing and
        // subpixel anti-aliasing.
        if (!booleanResult)
            cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_NONE);
        else if (cairo_font_options_get_antialias(options) == CAIRO_ANTIALIAS_NONE)
            cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_GRAY);
    }

    if (FcPatternGetInteger(pattern, FC_HINT_STYLE, 0, &integerResult) == FcResultMatch)
        cairo_font_options_set_hint_style(options, convertFontConfigHintStyle(integerResult));
    if (FcPatternGetBool(pattern, FC_HINTING, 0, &booleanResult) == FcResultMatch && !booleanResult)
        cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_NONE);
}

static cairo_font_options_t* getDefaultFontOptions()
{
    return cairo_font_options_create();
}

FontPlatformData::FontPlatformData(FcPattern* pattern, const FontDescription& fontDescription)
    : m_size(fontDescription.computedPixelSize())
    , m_useCairo(true)
    , m_pattern(pattern)
    , m_fallbacks(0)
    , m_fixedWidth(false)
    , m_syntheticBold(false)
    , m_syntheticOblique(false)
    , m_scaledFont(0)
{
    RefPtr<cairo_font_face_t> fontFace = adoptRef(cairo_ft_font_face_create_for_pattern(m_pattern.get()));
    initializeWithFontFace(fontFace.get());

    int spacing;
    if (FcPatternGetInteger(pattern, FC_SPACING, 0, &spacing) == FcResultMatch && spacing == FC_MONO)
        m_fixedWidth = true;

    if (fontDescription.weight() >= FontWeightBold) {
        // The FC_EMBOLDEN property instructs us to fake the boldness of the font.
        FcBool fontConfigEmbolden;
        if (FcPatternGetBool(pattern, FC_EMBOLDEN, 0, &fontConfigEmbolden) == FcResultMatch)
            m_syntheticBold = fontConfigEmbolden;
    }
}

FontPlatformData::FontPlatformData(cairo_font_face_t* fontFace, float size, bool bold, bool italic)
    : m_size(size)
    , m_useCairo(true)
    , m_pattern(0)
    , m_fallbacks(0)
    , m_syntheticBold(bold)
    , m_syntheticOblique(italic)
    , m_scaledFont(0)
{
    initializeWithFontFace(fontFace);

    FT_Face fontConfigFace = cairo_ft_scaled_font_lock_face(m_scaledFont);
    if (fontConfigFace) {
        m_fixedWidth = fontConfigFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH;
        cairo_ft_scaled_font_unlock_face(m_scaledFont);
    }
}

void FontPlatformData::initializeWithFontFace(cairo_font_face_t* fontFace)
{
    cairo_font_options_t* options = getDefaultFontOptions();

    cairo_matrix_t ctm;
    cairo_matrix_init_identity(&ctm);

    cairo_matrix_t fontMatrix;
    if (!m_pattern)
        cairo_matrix_init_scale(&fontMatrix, m_size, m_size);
    else {
        setCairoFontOptionsFromFontConfigPattern(options, m_pattern.get());

        // FontConfig may return a list of transformation matrices with the pattern, for instance,
        // for fonts that are oblique. We use that to initialize the cairo font matrix.
        FcMatrix fontConfigMatrix, *tempFontConfigMatrix;
        FcMatrixInit(&fontConfigMatrix);

        // These matrices may be stacked in the pattern, so it's our job to get them all and multiply them.
        for (int i = 0; FcPatternGetMatrix(m_pattern.get(), FC_MATRIX, i, &tempFontConfigMatrix) == FcResultMatch; i++)
            FcMatrixMultiply(&fontConfigMatrix, &fontConfigMatrix, tempFontConfigMatrix);
        cairo_matrix_init(&fontMatrix, fontConfigMatrix.xx, -fontConfigMatrix.yx,
                          -fontConfigMatrix.xy, fontConfigMatrix.yy, 0, 0);

        // The matrix from FontConfig does not include the scale.
        cairo_matrix_scale(&fontMatrix, m_size, m_size);
    }

    m_scaledFont = cairo_scaled_font_create(fontFace, &fontMatrix, &ctm, options);
    cairo_font_options_destroy(options);
}

bool FontPlatformData::hasCompatibleCharmap()
{
    if (!m_scaledFont)
        return false;

    FT_Face freeTypeFace = cairo_ft_scaled_font_lock_face(m_scaledFont);
    bool hasCompatibleCharmap = !(FT_Select_Charmap(freeTypeFace, ft_encoding_unicode)
                                && FT_Select_Charmap(freeTypeFace, ft_encoding_symbol)
                                && FT_Select_Charmap(freeTypeFace, ft_encoding_apple_roman));
    cairo_ft_scaled_font_unlock_face(m_scaledFont);
    return hasCompatibleCharmap;
}


bool FontPlatformData::isFixedPitch()
{
    return m_fixedWidth;
}
#endif
#endif
} //WebCore
