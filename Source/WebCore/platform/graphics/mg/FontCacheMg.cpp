/* 
** $Id: FontCacheMg.cpp 702 2011-06-27 09:26:14Z xwyan $
** 
** FontCacheMg.cpp: The platform implementation for FontCache.
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
#include "FontCache.h"

#include "Font.h"
#include "HashMap.h"
#include "HashSet.h"
#include "SimpleFontData.h"
#include "StringHash.h"

#if USE(ICU_UNICODE)
#include <unicode/uniset.h>
#endif

#if USE(MINIGUI_UNICODE)
#include "unicode/UnisetMg.h"
#endif

#include "NotImplemented.h"

#if ENABLE(CAIRO_MG)
#include "CString.h"
using std::min;
#endif

namespace WebCore
{

void FontCache::platformInit()
{
    // Not needed on mDolphin.
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    if (!FcInit())
        ASSERT_NOT_REACHED();
#endif
#endif
}

#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
FcPattern* createFontConfigPatternForCharacters(const UChar* characters, int length)
{
    FcPattern* pattern = FcPatternCreate();

    FcCharSet* fontConfigCharSet = FcCharSetCreate();
    for (int i = 0; i < length; ++i) {
        if (U16_IS_SURROGATE(characters[i]) && U16_IS_SURROGATE_LEAD(characters[i])
                && i != length - 1 && U16_IS_TRAIL(characters[i + 1])) {
            FcCharSetAddChar(fontConfigCharSet, U16_GET_SUPPLEMENTARY(characters[i], characters[i+1]));
            i++;
        } else
            FcCharSetAddChar(fontConfigCharSet, characters[i]);
    }
    FcPatternAddCharSet(pattern, FC_CHARSET, fontConfigCharSet);
    FcCharSetDestroy(fontConfigCharSet);

    FcPatternAddBool(pattern, FC_SCALABLE, FcTrue);
    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    return pattern;
}

FcPattern* findBestFontGivenFallbacks(const FontPlatformData& fontData, FcPattern* pattern)
{
    if (!fontData.m_pattern)
        return 0;

    if (!fontData.m_fallbacks) {
        FcResult fontConfigResult;
        fontData.m_fallbacks = FcFontSort(0, fontData.m_pattern.get(), FcTrue, 0, &fontConfigResult);
    }

    if (!fontData.m_fallbacks)
        return 0;

    FcFontSet* sets[] = { fontData.m_fallbacks };
    FcResult fontConfigResult;
    return FcFontSetMatch(0, sets, 1, pattern, &fontConfigResult);
}

static String getFamilyNameStringFromFontDescriptionAndFamily(const FontDescription& fontDescription, const AtomicString& family)
{
    // If we're creating a fallback font (e.g. "-webkit-monospace"), convert the name into
    // the fallback name (like "monospace") that fontconfig understands.
    if (family.length() && !family.startsWith("-webkit-"))
        return family.string();

    switch (fontDescription.genericFamily()) {
    case FontDescription::StandardFamily:
    case FontDescription::SerifFamily:
        return "serif";
    case FontDescription::SansSerifFamily:
        return "sans-serif";
    case FontDescription::MonospaceFamily:
        return "monospace";
    case FontDescription::CursiveFamily:
        return "cursive";
    case FontDescription::FantasyFamily:
        return "fantasy";
    case FontDescription::NoFamily:
    default:
        return "";
    }
}

int fontWeightToFontconfigWeight(FontWeight weight)
{
    switch (weight) {
    case FontWeight100:
        return FC_WEIGHT_THIN;
    case FontWeight200:
        return FC_WEIGHT_ULTRALIGHT;
    case FontWeight300:
        return FC_WEIGHT_LIGHT;
    case FontWeight400:
        return FC_WEIGHT_REGULAR;
    case FontWeight500:
        return FC_WEIGHT_MEDIUM;
    case FontWeight600:
        return FC_WEIGHT_SEMIBOLD;
    case FontWeight700:
        return FC_WEIGHT_BOLD;
    case FontWeight800:
        return FC_WEIGHT_EXTRABOLD;
    case FontWeight900:
        return FC_WEIGHT_ULTRABLACK;
    default:
        ASSERT_NOT_REACHED();
        return FC_WEIGHT_REGULAR;
    }
}

const SimpleFontData* FontCache::getFontDataForCharacters(const Font& font, const UChar* characters, int length)
{
    RefPtr<FcPattern> pattern = adoptRef(createFontConfigPatternForCharacters(characters, length));
    const FontPlatformData& fontData = font.primaryFont()->platformData();

    RefPtr<FcPattern> fallbackPattern = adoptRef(findBestFontGivenFallbacks(fontData, pattern.get()));
    if (fallbackPattern) {
        FontPlatformData alternateFontData(fallbackPattern.get(), font.fontDescription());
        return getCachedFontData(&alternateFontData);
    }

    FcResult fontConfigResult;
    RefPtr<FcPattern> resultPattern = adoptRef(FcFontMatch(0, pattern.get(), &fontConfigResult));
    if (!resultPattern)
        return 0;
    FontPlatformData alternateFontData(resultPattern.get(), font.fontDescription());
    return getCachedFontData(&alternateFontData);
}

SimpleFontData* FontCache::getLastResortFallbackFont(const FontDescription& fontDescription)
{
    // We want to return a fallback font here, otherwise the logic preventing FontConfig
    // matches for non-fallback fonts might return 0. See isFallbackFontAllowed.
    static AtomicString timesStr("serif");
    return getCachedFontData(fontDescription, timesStr);
}
#endif
#else
const SimpleFontData* FontCache::getFontDataForCharacters(const Font& font, const UChar* characters, int length)
{
    notImplemented();
    return 0;
}

SimpleFontData* FontCache::getLastResortFallbackFont(const FontDescription& description)
{
    FontDescription::GenericFamilyType generic = description.genericFamily();

    // FIXME: Would be even better to somehow get the user's default font here.
    // For now we'll pick the default that the user would get without changing
    // any prefs.
    static AtomicString timesStr("Times New Roman");
    static AtomicString courierStr("Courier New");
    static AtomicString arialStr("Arial");

    AtomicString& fontStr = timesStr;
    if (generic == FontDescription::SansSerifFamily)
        fontStr = arialStr;
    else if (generic == FontDescription::MonospaceFamily)
        fontStr = courierStr;

    return getCachedFontData(description, fontStr);
}

#endif

SimpleFontData* FontCache::getSimilarFontPlatformData(const Font& font)
{
    notImplemented();
    return 0;
}

void FontCache::getTraitsInFamily(const AtomicString& familyName, Vector<unsigned>& traitsMasks)
{
    notImplemented();
}

FontPlatformData* FontCache::createFontPlatformData(const FontDescription& fontDescription, const AtomicString& family)
{
#if ENABLE(CAIRO_MG)
#if !ENABLE(CAIRO_TEXT)
    if (fontDescription.renderingMode() == AlternateRenderingMode) 
#endif
    {
#if USE(FREETYPE)
        // The CSS font matching algorithm (http://www.w3.org/TR/css3-fonts/#font-matching-algorithm)
        // says that we must find an exact match for font family, slant (italic or oblique can be used)
        // and font weight (we only match bold/non-bold here).
        RefPtr<FcPattern> pattern = adoptRef(FcPatternCreate());
        String familyNameString(getFamilyNameStringFromFontDescriptionAndFamily(fontDescription, family));
        if (!FcPatternAddString(pattern.get(), FC_FAMILY, reinterpret_cast<const FcChar8*>(familyNameString.utf8().data())))
            return 0;

        bool italic = fontDescription.italic();
        if (!FcPatternAddInteger(pattern.get(), FC_SLANT, italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN))
            return 0;
        if (!FcPatternAddInteger(pattern.get(), FC_WEIGHT, fontWeightToFontconfigWeight(fontDescription.weight())))
            return 0;
        if (!FcPatternAddDouble(pattern.get(), FC_PIXEL_SIZE, fontDescription.computedPixelSize()))
            return 0;

        // The strategy is originally from Skia (src/ports/SkFontHost_fontconfig.cpp):

        // Allow Fontconfig to do pre-match substitution. Unless we are accessing a "fallback"
        // family like "sans," this is the only time we allow Fontconfig to substitute one
        // family name for another (i.e. if the fonts are aliased to each other).
        FcConfigSubstitute(0, pattern.get(), FcMatchPattern);
        FcDefaultSubstitute(pattern.get());

        FcChar8* fontConfigFamilyNameAfterConfiguration;
        FcPatternGetString(pattern.get(), FC_FAMILY, 0, &fontConfigFamilyNameAfterConfiguration);
        String familyNameAfterConfiguration = String::fromUTF8(reinterpret_cast<char*>(fontConfigFamilyNameAfterConfiguration));

        FcResult fontConfigResult;
        RefPtr<FcPattern> resultPattern = adoptRef(FcFontMatch(0, pattern.get(), &fontConfigResult));
        if (!resultPattern) // No match.
            return 0;

        FcChar8* fontConfigFamilyNameAfterMatching;
        FcPatternGetString(resultPattern.get(), FC_FAMILY, 0, &fontConfigFamilyNameAfterMatching);
        String familyNameAfterMatching = String::fromUTF8(reinterpret_cast<char*>(fontConfigFamilyNameAfterMatching));

        // If Fontconfig gave use a different font family than the one we requested, we should ignore it
        // and allow WebCore to give us the next font on the CSS fallback list. The only exception is if
        // this family name is a commonly used generic family.
        if (!equalIgnoringCase(familyNameAfterConfiguration, familyNameAfterMatching)
                && !(equalIgnoringCase(familyNameString, "sans") || equalIgnoringCase(familyNameString, "sans-serif")
                    || equalIgnoringCase(familyNameString, "serif") || equalIgnoringCase(familyNameString, "monospace")
                    || equalIgnoringCase(familyNameString, "fantasy") || equalIgnoringCase(familyNameString, "cursive")))
            return 0;

        // Verify that this font has an encoding compatible with Fontconfig. Fontconfig currently
        // supports three encodings in FcFreeTypeCharIndex: Unicode, Symbol and AppleRoman.
        // If this font doesn't have one of these three encodings, don't select it.
        FontPlatformData* platformData = new FontPlatformData(resultPattern.get(), fontDescription);
        if (!platformData->hasCompatibleCharmap()) {
            delete platformData;
            return 0;
        }

        return platformData;
#endif
    }
#endif

#if PLATFORM(BIG_ENDIAN)
	const char * font_charset = "UTF-16BE"; 
#else
	const char * font_charset = "UTF-16LE"; 
#endif 

    //param:type, family, charset, weight, slant, flip, other, 
    //      underline, struckout, size, rotation
    PLOGFONT font = CreateLogFont (NULL, (char*)family.characters(), font_charset,
                FONT_WEIGHT_SUBPIXEL, FONT_SLANT_ROMAN, FONT_FLIP_NIL, 
                FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE,
                FONT_STRUCKOUT_NONE, fontDescription.computedPixelSize(), 0);

    //If using truetype in MiniGUI, subpixel style cann't process other style,
    //such as bold and italic.  We should enhance MiniGUI to get correct 
    //font file for required logical font. 
    if (strcmp(font->type, FONT_TYPE_NAME_SCALE_TTF))
    {
        //only for truetype font
        font->style &= ~FS_WEIGHT_SUBPIXEL;

        if (fontDescription.weight() >= FontWeight600)
            font->style |= FS_WEIGHT_BOLD;
        else if (fontDescription.weight() <= FontWeight300)
            font->style |= FS_WEIGHT_LIGHT;

        if (fontDescription.italic())
            font->style |= FS_SLANT_ITALIC;
    }

    return new FontPlatformData(font, fontDescription.computedPixelSize());
}

}
