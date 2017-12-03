/* 
** $Id: FontPlatformData.h 704 2011-06-28 01:01:57Z xwyan $
** 
** FontPlatformData.h: The definition for FontPlatformData class.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: XiaoweiYan
**
** Create date: 2010-06-01
*/

#ifndef FontPlatformData_h
#define FontPlatformData_h

#include "FontOrientation.h"
#include "StringImpl.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

#if ENABLE(CAIRO_MG)
#include "HashFunctions.h"
#include "RefPtrCairo.h"
#include <cairo.h>
#include "FontDescription.h"
#include "GlyphBuffer.h"

#if USE(FREETYPE)
#include <cairo-ft.h>
#include <fontconfig/fcfreetype.h>
#elif USE(PANGO)
#include <pango/pangocairo.h>
#endif

#endif //ENABLE(CAIRO_MG)

struct _LOGFONT;
typedef struct _LOGFONT LOGFONT;
typedef LOGFONT* PLOGFONT;

namespace WebCore {

class FontDescription;

class FontPlatformData {
public:
    // Used for deleted values in the font cache's hash tables. The hash table
    // will create us with this structure, and it will compare other values
    // to this "Deleted" one. It expects the Deleted one to be differentiable
    // from the NULL one (created with the empty constructor), so we can't just
    // set everything to NULL.
    FontPlatformData(WTF::HashTableDeletedValueType);
    FontPlatformData();

#if ENABLE(CAIRO_MG)
    FontPlatformData(FcPattern*, const FontDescription&);
    FontPlatformData(cairo_font_face_t* fontFace, float size, bool bold, bool italic);
    FontPlatformData(const FontPlatformData&, float size);
    bool isFixedPitch();
    void setSize(float size) { m_size = size; }
    bool syntheticBold() const { return m_syntheticBold; }
    bool syntheticOblique() const { return m_syntheticOblique; }
    bool hasCompatibleCharmap();

    cairo_scaled_font_t* scaledFont() const { return m_scaledFont; }
#endif

    FontPlatformData(PLOGFONT pLogFont, int size);
    FontPlatformData(float size, bool bold, bool oblique);
    FontPlatformData(const FontPlatformData&);

    FontPlatformData& operator=(const FontPlatformData&);

    bool isHashTableDeletedValue() const 
    {
        if (!useCairo())
            return m_mgfont == hashTableDeletedFontValue(); 
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
        return m_scaledFont == hashTableDeletedCairoFontValue();
#elif USE(PANGO)
        return m_pangofont == hashTableDeletedCairoFontValue();
#endif
#endif
        return true;
    }

    ~FontPlatformData();

    PLOGFONT hfont() const { return m_mgfont ? m_mgfont->hfont() : 0; }
    float size() const { return m_size; }

    FontOrientation orientation() const { return Horizontal; } // FIXME: Implement.
    void setOrientation(FontOrientation) { } // FIXME: Implement.

    unsigned hash() const
    { 
        if (!useCairo())
            return m_mgfont ? m_mgfont->hash() : 0;
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
        return PtrHash<cairo_scaled_font_t*>::hash(m_scaledFont);
#elif USE(PANGO)
        uintptr_t hashCodes[1] = { reinterpret_cast<uintptr_t>(m_scaledFont) };
        return StringImpl::computeHash(reinterpret_cast<UChar*>(hashCodes), sizeof(hashCodes) / sizeof(UChar));
#endif
#endif
        return 0;
    }

    bool operator==(const FontPlatformData& other) const;
    bool useCairo() const {
#if ENABLE(CAIRO_MG)

#if ENABLE(CAIRO_TEXT)
        return true;
#else
        return m_useCairo;
#endif

#else
        return false;
#endif
    }

#ifndef NDEBUG
    String description() const;
#endif

private:
    // We refcount the internal PLOGFONT so that FontPlatformData can be
    // efficiently copied. WebKit depends on being able to copy it, and we
    // don't really want to re-create the PLOGFONT.
    class RefCountedHFONT : public RefCounted<RefCountedHFONT> {
    public:
        static PassRefPtr<RefCountedHFONT> create(PLOGFONT hfont)
        {
            return adoptRef(new RefCountedHFONT(hfont));
        }

        ~RefCountedHFONT();

        PLOGFONT hfont() const { return m_hfont; }
        unsigned hash() const
        {
            return StringHasher::computeHash(reinterpret_cast<const UChar*>(&m_hfont), sizeof(PLOGFONT) / sizeof(UChar));
        }

        bool operator==(const RefCountedHFONT& other) const
        {
            return m_hfont == other.m_hfont;
        }

    private:
        // The create() function assumes there is already a refcount of one 
        // so it can do adoptRef.
        RefCountedHFONT(PLOGFONT hfont) : m_hfont(hfont)
        {
        }

        PLOGFONT m_hfont;
    };

    static RefCountedHFONT* hashTableDeletedFontValue();

    RefPtr<RefCountedHFONT> m_mgfont;
    float m_size;  // Point size of the font in pixels.
    bool m_useCairo; // For cairo backend

public:
#if ENABLE(CAIRO_MG)
#if USE(FREETYPE)
    RefPtr<FcPattern> m_pattern;
    mutable FcFontSet* m_fallbacks;
    bool m_fixedWidth;
#elif USE(PANGO)
    static PangoFontMap* m_mgfontMap;
    static GHashTable* m_hashTable;

    PangoContext* m_context;
    PangoFont* m_pangofont;
#endif
    bool m_syntheticBold;
    bool m_syntheticOblique;
    cairo_scaled_font_t* m_scaledFont;

private:
#if USE(FREETYPE)
    void initializeWithFontFace(cairo_font_face_t*);
    static cairo_scaled_font_t* hashTableDeletedCairoFontValue() { return reinterpret_cast<cairo_scaled_font_t*>(-1); }
#elif USE(PANGO)
    static PangoFont *hashTableDeletedCairoFontValue() { return reinterpret_cast<PangoFont*>(-1); }
#endif
#endif //ENABLE(CAIRO_MG)
};

} // WebCore

#endif // FontPlatformData_h
