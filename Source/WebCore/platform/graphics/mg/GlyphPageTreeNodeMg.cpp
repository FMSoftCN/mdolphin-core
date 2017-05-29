/* 
** $Id: GlyphPageTreeNodeMg.cpp 611 2011-04-27 06:46:12Z lijiangwei $
** 
** GlyphPageTreeNodeMg.cpp: The platform implementation for GlyphPage. 
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
#include "GlyphPageTreeNode.h"

#include "SimpleFontData.h"

#if ENABLE(CAIRO_MG)
#if defined(USE_PANGO)
#include <pango/pango-font.h>
#endif
#endif

namespace WebCore {

#if ENABLE(CAIRO_MG)
#if defined(USE_PANGO)
static PangoGlyph pango_font_get_glyph(PangoFont* font, PangoContext* context, gunichar wc)
{
    PangoGlyph result = 0;
    gchar buffer[7];

    gint  length = g_unichar_to_utf8(wc, buffer);
    g_return_val_if_fail(length, 0);

    GList* items = pango_itemize(context, buffer, 0, length, NULL, NULL);

    if (g_list_length(items) == 1) {
        PangoItem* item = reinterpret_cast<PangoItem*>(items->data);
        PangoFont* tmpFont = item->analysis.font;
        item->analysis.font = font;

        PangoGlyphString* glyphs = pango_glyph_string_new();
        pango_shape(buffer, length, &item->analysis, glyphs);

        item->analysis.font = tmpFont;

        if (glyphs->num_glyphs == 1)
            result = glyphs->glyphs[0].glyph;
        else
            g_warning("didn't get 1 glyph but %d", glyphs->num_glyphs);

        pango_glyph_string_free(glyphs);
    }

    g_list_foreach(items, (GFunc)pango_item_free, NULL);
    g_list_free(items);

    return result;
}
#endif
#endif

bool GlyphPage::fill(unsigned offset, unsigned length, UChar* buffer, unsigned bufferLength, const SimpleFontData* fontData)
{
#if ENABLE(CAIRO_MG)
    if (fontData->platformData().useCairo()){
#if USE(FREETYPE)
        // The bufferLength will be greater than the glyph page size if the buffer has Unicode supplementary characters.
        // We won't support this for now.
        if (bufferLength > GlyphPage::size)
            return false;

        FT_Face face = cairo_ft_scaled_font_lock_face(fontData->platformData().m_scaledFont);
        if (!face)
            return false;

        bool haveGlyphs = false;
        for (unsigned i = 0; i < length; i++) {
            Glyph glyph = FcFreeTypeCharIndex(face, buffer[i]);
            if (!glyph)
                setGlyphDataForIndex(offset + i, 0, 0);
            else {
                setGlyphDataForIndex(offset + i, glyph, fontData);
                haveGlyphs = true;
            }
        }

        cairo_ft_scaled_font_unlock_face(fontData->platformData().m_scaledFont);

        return haveGlyphs;
#elif defined(USE_PANGO)
    // The bufferLength will be greater than the glyph page size if the buffer has Unicode supplementary characters.
    // We won't support this for now.
    if (bufferLength > GlyphPage::size)
        return false;

    if (!fontData->platformData().m_font || fontData->platformData().m_font == reinterpret_cast<PangoFont*>(-1))
        return false;

    bool haveGlyphs = false;
    for (unsigned i = 0; i < length; i++) {
        Glyph glyph = pango_font_get_glyph(fontData->platformData().m_font, fontData->platformData().m_context, buffer[i]);
        if (!glyph)
            setGlyphDataForIndex(offset + i, 0, 0);
        else {
            setGlyphDataForIndex(offset + i, glyph, fontData);
            haveGlyphs = true;
        }
    }

    return haveGlyphs;
#endif
    }
#endif

    if (!fontData->platformData().hfont() || fontData->platformData().isHashTableDeletedValue())
        return false;

    bool isUtf16 = bufferLength != length;
#define MD_U16_GET_SUPPLEMENTARY(lead, trail) \
    (((UChar32)(lead)<<10UL)+(UChar32)(trail)-((0xd800<<10UL)+0xdc00-0x10000))

    for (unsigned i = 0; i < length; i++) {
        UChar32 character;
        if (isUtf16) {
            UChar lead = buffer[i * 2];
            UChar trail = buffer[i * 2 + 1];
            character = MD_U16_GET_SUPPLEMENTARY(lead, trail);
        }
        else {
            character = buffer[i];
        }

        setGlyphDataForIndex(offset + i, character, fontData);
    }
#undef MD_U16_GET_SUPPLEMENTARY

    return true;
}

}
