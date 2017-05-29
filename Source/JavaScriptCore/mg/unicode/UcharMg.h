/*
** $Id: UcharMg.h 592 2011-04-22 07:16:58Z lijiangwei $
**
** UcharMg.h: unicode characters implements.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-25
*/

#ifndef UcharMg_h
#define UcharMg_h

#include  "unicode/UtypesMg.h"

typedef unsigned short UChar;
typedef unsigned short UChar16;
typedef int UChar32;
typedef signed int USChar32; 

/** Number of bytes in a UChar. @stable ICU 2.0 */
#define U_SIZEOF_UCHAR 2

#define U_MAX_VERSION_LENGTH 4

/** The binary form of a version on ICU APIs is an array of 4 uint8_t.
 *  @stable ICU 2.4
 */
typedef unsigned char UVersionInfo[U_MAX_VERSION_LENGTH];

/**
 * This specifies the language directional property of a character set.
 * @stable ICU 2.0
 */
typedef enum UCharDirection {
    /** See note !!.  Comments of the form "EN" are read by genpname. */

    /** L @stable ICU 2.0 */
    U_LEFT_TO_RIGHT               = 0,
    /** R @stable ICU 2.0 */
    U_RIGHT_TO_LEFT               = 1,
    /** EN @stable ICU 2.0 */
    U_EUROPEAN_NUMBER             = 2,
    /** ES @stable ICU 2.0 */
    U_EUROPEAN_NUMBER_SEPARATOR   = 3,
    /** ET @stable ICU 2.0 */
    U_EUROPEAN_NUMBER_TERMINATOR  = 4,
    /** AN @stable ICU 2.0 */
    U_ARABIC_NUMBER               = 5,
    /** CS @stable ICU 2.0 */
    U_COMMON_NUMBER_SEPARATOR     = 6,
    /** B @stable ICU 2.0 */
    U_BLOCK_SEPARATOR             = 7,
    /** S @stable ICU 2.0 */
    U_SEGMENT_SEPARATOR           = 8,
    /** WS @stable ICU 2.0 */
    U_WHITE_SPACE_NEUTRAL         = 9,
    /** ON @stable ICU 2.0 */
    U_OTHER_NEUTRAL               = 10,
    /** LRE @stable ICU 2.0 */
    U_LEFT_TO_RIGHT_EMBEDDING     = 11,
    /** LRO @stable ICU 2.0 */
    U_LEFT_TO_RIGHT_OVERRIDE      = 12,
    /** AL @stable ICU 2.0 */
    U_RIGHT_TO_LEFT_ARABIC        = 13,
    /** RLE @stable ICU 2.0 */
    U_RIGHT_TO_LEFT_EMBEDDING     = 14,
    /** RLO @stable ICU 2.0 */
    U_RIGHT_TO_LEFT_OVERRIDE      = 15,
    /** PDF @stable ICU 2.0 */
    U_POP_DIRECTIONAL_FORMAT      = 16,
    /** NSM @stable ICU 2.0 */
    U_DIR_NON_SPACING_MARK        = 17,
    /** BN @stable ICU 2.0 */
    U_BOUNDARY_NEUTRAL            = 18,
    /** @stable ICU 2.0 */
    U_CHAR_DIRECTION_COUNT
} UCharDirection;


#define U_SENTINEL (-1)
#ifndef U16_LENGTH
#define U16_LENGTH(c) ((uint32_t)(c)<=0xffff ? 1 : 2)
#endif

/** Option value for case folding: use default mappings defined in CaseFolding.txt. @stable ICU 2.0 */
#define U_FOLD_CASE_DEFAULT 0

/**
 * Is this code unit a lead surrogate (U+d800..U+dbff)?
 * @param c 16-bit code unit
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U16_IS_LEAD(c) (((c)&0xfffffc00)==0xd800)

/**
 * Is this code unit a trail surrogate (U+dc00..U+dfff)?
 * @param c 16-bit code unit
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)

/**
 * Assuming c is a surrogate code point (U16_IS_SURROGATE(c)),
 * is it a lead surrogate?
 * @param c 16-bit code unit
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U16_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)

/**
 * Helper constant for U16_GET_SUPPLEMENTARY.
 * @internal
 */
#define U16_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)


/**
 * Get a supplementary code point value (U+10000..U+10ffff)
 * from its lead and trail surrogates.
 * The result is undefined if the input values are not
 * lead and trail surrogates.
 *
 * @param lead lead surrogate (U+d800..U+dbff)
 * @param trail trail surrogate (U+dc00..U+dfff)
 * @return supplementary code point (U+10000..U+10ffff)
 * @stable ICU 2.4
 */
#ifndef U16_GET_SUPPLEMENTARY
#define U16_GET_SUPPLEMENTARY(lead, trail) \
    (((USChar32)(lead)<<10UL)+(USChar32)(trail)-U16_SURROGATE_OFFSET)
#endif

/**
 * Is this code point a surrogate (U+d800..U+dfff)?
 * @param c 32-bit code point
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U_IS_SURROGATE(c) (((c)&0xfffff800)==0xd800)

#define UTF_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)

/**
 * Does this code unit alone encode a code point (BMP, not a surrogate)?
 * @param c 16-bit code unit
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U16_IS_SINGLE(c) !U_IS_SURROGATE(c)

/**
 * Assuming c is a surrogate code point (U_IS_SURROGATE(c)),
 * is it a lead surrogate?
 * @param c 32-bit code point
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)


/**
 * Is this code unit a surrogate (U+d800..U+dfff)?
 * @param c 16-bit code unit
 * @return TRUE or FALSE
 * @stable ICU 2.4
 */
#define U16_IS_SURROGATE(c) U_IS_SURROGATE(c)

/**
 * Get the lead surrogate (0xd800..0xdbff) for a
 * supplementary code point (0x10000..0x10ffff).
 * @param supplementary 32-bit code point (U+10000..U+10ffff)
 * @return lead surrogate (U+d800..U+dbff) for supplementary
 * @stable ICU 2.4
 */
#define U16_LEAD(supplementary) (UChar)(((supplementary)>>10)+0xd7c0)

/**
 * Get the trail surrogate (0xdc00..0xdfff) for a
 * supplementary code point (0x10000..0x10ffff).
 * @param supplementary 32-bit code point (U+10000..U+10ffff)
 * @return trail surrogate (U+dc00..U+dfff) for supplementary
 * @stable ICU 2.4
 */
#define U16_TRAIL(supplementary) (UChar)(((supplementary)&0x3ff)|0xdc00)

/**
 * Get a code point from a string at a random-access offset,
 * without changing the offset.
 * "Safe" macro, handles unpaired surrogates and checks for string boundaries.
 *
 * The offset may point to either the lead or trail surrogate unit
 * for a supplementary code point, in which case the macro will read
 * the adjacent matching surrogate as well.
 * If the offset points to a single, unpaired surrogate, then that itself
 * will be returned as the code point.
 * Iteration through a string is more efficient with U16_NEXT_UNSAFE or U16_NEXT.
 *
 * @param s const UChar * string
 * @param start starting string offset (usually 0)
 * @param i string offset, must be start<=i<length
 * @param length string length
 * @param c output UChar32 variable
 * @see U16_GET_UNSAFE
 * @stable ICU 2.4
 */
#define U16_GET(s, start, i, length, c) { \
    (c)=(s)[i]; \
    if(U16_IS_SURROGATE(c)) { \
        uint16_t __c2; \
        if(U16_IS_SURROGATE_LEAD(c)) { \
            if((i)+1<(length) && U16_IS_TRAIL(__c2=(s)[(i)+1])) { \
                (c)=U16_GET_SUPPLEMENTARY((c), __c2); \
            } \
        } else { \
            if((i)-1>=(start) && U16_IS_LEAD(__c2=(s)[(i)-1])) { \
                (c)=U16_GET_SUPPLEMENTARY(__c2, (c)); \
            } \
        } \
    } \
}

#define U16_PREV(s, start, i, c) { \
    (c)=(s)[--(i)]; \
    if(U16_IS_TRAIL(c)) { \
        uint16_t __c2; \
        if((i)>(start) && U16_IS_LEAD(__c2=(s)[(i)-1])) { \
            --(i); \
            (c)=U16_GET_SUPPLEMENTARY(__c2, (c)); \
        } \
    } \
}

#define U16_APPEND_UNSAFE(s, i, c) { \
    if((uint32_t)(c)<=0xffff) { \
        (s)[(i)++]=(uint16_t)(c); \
    } else { \
        (s)[(i)++]=(uint16_t)(((c)>>10)+0xd7c0); \
        (s)[(i)++]=(uint16_t)(((c)&0x3ff)|0xdc00); \
    } \
}

/**
 * Append a code point to a string, overwriting 1 or 2 code units.
 * The offset points to the current end of the string contents
 * and is advanced (post-increment).
 * "Safe" macro, checks for a valid code point.
 * If a surrogate pair is written, checks for sufficient space in the string.
 * If the code point is not valid or a trail surrogate does not fit,
 * then isError is set to TRUE.
 *
 * @param s const UChar * string buffer
 * @param i string offset, must be i<capacity
 * @param capacity size of the string buffer
 * @param c code point to append
 * @param isError output UBool set to TRUE if an error occurs, otherwise not modified
 * @see U16_APPEND_UNSAFE
 * @stable ICU 2.4
 */
#define U16_APPEND(s, i, capacity, c, isError) { \
    if((uint32_t)(c)<=0xffff) { \
        (s)[(i)++]=(uint16_t)(c); \
    } else if((uint32_t)(c)<=0x10ffff && (i)+1<(capacity)) { \
        (s)[(i)++]=(uint16_t)(((c)>>10)+0xd7c0); \
        (s)[(i)++]=(uint16_t)(((c)&0x3ff)|0xdc00); \
    } else /* c>0x10ffff or not enough space */ { \
        (isError)=TRUE; \
    } \
}

/**
 * Get a code point from a string at a code point boundary offset,
 * and advance the offset to the next code point boundary.
 * (Post-incrementing forward iteration.)
 * "Safe" macro, handles unpaired surrogates and checks for string boundaries.
 *
 * The offset may point to the lead surrogate unit
 * for a supplementary code point, in which case the macro will read
 * the following trail surrogate as well.
 * If the offset points to a trail surrogate or
 * to a single, unpaired lead surrogate, then that itself
 * will be returned as the code point.
 *
 * @param s const UChar * string
 * @param i string offset, must be i<length
 * @param length string length
 * @param c output USChar32 variable
 * @see U16_NEXT_UNSAFE
 * @stable ICU 2.4
 */
#define U16_NEXT(s, i, length, c) { \
    (c)=(s)[(i)++]; \
    if(U16_IS_LEAD(c)) { \
        uint16_t __c2; \
        if((i)<(length) && U16_IS_TRAIL(__c2=(s)[(i)])) { \
            ++(i); \
            (c)=U16_GET_SUPPLEMENTARY((c), __c2); \
        } \
    } \
}

/**
 * Get a single-bit bit set (a flag) from a bit number 0..31.
 * @stable ICU 2.1
 */
#define U_MASK(x) ((uint32_t)1<<(x))

/**
 * Selection constants for Unicode properties.
 * These constants are used in functions like u_hasBinaryProperty to select
 * one of the Unicode properties.
 *
 * The properties APIs are intended to reflect Unicode properties as defined
 * in the Unicode Character Database (UCD) and Unicode Technical Reports (UTR).
 * For details about the properties see http://www.unicode.org/ucd/ .
 * For names of Unicode properties see the UCD file PropertyAliases.txt.
 *
 * Important: If ICU is built with UCD files from Unicode versions below, e.g., 3.2,
 * then properties marked with "new in Unicode 3.2" are not or not fully available.
 * Check u_getUnicodeVersion to be sure.
 *
 * @see u_hasBinaryProperty
 * @see u_getIntPropertyValue
 * @see u_getUnicodeVersion
 * @stable ICU 2.1
 */
typedef enum UProperty {
    /*  See note !!.  Comments of the form "Binary property Dash",
        "Enumerated property Script", "Double property Numeric_Value",
        and "String property Age" are read by genpname. */

    /*  Note: Place UCHAR_ALPHABETIC before UCHAR_BINARY_START so that
    debuggers display UCHAR_ALPHABETIC as the symbolic name for 0,
    rather than UCHAR_BINARY_START.  Likewise for other *_START
    identifiers. */

    /** Binary property Alphabetic. Same as u_isUAlphabetic, different from u_isalpha.
        Lu+Ll+Lt+Lm+Lo+Nl+Other_Alphabetic @stable ICU 2.1 */
    UCHAR_ALPHABETIC=0,
    /** First constant for binary Unicode properties. @stable ICU 2.1 */
    UCHAR_BINARY_START=UCHAR_ALPHABETIC,
    /** Binary property ASCII_Hex_Digit. 0-9 A-F a-f @stable ICU 2.1 */
    UCHAR_ASCII_HEX_DIGIT=1,
    /** Binary property Bidi_Control.
        Format controls which have specific functions
        in the Bidi Algorithm. @stable ICU 2.1 */
    UCHAR_BIDI_CONTROL=2,
    /** Binary property Bidi_Mirrored.
        Characters that may change display in RTL text.
        Same as u_isMirrored.
        See Bidi Algorithm, UTR 9. @stable ICU 2.1 */
    UCHAR_BIDI_MIRRORED=3,
    /** Binary property Dash. Variations of dashes. @stable ICU 2.1 */
    UCHAR_DASH=4,
    /** Binary property Default_Ignorable_Code_Point (new in Unicode 3.2).
        Ignorable in most processing.
        <2060..206F, FFF0..FFFB, E0000..E0FFF>+Other_Default_Ignorable_Code_Point+(Cf+Cc+Cs-White_Space) @stable ICU 2.1 */
    UCHAR_DEFAULT_IGNORABLE_CODE_POINT=5,
    /** Binary property Deprecated (new in Unicode 3.2).
        The usage of deprecated characters is strongly discouraged. @stable ICU 2.1 */
    UCHAR_DEPRECATED=6,
    /** Binary property Diacritic. Characters that linguistically modify
        the meaning of another character to which they apply. @stable ICU 2.1 */
    UCHAR_DIACRITIC=7,
    /** Binary property Extender.
        Extend the value or shape of a preceding alphabetic character,
        e.g., length and iteration marks. @stable ICU 2.1 */
    UCHAR_EXTENDER=8,
    /** Binary property Full_Composition_Exclusion.
        CompositionExclusions.txt+Singleton Decompositions+
        Non-Starter Decompositions. @stable ICU 2.1 */
    UCHAR_FULL_COMPOSITION_EXCLUSION=9,
    /** Binary property Grapheme_Base (new in Unicode 3.2).
        For programmatic determination of grapheme cluster boundaries.
        [0..10FFFF]-Cc-Cf-Cs-Co-Cn-Zl-Zp-Grapheme_Link-Grapheme_Extend-CGJ @stable ICU 2.1 */
    UCHAR_GRAPHEME_BASE=10,
    /** Binary property Grapheme_Extend (new in Unicode 3.2).
        For programmatic determination of grapheme cluster boundaries.
        Me+Mn+Mc+Other_Grapheme_Extend-Grapheme_Link-CGJ @stable ICU 2.1 */
    UCHAR_GRAPHEME_EXTEND=11,
    /** Binary property Grapheme_Link (new in Unicode 3.2).
        For programmatic determination of grapheme cluster boundaries. @stable ICU 2.1 */
    UCHAR_GRAPHEME_LINK=12,
    /** Binary property Hex_Digit.
        Characters commonly used for hexadecimal numbers. @stable ICU 2.1 */
    UCHAR_HEX_DIGIT=13,
    /** Binary property Hyphen. Dashes used to mark connections
        between pieces of words, plus the Katakana middle dot. @stable ICU 2.1 */
    UCHAR_HYPHEN=14,
    /** Binary property ID_Continue.
        Characters that can continue an identifier.
        DerivedCoreProperties.txt also says "NOTE: Cf characters should be filtered out."
        ID_Start+Mn+Mc+Nd+Pc @stable ICU 2.1 */
    UCHAR_ID_CONTINUE=15,
    /** Binary property ID_Start.
        Characters that can start an identifier.
        Lu+Ll+Lt+Lm+Lo+Nl @stable ICU 2.1 */
    UCHAR_ID_START=16,
    /** Binary property Ideographic.
        CJKV ideographs. @stable ICU 2.1 */
    UCHAR_IDEOGRAPHIC=17,
    /** Binary property IDS_Binary_Operator (new in Unicode 3.2).
        For programmatic determination of
        Ideographic Description Sequences. @stable ICU 2.1 */
    UCHAR_IDS_BINARY_OPERATOR=18,
    /** Binary property IDS_Trinary_Operator (new in Unicode 3.2).
        For programmatic determination of
        Ideographic Description Sequences. @stable ICU 2.1 */
    UCHAR_IDS_TRINARY_OPERATOR=19,
    /** Binary property Join_Control.
        Format controls for cursive joining and ligation. @stable ICU 2.1 */
    UCHAR_JOIN_CONTROL=20,
    /** Binary property Logical_Order_Exception (new in Unicode 3.2).
        Characters that do not use logical order and
        require special handling in most processing. @stable ICU 2.1 */
    UCHAR_LOGICAL_ORDER_EXCEPTION=21,
    /** Binary property Lowercase. Same as u_isULowercase, different from u_islower.
        Ll+Other_Lowercase @stable ICU 2.1 */
    UCHAR_LOWERCASE=22,
    /** Binary property Math. Sm+Other_Math @stable ICU 2.1 */
    UCHAR_MATH=23,
    /** Binary property Noncharacter_Code_Point.
        Code points that are explicitly defined as illegal
        for the encoding of characters. @stable ICU 2.1 */
    UCHAR_NONCHARACTER_CODE_POINT=24,
    /** Binary property Quotation_Mark. @stable ICU 2.1 */
    UCHAR_QUOTATION_MARK=25,
    /** Binary property Radical (new in Unicode 3.2).
        For programmatic determination of
        Ideographic Description Sequences. @stable ICU 2.1 */
    UCHAR_RADICAL=26,
    /** Binary property Soft_Dotted (new in Unicode 3.2).
        Characters with a "soft dot", like i or j.
        An accent placed on these characters causes
        the dot to disappear. @stable ICU 2.1 */
    UCHAR_SOFT_DOTTED=27,
    /** Binary property Terminal_Punctuation.
        Punctuation characters that generally mark
        the end of textual units. @stable ICU 2.1 */
    UCHAR_TERMINAL_PUNCTUATION=28,
    /** Binary property Unified_Ideograph (new in Unicode 3.2).
        For programmatic determination of
        Ideographic Description Sequences. @stable ICU 2.1 */
    UCHAR_UNIFIED_IDEOGRAPH=29,
    /** Binary property Uppercase. Same as u_isUUppercase, different from u_isupper.
        Lu+Other_Uppercase @stable ICU 2.1 */
    UCHAR_UPPERCASE=30,
    /** Binary property White_Space.
        Same as u_isUWhiteSpace, different from u_isspace and u_isWhitespace.
        Space characters+TAB+CR+LF-ZWSP-ZWNBSP @stable ICU 2.1 */
    UCHAR_WHITE_SPACE=31,
    /** Binary property XID_Continue.
        ID_Continue modified to allow closure under
        normalization forms NFKC and NFKD. @stable ICU 2.1 */
    UCHAR_XID_CONTINUE=32,
    /** Binary property XID_Start. ID_Start modified to allow
        closure under normalization forms NFKC and NFKD. @stable ICU 2.1 */
    UCHAR_XID_START=33,
    /** Binary property Case_Sensitive. Either the source of a case
        mapping or _in_ the target of a case mapping. Not the same as
        the general category Cased_Letter. @stable ICU 2.6 */
   UCHAR_CASE_SENSITIVE=34,
    /** Binary property STerm (new in Unicode 4.0.1).
        Sentence Terminal. Used in UAX #29: Text Boundaries
        (http://www.unicode.org/reports/tr29/)
        @stable ICU 3.0 */
    UCHAR_S_TERM=35,
    /** Binary property Variation_Selector (new in Unicode 4.0.1).
        Indicates all those characters that qualify as Variation Selectors.
        For details on the behavior of these characters,
        see StandardizedVariants.html and 15.6 Variation Selectors.
        @stable ICU 3.0 */
    UCHAR_VARIATION_SELECTOR=36,
    /** Binary property NFD_Inert.
        ICU-specific property for characters that are inert under NFD,
        i.e., they do not interact with adjacent characters.
        Used for example in normalizing transforms in incremental mode
        to find the boundary of safely normalizable text despite possible
        text additions.

        There is one such property per normalization form.
        These properties are computed as follows - an inert character is:
        a) unassigned, or ALL of the following:
        b) of combining class 0.
        c) not decomposed by this normalization form.
        AND if NFC or NFKC,
        d) can never compose with a previous character.
        e) can never compose with a following character.
        f) can never change if another character is added.
           Example: a-breve might satisfy all but f, but if you
           add an ogonek it changes to a-ogonek + breve

        See also com.ibm.text.UCD.NFSkippable in the ICU4J repository,
        and icu/source/common/unormimp.h .
        @stable ICU 3.0 */
    UCHAR_NFD_INERT=37,
    /** Binary property NFKD_Inert.
        ICU-specific property for characters that are inert under NFKD,
        i.e., they do not interact with adjacent characters.
        Used for example in normalizing transforms in incremental mode
        to find the boundary of safely normalizable text despite possible
        text additions.
        @see UCHAR_NFD_INERT
        @stable ICU 3.0 */
    UCHAR_NFKD_INERT=38,
    /** Binary property NFC_Inert.
        ICU-specific property for characters that are inert under NFC,
        i.e., they do not interact with adjacent characters.
        Used for example in normalizing transforms in incremental mode
        to find the boundary of safely normalizable text despite possible
        text additions.
        @see UCHAR_NFD_INERT
        @stable ICU 3.0 */
    UCHAR_NFC_INERT=39,
    /** Binary property NFKC_Inert.
        ICU-specific property for characters that are inert under NFKC,
        i.e., they do not interact with adjacent characters.
        Used for example in normalizing transforms in incremental mode
        to find the boundary of safely normalizable text despite possible
        text additions.
        @see UCHAR_NFD_INERT
        @stable ICU 3.0 */
    UCHAR_NFKC_INERT=40,
    /** Binary Property Segment_Starter.
        ICU-specific property for characters that are starters in terms of
        Unicode normalization and combining character sequences.
        They have ccc=0 and do not occur in non-initial position of the
        canonical decomposition of any character
        (like " in NFD(a-umlaut) and a Jamo T in an NFD(Hangul LVT)).
        ICU uses this property for segmenting a string for generating a set of
        canonically equivalent strings, e.g. for canonical closure while
        processing collation tailoring rules.
        @stable ICU 3.0 */
    UCHAR_SEGMENT_STARTER=41,
    /** Binary property Pattern_Syntax (new in Unicode 4.1).
        See UAX #31 Identifier and Pattern Syntax
        (http://www.unicode.org/reports/tr31/)
        @stable ICU 3.4 */
    UCHAR_PATTERN_SYNTAX=42,
    /** Binary property Pattern_White_Space (new in Unicode 4.1).
        See UAX #31 Identifier and Pattern Syntax
        (http://www.unicode.org/reports/tr31/)
        @stable ICU 3.4 */
    UCHAR_PATTERN_WHITE_SPACE=43,
    /** Binary property alnum (a C/POSIX character class).
        Implemented according to the UTS #18 Annex C Standard Recommendation.
        See the uchar.h file documentation.
        @stable ICU 3.4 */
    UCHAR_POSIX_ALNUM=44,
    /** Binary property blank (a C/POSIX character class).
        Implemented according to the UTS #18 Annex C Standard Recommendation.
        See the uchar.h file documentation.
        @stable ICU 3.4 */
    UCHAR_POSIX_BLANK=45,
    /** Binary property graph (a C/POSIX character class).
        Implemented according to the UTS #18 Annex C Standard Recommendation.
        See the uchar.h file documentation.
        @stable ICU 3.4 */
    UCHAR_POSIX_GRAPH=46,
    /** Binary property print (a C/POSIX character class).
        Implemented according to the UTS #18 Annex C Standard Recommendation.
        See the uchar.h file documentation.
        @stable ICU 3.4 */
    UCHAR_POSIX_PRINT=47,
    /** Binary property xdigit (a C/POSIX character class).
        Implemented according to the UTS #18 Annex C Standard Recommendation.
        See the uchar.h file documentation.
        @stable ICU 3.4 */
    UCHAR_POSIX_XDIGIT=48,
    /** One more than the last constant for binary Unicode properties. @stable ICU 2.1 */
    UCHAR_BINARY_LIMIT=49,

    /** Enumerated property Bidi_Class.
        Same as u_charDirection, returns UCharDirection values. @stable ICU 2.2 */
    UCHAR_BIDI_CLASS=0x1000,
    /** First constant for enumerated/integer Unicode properties. @stable ICU 2.2 */
    UCHAR_INT_START=UCHAR_BIDI_CLASS,
    /** Enumerated property Block.
        Same as ublock_getCode, returns UBlockCode values. @stable ICU 2.2 */
    UCHAR_BLOCK=0x1001,
    /** Enumerated property Canonical_Combining_Class.
        Same as u_getCombiningClass, returns 8-bit numeric values. @stable ICU 2.2 */
    UCHAR_CANONICAL_COMBINING_CLASS=0x1002,
    /** Enumerated property Decomposition_Type.
        Returns UDecompositionType values. @stable ICU 2.2 */
    UCHAR_DECOMPOSITION_TYPE=0x1003,
    /** Enumerated property East_Asian_Width.
        See http://www.unicode.org/reports/tr11/
        Returns UEastAsianWidth values. @stable ICU 2.2 */
    UCHAR_EAST_ASIAN_WIDTH=0x1004,
    /** Enumerated property General_Category.
        Same as u_charType, returns UCharCategory values. @stable ICU 2.2 */
    UCHAR_GENERAL_CATEGORY=0x1005,
    /** Enumerated property Joining_Group.
        Returns UJoiningGroup values. @stable ICU 2.2 */
    UCHAR_JOINING_GROUP=0x1006,
    /** Enumerated property Joining_Type.
        Returns UJoiningType values. @stable ICU 2.2 */
    UCHAR_JOINING_TYPE=0x1007,
    /** Enumerated property Line_Break.
        Returns ULineBreak values. @stable ICU 2.2 */
    UCHAR_LINE_BREAK=0x1008,
    /** Enumerated property Numeric_Type.
        Returns UNumericType values. @stable ICU 2.2 */
    UCHAR_NUMERIC_TYPE=0x1009,
    /** Enumerated property Script.
        Same as uscript_getScript, returns UScriptCode values. @stable ICU 2.2 */
    UCHAR_SCRIPT=0x100A,
    /** Enumerated property Hangul_Syllable_Type, new in Unicode 4.
        Returns UHangulSyllableType values. @stable ICU 2.6 */
    UCHAR_HANGUL_SYLLABLE_TYPE=0x100B,
    /** Enumerated property NFD_Quick_Check.
        Returns UNormalizationCheckResult values. @stable ICU 3.0 */
    UCHAR_NFD_QUICK_CHECK=0x100C,
    /** Enumerated property NFKD_Quick_Check.
        Returns UNormalizationCheckResult values. @stable ICU 3.0 */
    UCHAR_NFKD_QUICK_CHECK=0x100D,
    /** Enumerated property NFC_Quick_Check.
        Returns UNormalizationCheckResult values. @stable ICU 3.0 */
    UCHAR_NFC_QUICK_CHECK=0x100E,
    /** Enumerated property NFKC_Quick_Check.
        Returns UNormalizationCheckResult values. @stable ICU 3.0 */
    UCHAR_NFKC_QUICK_CHECK=0x100F,
    /** Enumerated property Lead_Canonical_Combining_Class.
        ICU-specific property for the ccc of the first code point
        of the decomposition, or lccc(c)=ccc(NFD(c)[0]).
        Useful for checking for canonically ordered text;
        see UNORM_FCD and http://www.unicode.org/notes/tn5/#FCD .
        Returns 8-bit numeric values like UCHAR_CANONICAL_COMBINING_CLASS. @stable ICU 3.0 */
    UCHAR_LEAD_CANONICAL_COMBINING_CLASS=0x1010,
    /** Enumerated property Trail_Canonical_Combining_Class.
        ICU-specific property for the ccc of the last code point
        of the decomposition, or tccc(c)=ccc(NFD(c)[last]).
        Useful for checking for canonically ordered text;
        see UNORM_FCD and http://www.unicode.org/notes/tn5/#FCD .
        Returns 8-bit numeric values like UCHAR_CANONICAL_COMBINING_CLASS. @stable ICU 3.0 */
    UCHAR_TRAIL_CANONICAL_COMBINING_CLASS=0x1011,
    /** Enumerated property Grapheme_Cluster_Break (new in Unicode 4.1).
        Used in UAX #29: Text Boundaries
        (http://www.unicode.org/reports/tr29/)
        Returns UGraphemeClusterBreak values. @stable ICU 3.4 */
    UCHAR_GRAPHEME_CLUSTER_BREAK=0x1012,
    /** Enumerated property Sentence_Break (new in Unicode 4.1).
        Used in UAX #29: Text Boundaries
        (http://www.unicode.org/reports/tr29/)
        Returns USentenceBreak values. @stable ICU 3.4 */
    UCHAR_SENTENCE_BREAK=0x1013,
    /** Enumerated property Word_Break (new in Unicode 4.1).
        Used in UAX #29: Text Boundaries
        (http://www.unicode.org/reports/tr29/)
        Returns UWordBreakValues values. @stable ICU 3.4 */
    UCHAR_WORD_BREAK=0x1014,
    /** One more than the last constant for enumerated/integer Unicode properties. @stable ICU 2.2 */
    UCHAR_INT_LIMIT=0x1015,

    /** Bitmask property General_Category_Mask.
        This is the General_Category property returned as a bit mask.
        When used in u_getIntPropertyValue(c), same as U_MASK(u_charType(c)),
        returns bit masks for UCharCategory values where exactly one bit is set.
        When used with u_getPropertyValueName() and u_getPropertyValueEnum(),
        a multi-bit mask is used for sets of categories like "Letters".
        Mask values should be cast to uint32_t.
        @stable ICU 2.4 */
    UCHAR_GENERAL_CATEGORY_MASK=0x2000,
    /** First constant for bit-mask Unicode properties. @stable ICU 2.4 */
    UCHAR_MASK_START=UCHAR_GENERAL_CATEGORY_MASK,
    /** One more than the last constant for bit-mask Unicode properties. @stable ICU 2.4 */
    UCHAR_MASK_LIMIT=0x2001,

    /** Double property Numeric_Value.
        Corresponds to u_getNumericValue. @stable ICU 2.4 */
    UCHAR_NUMERIC_VALUE=0x3000,
    /** First constant for double Unicode properties. @stable ICU 2.4 */
    UCHAR_DOUBLE_START=UCHAR_NUMERIC_VALUE,
    /** One more than the last constant for double Unicode properties. @stable ICU 2.4 */
    UCHAR_DOUBLE_LIMIT=0x3001,

    /** String property Age.
        Corresponds to u_charAge. @stable ICU 2.4 */
    UCHAR_AGE=0x4000,
    /** First constant for string Unicode properties. @stable ICU 2.4 */
    UCHAR_STRING_START=UCHAR_AGE,
    /** String property Bidi_Mirroring_Glyph.
        Corresponds to u_charMirror. @stable ICU 2.4 */
    UCHAR_BIDI_MIRRORING_GLYPH=0x4001,
    /** String property Case_Folding.
        Corresponds to u_strFoldCase in ustring.h. @stable ICU 2.4 */
    UCHAR_CASE_FOLDING=0x4002,
    /** String property ISO_Comment.
        Corresponds to u_getISOComment. @stable ICU 2.4 */
    UCHAR_ISO_COMMENT=0x4003,
    /** String property Lowercase_Mapping.
        Corresponds to u_strToLower in ustring.h. @stable ICU 2.4 */
    UCHAR_LOWERCASE_MAPPING=0x4004,
    /** String property Name.
        Corresponds to u_charName. @stable ICU 2.4 */
    UCHAR_NAME=0x4005,
    /** String property Simple_Case_Folding.
        Corresponds to u_foldCase. @stable ICU 2.4 */
    UCHAR_SIMPLE_CASE_FOLDING=0x4006,
    /** String property Simple_Lowercase_Mapping.
        Corresponds to u_tolower. @stable ICU 2.4 */
    UCHAR_SIMPLE_LOWERCASE_MAPPING=0x4007,
    /** String property Simple_Titlecase_Mapping.
        Corresponds to u_totitle. @stable ICU 2.4 */
    UCHAR_SIMPLE_TITLECASE_MAPPING=0x4008,
    /** String property Simple_Uppercase_Mapping.
        Corresponds to u_toupper. @stable ICU 2.4 */
    UCHAR_SIMPLE_UPPERCASE_MAPPING=0x4009,
    /** String property Titlecase_Mapping.
        Corresponds to u_strToTitle in ustring.h. @stable ICU 2.4 */
    UCHAR_TITLECASE_MAPPING=0x400A,
    /** String property Unicode_1_Name.
        Corresponds to u_charName. @stable ICU 2.4 */
    UCHAR_UNICODE_1_NAME=0x400B,
    /** String property Uppercase_Mapping.
        Corresponds to u_strToUpper in ustring.h. @stable ICU 2.4 */
    UCHAR_UPPERCASE_MAPPING=0x400C,
    /** One more than the last constant for string Unicode properties. @stable ICU 2.4 */
    UCHAR_STRING_LIMIT=0x400D,

    /** Represents a nonexistent or invalid property or property value. @stable ICU 2.4 */
    UCHAR_INVALID_CODE = -1
} UProperty;

/**
 * U_GC_XX_MASK constants are bit flags corresponding to Unicode
 * general category values.
 * For each category, the nth bit is set if the numeric value of the
 * corresponding UCharCategory constant is n.
 *
 * There are also some U_GC_Y_MASK constants for groups of general categories
 * like L for all letter categories.
 *
 * @see u_charType
 * @see U_GET_GC_MASK
 * @see UCharCategory
 * @stable ICU 2.1
 */
#define U_GC_CN_MASK    U_MASK(U_GENERAL_OTHER_TYPES)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LU_MASK    U_MASK(U_UPPERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LL_MASK    U_MASK(U_LOWERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LT_MASK    U_MASK(U_TITLECASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LM_MASK    U_MASK(U_MODIFIER_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LO_MASK    U_MASK(U_OTHER_LETTER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MN_MASK    U_MASK(U_NON_SPACING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ME_MASK    U_MASK(U_ENCLOSING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MC_MASK    U_MASK(U_COMBINING_SPACING_MARK)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ND_MASK    U_MASK(U_DECIMAL_DIGIT_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NL_MASK    U_MASK(U_LETTER_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NO_MASK    U_MASK(U_OTHER_NUMBER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZS_MASK    U_MASK(U_SPACE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZL_MASK    U_MASK(U_LINE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZP_MASK    U_MASK(U_PARAGRAPH_SEPARATOR)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CC_MASK    U_MASK(U_CONTROL_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CF_MASK    U_MASK(U_FORMAT_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CO_MASK    U_MASK(U_PRIVATE_USE_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CS_MASK    U_MASK(U_SURROGATE)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PD_MASK    U_MASK(U_DASH_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PS_MASK    U_MASK(U_START_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PE_MASK    U_MASK(U_END_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PC_MASK    U_MASK(U_CONNECTOR_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PO_MASK    U_MASK(U_OTHER_PUNCTUATION)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SM_MASK    U_MASK(U_MATH_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SC_MASK    U_MASK(U_CURRENCY_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SK_MASK    U_MASK(U_MODIFIER_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SO_MASK    U_MASK(U_OTHER_SYMBOL)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PI_MASK    U_MASK(U_INITIAL_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PF_MASK    U_MASK(U_FINAL_PUNCTUATION)


/** Mask constant for multiple UCharCategory bits (L Letters). @stable ICU 2.1 */
#define U_GC_L_MASK \
            (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK|U_GC_LM_MASK|U_GC_LO_MASK)

/** Mask constant for multiple UCharCategory bits (LC Cased Letters). @stable ICU 2.1 */
#define U_GC_LC_MASK \
            (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK)

/** Mask constant for multiple UCharCategory bits (M Marks). @stable ICU 2.1 */
#define U_GC_M_MASK (U_GC_MN_MASK|U_GC_ME_MASK|U_GC_MC_MASK)

/** Mask constant for multiple UCharCategory bits (N Numbers). @stable ICU 2.1 */
#define U_GC_N_MASK (U_GC_ND_MASK|U_GC_NL_MASK|U_GC_NO_MASK)

/** Mask constant for multiple UCharCategory bits (Z Separators). @stable ICU 2.1 */
#define U_GC_Z_MASK (U_GC_ZS_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK)

/** Mask constant for multiple UCharCategory bits (C Others). @stable ICU 2.1 */
#define U_GC_C_MASK \
            (U_GC_CN_MASK|U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CO_MASK|U_GC_CS_MASK)

/** Mask constant for multiple UCharCategory bits (P Punctuation). @stable ICU 2.1 */
#define U_GC_P_MASK \
            (U_GC_PD_MASK|U_GC_PS_MASK|U_GC_PE_MASK|U_GC_PC_MASK|U_GC_PO_MASK| \
             U_GC_PI_MASK|U_GC_PF_MASK)

/** Mask constant for multiple UCharCategory bits (S Symbols). @stable ICU 2.1 */
#define U_GC_S_MASK (U_GC_SM_MASK|U_GC_SC_MASK|U_GC_SK_MASK|U_GC_SO_MASK)

/**
 * Joining Type constants.
 *
 * @see UCHAR_JOINING_TYPE
 * @stable ICU 2.2
 */
typedef enum UJoiningType {
    U_JT_NON_JOINING,       /*[U]*/ /*See note !!*/
    U_JT_JOIN_CAUSING,      /*[C]*/
    U_JT_DUAL_JOINING,      /*[D]*/
    U_JT_LEFT_JOINING,      /*[L]*/
    U_JT_RIGHT_JOINING,     /*[R]*/
    U_JT_TRANSPARENT,       /*[T]*/
    U_JT_COUNT /* 6 */
} UJoiningType;



/**
 * Decomposition Type constants.
 *
 * @see UCHAR_DECOMPOSITION_TYPE
 * @stable ICU 2.2
 */
typedef enum UDecompositionType {
    U_DT_NONE,              /*[none]*/ /*See note !!*/
    U_DT_CANONICAL,         /*[can]*/
    U_DT_COMPAT,            /*[com]*/
    U_DT_CIRCLE,            /*[enc]*/
    U_DT_FINAL,             /*[fin]*/
    U_DT_FONT,              /*[font]*/
    U_DT_FRACTION,          /*[fra]*/
    U_DT_INITIAL,           /*[init]*/
    U_DT_ISOLATED,          /*[iso]*/
    U_DT_MEDIAL,            /*[med]*/
    U_DT_NARROW,            /*[nar]*/
    U_DT_NOBREAK,           /*[nb]*/
    U_DT_SMALL,             /*[sml]*/
    U_DT_SQUARE,            /*[sqr]*/
    U_DT_SUB,               /*[sub]*/
    U_DT_SUPER,             /*[sup]*/
    U_DT_VERTICAL,          /*[vert]*/
    U_DT_WIDE,              /*[wide]*/
    U_DT_COUNT /* 18 */
} UDecompositionType;

/**
 * Data for enumerated Unicode general category types.
 * See http://www.unicode.org/Public/UNIDATA/UnicodeData.html .
 * @stable ICU 2.0
 */
typedef enum UCharCategory
{
    /** See note !!.  Comments of the form "Cn" are read by genpname. */

    /** Non-category for unassigned and non-character code points. @stable ICU 2.0 */
    U_UNASSIGNED              = 0,
    /** Cn "Other, Not Assigned (no characters in [UnicodeData.txt] have this property)" (same as U_UNASSIGNED!) @stable ICU 2.0 */
    U_GENERAL_OTHER_TYPES     = 0,
    /** Lu @stable ICU 2.0 */
    U_UPPERCASE_LETTER        = 1,
    /** Ll @stable ICU 2.0 */
    U_LOWERCASE_LETTER        = 2,
    /** Lt @stable ICU 2.0 */
    U_TITLECASE_LETTER        = 3,
    /** Lm @stable ICU 2.0 */
    U_MODIFIER_LETTER         = 4,
    /** Lo @stable ICU 2.0 */
    U_OTHER_LETTER            = 5,
    /** Mn @stable ICU 2.0 */
    U_NON_SPACING_MARK        = 6,
    /** Me @stable ICU 2.0 */
    U_ENCLOSING_MARK          = 7,
    /** Mc @stable ICU 2.0 */
    U_COMBINING_SPACING_MARK  = 8,
    /** Nd @stable ICU 2.0 */
    U_DECIMAL_DIGIT_NUMBER    = 9,
    /** Nl @stable ICU 2.0 */
    U_LETTER_NUMBER           = 10,
    /** No @stable ICU 2.0 */
    U_OTHER_NUMBER            = 11,
    /** Zs @stable ICU 2.0 */
    U_SPACE_SEPARATOR         = 12,
    /** Zl @stable ICU 2.0 */
    U_LINE_SEPARATOR          = 13,
    /** Zp @stable ICU 2.0 */
    U_PARAGRAPH_SEPARATOR     = 14,
    /** Cc @stable ICU 2.0 */
    U_CONTROL_CHAR            = 15,
    /** Cf @stable ICU 2.0 */
    U_FORMAT_CHAR             = 16,
    /** Co @stable ICU 2.0 */
    U_PRIVATE_USE_CHAR        = 17,
    /** Cs @stable ICU 2.0 */
    U_SURROGATE               = 18,
    /** Pd @stable ICU 2.0 */
    U_DASH_PUNCTUATION        = 19,
    /** Ps @stable ICU 2.0 */
    U_START_PUNCTUATION       = 20,
    /** Pe @stable ICU 2.0 */
    U_END_PUNCTUATION         = 21,
    /** Pc @stable ICU 2.0 */
    U_CONNECTOR_PUNCTUATION   = 22,
    /** Po @stable ICU 2.0 */
    U_OTHER_PUNCTUATION       = 23,
    /** Sm @stable ICU 2.0 */
    U_MATH_SYMBOL             = 24,
    /** Sc @stable ICU 2.0 */
    U_CURRENCY_SYMBOL         = 25,
    /** Sk @stable ICU 2.0 */
    U_MODIFIER_SYMBOL         = 26,
    /** So @stable ICU 2.0 */
    U_OTHER_SYMBOL            = 27,
    /** Pi @stable ICU 2.0 */
    U_INITIAL_PUNCTUATION     = 28,
    /** Pf @stable ICU 2.0 */
    U_FINAL_PUNCTUATION       = 29,
    /** One higher than the last enum UCharCategory constant. @stable ICU 2.0 */
    U_CHAR_CATEGORY_COUNT
} UCharCategory;

/**
 * Joining Group constants.
 *
 * @see UCHAR_JOINING_GROUP
 * @stable ICU 2.2
 */
typedef enum UJoiningGroup {
    U_JG_NO_JOINING_GROUP,
    U_JG_AIN,
    U_JG_ALAPH,
    U_JG_ALEF,
    U_JG_BEH,
    U_JG_BETH,
    U_JG_DAL,
    U_JG_DALATH_RISH,
    U_JG_E,
    U_JG_FEH,
    U_JG_FINAL_SEMKATH,
    U_JG_GAF,
    U_JG_GAMAL,
    U_JG_HAH,
    U_JG_HAMZA_ON_HEH_GOAL,
    U_JG_HE,
    U_JG_HEH,
    U_JG_HEH_GOAL,
    U_JG_HETH,
    U_JG_KAF,
    U_JG_KAPH,
    U_JG_KNOTTED_HEH,
    U_JG_LAM,
    U_JG_LAMADH,
    U_JG_MEEM,
    U_JG_MIM,
    U_JG_NOON,
    U_JG_NUN,
    U_JG_PE,
    U_JG_QAF,
    U_JG_QAPH,
    U_JG_REH,
    U_JG_REVERSED_PE,
    U_JG_SAD,
    U_JG_SADHE,
    U_JG_SEEN,
    U_JG_SEMKATH,
    U_JG_SHIN,
    U_JG_SWASH_KAF,
    U_JG_SYRIAC_WAW,
    U_JG_TAH,
    U_JG_TAW,
    U_JG_TEH_MARBUTA,
    U_JG_TETH,
    U_JG_WAW,
    U_JG_YEH,
    U_JG_YEH_BARREE,
    U_JG_YEH_WITH_TAIL,
    U_JG_YUDH,
    U_JG_YUDH_HE,
    U_JG_ZAIN,
    U_JG_FE,        /**< @stable ICU 2.6 */
    U_JG_KHAPH,     /**< @stable ICU 2.6 */
    U_JG_ZHAIN,     /**< @stable ICU 2.6 */
    U_JG_BURUSHASKI_YEH_BARREE, /**< @stable ICU 4.0 */
    U_JG_COUNT
} UJoiningGroup;

/**
 * Constants for ISO 15924 script codes.
 *
 * Many of these script codes - those from Unicode's ScriptNames.txt -
 * are character property values for Unicode's Script property.
 * See UAX #24 Script Names (http://www.unicode.org/reports/tr24/).
 *
 * Starting with ICU 3.6, constants for most ISO 15924 script codes
 * are included (currently excluding private-use codes Qaaa..Qabx).
 * For scripts for which there are codes in ISO 15924 but which are not
 * used in the Unicode Character Database (UCD), there are no Unicode characters
 * associated with those scripts.
 *
 * For example, there are no characters that have a UCD script code of
 * Hans or Hant. All Han ideographs have the Hani script code.
 * The Hans and Hant script codes are used with CLDR data.
 *
 * ISO 15924 script codes are included for use with CLDR and similar.
 *
 * @stable ICU 2.2
 */
typedef enum UScriptCode {
      USCRIPT_INVALID_CODE = -1,
      USCRIPT_COMMON       =  0 , /* Zyyy */
      USCRIPT_INHERITED    =  1,  /* Qaai */
      USCRIPT_ARABIC       =  2,  /* Arab */
      USCRIPT_ARMENIAN     =  3,  /* Armn */
      USCRIPT_BENGALI      =  4,  /* Beng */
      USCRIPT_BOPOMOFO     =  5,  /* Bopo */
      USCRIPT_CHEROKEE     =  6,  /* Cher */
      USCRIPT_COPTIC       =  7,  /* Copt */
      USCRIPT_CYRILLIC     =  8,  /* Cyrl */
      USCRIPT_DESERET      =  9,  /* Dsrt */
      USCRIPT_DEVANAGARI   = 10,  /* Deva */
      USCRIPT_ETHIOPIC     = 11,  /* Ethi */
      USCRIPT_GEORGIAN     = 12,  /* Geor */
      USCRIPT_GOTHIC       = 13,  /* Goth */
      USCRIPT_GREEK        = 14,  /* Grek */
      USCRIPT_GUJARATI     = 15,  /* Gujr */
      USCRIPT_GURMUKHI     = 16,  /* Guru */
      USCRIPT_HAN          = 17,  /* Hani */
      USCRIPT_HANGUL       = 18,  /* Hang */
      USCRIPT_HEBREW       = 19,  /* Hebr */
      USCRIPT_HIRAGANA     = 20,  /* Hira */
      USCRIPT_KANNADA      = 21,  /* Knda */
      USCRIPT_KATAKANA     = 22,  /* Kana */
      USCRIPT_KHMER        = 23,  /* Khmr */
      USCRIPT_LAO          = 24,  /* Laoo */
      USCRIPT_LATIN        = 25,  /* Latn */
      USCRIPT_MALAYALAM    = 26,  /* Mlym */
      USCRIPT_MONGOLIAN    = 27,  /* Mong */
      USCRIPT_MYANMAR      = 28,  /* Mymr */
      USCRIPT_OGHAM        = 29,  /* Ogam */
      USCRIPT_OLD_ITALIC   = 30,  /* Ital */
      USCRIPT_ORIYA        = 31,  /* Orya */
      USCRIPT_RUNIC        = 32,  /* Runr */
      USCRIPT_SINHALA      = 33,  /* Sinh */
      USCRIPT_SYRIAC       = 34,  /* Syrc */
      USCRIPT_TAMIL        = 35,  /* Taml */
      USCRIPT_TELUGU       = 36,  /* Telu */
      USCRIPT_THAANA       = 37,  /* Thaa */
      USCRIPT_THAI         = 38,  /* Thai */
      USCRIPT_TIBETAN      = 39,  /* Tibt */
      /** Canadian_Aboriginal script. @stable ICU 2.6 */
      USCRIPT_CANADIAN_ABORIGINAL = 40,  /* Cans */
      /** Canadian_Aboriginal script (alias). @stable ICU 2.2 */
      USCRIPT_UCAS         = USCRIPT_CANADIAN_ABORIGINAL,
      USCRIPT_YI           = 41,  /* Yiii */
      USCRIPT_TAGALOG      = 42,  /* Tglg */
      USCRIPT_HANUNOO      = 43,  /* Hano */
      USCRIPT_BUHID        = 44,  /* Buhd */
      USCRIPT_TAGBANWA     = 45,  /* Tagb */

      /* New scripts in Unicode 4 @stable ICU 2.6 */
      USCRIPT_BRAILLE      = 46,  /* Brai */
      USCRIPT_CYPRIOT      = 47,  /* Cprt */
      USCRIPT_LIMBU        = 48,  /* Limb */
      USCRIPT_LINEAR_B     = 49,  /* Linb */
      USCRIPT_OSMANYA      = 50,  /* Osma */
      USCRIPT_SHAVIAN      = 51,  /* Shaw */
      USCRIPT_TAI_LE       = 52,  /* Tale */
      USCRIPT_UGARITIC     = 53,  /* Ugar */

      /** New script code in Unicode 4.0.1 @stable ICU 3.0 */
      USCRIPT_KATAKANA_OR_HIRAGANA = 54,/*Hrkt */
      /* New scripts in Unicode 4.1 @stable ICU 3.4 */
      USCRIPT_BUGINESE      = 55, /* Bugi */
      USCRIPT_GLAGOLITIC    = 56, /* Glag */
      USCRIPT_KHAROSHTHI    = 57, /* Khar */
      USCRIPT_SYLOTI_NAGRI  = 58, /* Sylo */
      USCRIPT_NEW_TAI_LUE   = 59, /* Talu */
      USCRIPT_TIFINAGH      = 60, /* Tfng */
      USCRIPT_OLD_PERSIAN   = 61, /* Xpeo */

      /* New script codes from ISO 15924 @stable ICU 3.6 */
      USCRIPT_BALINESE                      = 62, /* Bali */
      USCRIPT_BATAK                         = 63, /* Batk */
      USCRIPT_BLISSYMBOLS                   = 64, /* Blis */
      USCRIPT_BRAHMI                        = 65, /* Brah */
      USCRIPT_CHAM                          = 66, /* Cham */
      USCRIPT_CIRTH                         = 67, /* Cirt */
      USCRIPT_OLD_CHURCH_SLAVONIC_CYRILLIC  = 68, /* Cyrs */
      USCRIPT_DEMOTIC_EGYPTIAN              = 69, /* Egyd */
      USCRIPT_HIERATIC_EGYPTIAN             = 70, /* Egyh */
      USCRIPT_EGYPTIAN_HIEROGLYPHS          = 71, /* Egyp */
      USCRIPT_KHUTSURI                      = 72, /* Geok */
      USCRIPT_SIMPLIFIED_HAN                = 73, /* Hans */
      USCRIPT_TRADITIONAL_HAN               = 74, /* Hant */
      USCRIPT_PAHAWH_HMONG                  = 75, /* Hmng */
      USCRIPT_OLD_HUNGARIAN                 = 76, /* Hung */
      USCRIPT_HARAPPAN_INDUS                = 77, /* Inds */
      USCRIPT_JAVANESE                      = 78, /* Java */
      USCRIPT_KAYAH_LI                      = 79, /* Kali */
      USCRIPT_LATIN_FRAKTUR                 = 80, /* Latf */
      USCRIPT_LATIN_GAELIC                  = 81, /* Latg */
      USCRIPT_LEPCHA                        = 82, /* Lepc */
      USCRIPT_LINEAR_A                      = 83, /* Lina */
      USCRIPT_MANDAEAN                      = 84, /* Mand */
      USCRIPT_MAYAN_HIEROGLYPHS             = 85, /* Maya */
      USCRIPT_MEROITIC                      = 86, /* Mero */
      USCRIPT_NKO                           = 87, /* Nkoo */
      USCRIPT_ORKHON                        = 88, /* Orkh */
      USCRIPT_OLD_PERMIC                    = 89, /* Perm */
      USCRIPT_PHAGS_PA                      = 90, /* Phag */
      USCRIPT_PHOENICIAN                    = 91, /* Phnx */
      USCRIPT_PHONETIC_POLLARD              = 92, /* Plrd */
      USCRIPT_RONGORONGO                    = 93, /* Roro */
      USCRIPT_SARATI                        = 94, /* Sara */
      USCRIPT_ESTRANGELO_SYRIAC             = 95, /* Syre */
      USCRIPT_WESTERN_SYRIAC                = 96, /* Syrj */
      USCRIPT_EASTERN_SYRIAC                = 97, /* Syrn */
      USCRIPT_TENGWAR                       = 98, /* Teng */
      USCRIPT_VAI                           = 99, /* Vaii */
      USCRIPT_VISIBLE_SPEECH                = 100, /* Visp */
      USCRIPT_CUNEIFORM                     = 101,/* Xsux */
      USCRIPT_UNWRITTEN_LANGUAGES           = 102,/* Zxxx */
      USCRIPT_UNKNOWN                       = 103,/* Zzzz */ /* Unknown="Code for uncoded script", for unassigned code points */

      /* New script codes from ISO 15924 @stable ICU 3.8 */
      USCRIPT_CARIAN                        = 104,/* Cari */
      USCRIPT_JAPANESE                      = 105,/* Jpan */
      USCRIPT_LANNA                         = 106,/* Lana */
      USCRIPT_LYCIAN                        = 107,/* Lyci */
      USCRIPT_LYDIAN                        = 108,/* Lydi */
      USCRIPT_OL_CHIKI                      = 109,/* Olck */
      USCRIPT_REJANG                        = 110,/* Rjng */
      USCRIPT_SAURASHTRA                    = 111,/* Saur */
      USCRIPT_SIGN_WRITING                  = 112,/* Sgnw */
      USCRIPT_SUNDANESE                     = 113,/* Sund */
      USCRIPT_MOON                          = 114,/* Moon */
      USCRIPT_MEITEI_MAYEK                  = 115,/* Mtei */

      /* New script codes from ISO 15924 @stable ICU 4.0 */
      USCRIPT_IMPERIAL_ARAMAIC              = 116,/* Armi */
      USCRIPT_AVESTAN                       = 117,/* Avst */
      USCRIPT_CHAKMA                        = 118,/* Cakm */
      USCRIPT_KOREAN                        = 119,/* Kore */
      USCRIPT_KAITHI                        = 120,/* Kthi */
      USCRIPT_MANICHAEAN                    = 121,/* Mani */
      USCRIPT_INSCRIPTIONAL_PAHLAVI         = 122,/* Phli */
      USCRIPT_PSALTER_PAHLAVI               = 123,/* Phlp */
      USCRIPT_BOOK_PAHLAVI                  = 124,/* Phlv */
      USCRIPT_INSCRIPTIONAL_PARTHIAN        = 125,/* Prti */
      USCRIPT_SAMARITAN                     = 126,/* Samr */
      USCRIPT_TAI_VIET                      = 127,/* Tavt */
      USCRIPT_MATHEMATICAL_NOTATION         = 128,/* Zmth */
      USCRIPT_SYMBOLS                       = 129,/* Zsym */

      /* Private use codes from Qaaa - Qabx are not supported*/
      USCRIPT_CODE_LIMIT    = 130
} UScriptCode;


/**
 * Hangul Syllable Type constants.
 *
 * @see UCHAR_HANGUL_SYLLABLE_TYPE
 * @stable ICU 2.6
 */
typedef enum UHangulSyllableType {
    U_HST_NOT_APPLICABLE,   /*[NA]*/ /*See note !!*/
    U_HST_LEADING_JAMO,     /*[L]*/
    U_HST_VOWEL_JAMO,       /*[V]*/
    U_HST_TRAILING_JAMO,    /*[T]*/
    U_HST_LV_SYLLABLE,      /*[LV]*/
    U_HST_LVT_SYLLABLE,     /*[LVT]*/
    U_HST_COUNT
} UHangulSyllableType;

/**
 * Numeric Type constants.
 *
 * @see UCHAR_NUMERIC_TYPE
 * @stable ICU 2.2
 */
typedef enum UNumericType {
    U_NT_NONE,              /*[None]*/ /*See note !!*/
    U_NT_DECIMAL,           /*[de]*/
    U_NT_DIGIT,             /*[di]*/
    U_NT_NUMERIC,           /*[nu]*/
    U_NT_COUNT
} UNumericType;

/**
 * Line Break constants.
 *
 * @see UCHAR_LINE_BREAK
 * @stable ICU 2.2
 */
typedef enum ULineBreak {
    U_LB_UNKNOWN = 0,           /*[XX]*/ /*See note !!*/
    U_LB_AMBIGUOUS = 1,         /*[AI]*/
    U_LB_ALPHABETIC = 2,        /*[AL]*/
    U_LB_BREAK_BOTH = 3,        /*[B2]*/
    U_LB_BREAK_AFTER = 4,       /*[BA]*/
    U_LB_BREAK_BEFORE = 5,      /*[BB]*/
    U_LB_MANDATORY_BREAK = 6,   /*[BK]*/
    U_LB_CONTINGENT_BREAK = 7,  /*[CB]*/
    U_LB_CLOSE_PUNCTUATION = 8, /*[CL]*/
    U_LB_COMBINING_MARK = 9,    /*[CM]*/
    U_LB_CARRIAGE_RETURN = 10,   /*[CR]*/
    U_LB_EXCLAMATION = 11,       /*[EX]*/
    U_LB_GLUE = 12,              /*[GL]*/
    U_LB_HYPHEN = 13,            /*[HY]*/
    U_LB_IDEOGRAPHIC = 14,       /*[ID]*/
    U_LB_INSEPERABLE = 15,
    /** Renamed from the misspelled "inseperable" in Unicode 4.0.1/ICU 3.0 @stable ICU 3.0 */
    U_LB_INSEPARABLE=U_LB_INSEPERABLE,/*[IN]*/
    U_LB_INFIX_NUMERIC = 16,     /*[IS]*/
    U_LB_LINE_FEED = 17,         /*[LF]*/
    U_LB_NONSTARTER = 18,        /*[NS]*/
    U_LB_NUMERIC = 19,           /*[NU]*/
    U_LB_OPEN_PUNCTUATION = 20,  /*[OP]*/
    U_LB_POSTFIX_NUMERIC = 21,   /*[PO]*/
    U_LB_PREFIX_NUMERIC = 22,    /*[PR]*/
    U_LB_QUOTATION = 23,         /*[QU]*/
    U_LB_COMPLEX_CONTEXT = 24,   /*[SA]*/
    U_LB_SURROGATE = 25,         /*[SG]*/
    U_LB_SPACE = 26,             /*[SP]*/
    U_LB_BREAK_SYMBOLS = 27,     /*[SY]*/
    U_LB_ZWSPACE = 28,           /*[ZW]*/
    U_LB_NEXT_LINE = 29,         /*[NL]*/ /* from here on: new in Unicode 4/ICU 2.6 */
    U_LB_WORD_JOINER = 30,       /*[WJ]*/
    U_LB_H2 = 31,                /*[H2]*/ /* from here on: new in Unicode 4.1/ICU 3.4 */
    U_LB_H3 = 32,                /*[H3]*/
    U_LB_JL = 33,                /*[JL]*/
    U_LB_JT = 34,                /*[JT]*/
    U_LB_JV = 35,                /*[JV]*/
    U_LB_COUNT = 36
} ULineBreak;

/**
 * Option bit for u_strCaseCompare, u_strcasecmp, unorm_compare, etc:
 * Compare strings in code point order instead of code unit order.
 * @stable ICU 2.2
 */
#define U_COMPARE_CODE_POINT_ORDER  0x8000

/**
 * Get a single-bit bit set for the general category of a character.
 * This bit set can be compared bitwise with U_GC_SM_MASK, U_GC_L_MASK, etc.
 * Same as U_MASK(u_charType(c)).
 *
 * @param c the code point to be tested
 * @return a single-bit mask corresponding to the general category (UCharCategory) value
 *
 * @see u_charType
 * @see UCharCategory
 * @see U_GC_CN_MASK
 * @stable ICU 2.1
 */
#define U_GET_GC_MASK(c) U_MASK(u_charType(c))

/**
 * Constants for Unicode blocks, see the Unicode Data file Blocks.txt
 * @stable ICU 2.0
 */
enum UBlockCode {

    /** New No_Block value in Unicode 4. @stable ICU 2.6 */
    UBLOCK_NO_BLOCK = 0, /*[none]*/ /* Special range indicating No_Block */

    UBLOCK_BASIC_LATIN = 1, /*[0000]*/ /*See note !!*/

    UBLOCK_LATIN_1_SUPPLEMENT=2, /*[0080]*/

    UBLOCK_LATIN_EXTENDED_A =3, /*[0100]*/

    UBLOCK_LATIN_EXTENDED_B =4, /*[0180]*/

    UBLOCK_IPA_EXTENSIONS =5, /*[0250]*/

    UBLOCK_SPACING_MODIFIER_LETTERS =6, /*[02B0]*/

    UBLOCK_COMBINING_DIACRITICAL_MARKS =7, /*[0300]*/

    /**
     * Unicode 3.2 renames this block to "Greek and Coptic".
     * @stable ICU 2.0
     */
    UBLOCK_GREEK =8, /*[0370]*/

    UBLOCK_CYRILLIC =9, /*[0400]*/

    UBLOCK_ARMENIAN =10, /*[0530]*/

    UBLOCK_HEBREW =11, /*[0590]*/

    UBLOCK_ARABIC =12, /*[0600]*/

    UBLOCK_SYRIAC =13, /*[0700]*/

    UBLOCK_THAANA =14, /*[0780]*/

    UBLOCK_DEVANAGARI =15, /*[0900]*/

    UBLOCK_BENGALI =16, /*[0980]*/

    UBLOCK_GURMUKHI =17, /*[0A00]*/

    UBLOCK_GUJARATI =18, /*[0A80]*/

    UBLOCK_ORIYA =19, /*[0B00]*/

    UBLOCK_TAMIL =20, /*[0B80]*/

    UBLOCK_TELUGU =21, /*[0C00]*/

    UBLOCK_KANNADA =22, /*[0C80]*/

    UBLOCK_MALAYALAM =23, /*[0D00]*/

    UBLOCK_SINHALA =24, /*[0D80]*/

    UBLOCK_THAI =25, /*[0E00]*/

    UBLOCK_LAO =26, /*[0E80]*/

    UBLOCK_TIBETAN =27, /*[0F00]*/

    UBLOCK_MYANMAR =28, /*[1000]*/

    UBLOCK_GEORGIAN =29, /*[10A0]*/

    UBLOCK_HANGUL_JAMO =30, /*[1100]*/

    UBLOCK_ETHIOPIC =31, /*[1200]*/

    UBLOCK_CHEROKEE =32, /*[13A0]*/

    UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS =33, /*[1400]*/

    UBLOCK_OGHAM =34, /*[1680]*/

    UBLOCK_RUNIC =35, /*[16A0]*/

    UBLOCK_KHMER =36, /*[1780]*/

    UBLOCK_MONGOLIAN =37, /*[1800]*/

    UBLOCK_LATIN_EXTENDED_ADDITIONAL =38, /*[1E00]*/

    UBLOCK_GREEK_EXTENDED =39, /*[1F00]*/

    UBLOCK_GENERAL_PUNCTUATION =40, /*[2000]*/

    UBLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS =41, /*[2070]*/

    UBLOCK_CURRENCY_SYMBOLS =42, /*[20A0]*/

    /**
     * Unicode 3.2 renames this block to "Combining Diacritical Marks for Symbols".
     * @stable ICU 2.0
     */
    UBLOCK_COMBINING_MARKS_FOR_SYMBOLS =43, /*[20D0]*/

    UBLOCK_LETTERLIKE_SYMBOLS =44, /*[2100]*/

    UBLOCK_NUMBER_FORMS =45, /*[2150]*/

    UBLOCK_ARROWS =46, /*[2190]*/

    UBLOCK_MATHEMATICAL_OPERATORS =47, /*[2200]*/

    UBLOCK_MISCELLANEOUS_TECHNICAL =48, /*[2300]*/

    UBLOCK_CONTROL_PICTURES =49, /*[2400]*/

    UBLOCK_OPTICAL_CHARACTER_RECOGNITION =50, /*[2440]*/

    UBLOCK_ENCLOSED_ALPHANUMERICS =51, /*[2460]*/

    UBLOCK_BOX_DRAWING =52, /*[2500]*/

    UBLOCK_BLOCK_ELEMENTS =53, /*[2580]*/

    UBLOCK_GEOMETRIC_SHAPES =54, /*[25A0]*/

    UBLOCK_MISCELLANEOUS_SYMBOLS =55, /*[2600]*/

    UBLOCK_DINGBATS =56, /*[2700]*/

    UBLOCK_BRAILLE_PATTERNS =57, /*[2800]*/

    UBLOCK_CJK_RADICALS_SUPPLEMENT =58, /*[2E80]*/

    UBLOCK_KANGXI_RADICALS =59, /*[2F00]*/

    UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS =60, /*[2FF0]*/

    UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION =61, /*[3000]*/

    UBLOCK_HIRAGANA =62, /*[3040]*/

    UBLOCK_KATAKANA =63, /*[30A0]*/

    UBLOCK_BOPOMOFO =64, /*[3100]*/

    UBLOCK_HANGUL_COMPATIBILITY_JAMO =65, /*[3130]*/

    UBLOCK_KANBUN =66, /*[3190]*/

    UBLOCK_BOPOMOFO_EXTENDED =67, /*[31A0]*/

    UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS =68, /*[3200]*/

    UBLOCK_CJK_COMPATIBILITY =69, /*[3300]*/

    UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A =70, /*[3400]*/

    UBLOCK_CJK_UNIFIED_IDEOGRAPHS =71, /*[4E00]*/

    UBLOCK_YI_SYLLABLES =72, /*[A000]*/

    UBLOCK_YI_RADICALS =73, /*[A490]*/

    UBLOCK_HANGUL_SYLLABLES =74, /*[AC00]*/

    UBLOCK_HIGH_SURROGATES =75, /*[D800]*/

    UBLOCK_HIGH_PRIVATE_USE_SURROGATES =76, /*[DB80]*/

    UBLOCK_LOW_SURROGATES =77, /*[DC00]*/

    /**
     * Same as UBLOCK_PRIVATE_USE_AREA.
     * Until Unicode 3.1.1, the corresponding block name was "Private Use",
     * and multiple code point ranges had this block.
     * Unicode 3.2 renames the block for the BMP PUA to "Private Use Area" and
     * adds separate blocks for the supplementary PUAs.
     *
     * @stable ICU 2.0
     */
    UBLOCK_PRIVATE_USE = 78,

    /**
     * Same as UBLOCK_PRIVATE_USE.
     * Until Unicode 3.1.1, the corresponding block name was "Private Use",
     * and multiple code point ranges had this block.
     * Unicode 3.2 renames the block for the BMP PUA to "Private Use Area" and
     * adds separate blocks for the supplementary PUAs.
     *
     * @stable ICU 2.0
     */
    UBLOCK_PRIVATE_USE_AREA =UBLOCK_PRIVATE_USE, /*[E000]*/
    UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS =79, /*[F900]*/

    UBLOCK_ALPHABETIC_PRESENTATION_FORMS =80, /*[FB00]*/

    UBLOCK_ARABIC_PRESENTATION_FORMS_A =81, /*[FB50]*/

    UBLOCK_COMBINING_HALF_MARKS =82, /*[FE20]*/

    UBLOCK_CJK_COMPATIBILITY_FORMS =83, /*[FE30]*/

    UBLOCK_SMALL_FORM_VARIANTS =84, /*[FE50]*/

    UBLOCK_ARABIC_PRESENTATION_FORMS_B =85, /*[FE70]*/

    UBLOCK_SPECIALS =86, /*[FFF0]*/

    UBLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS =87, /*[FF00]*/

    /* New blocks in Unicode 3.1 */

    UBLOCK_OLD_ITALIC = 88  , /*[10300]*/
    UBLOCK_GOTHIC = 89 , /*[10330]*/
    UBLOCK_DESERET = 90 , /*[10400]*/
    UBLOCK_BYZANTINE_MUSICAL_SYMBOLS = 91 , /*[1D000]*/
    UBLOCK_MUSICAL_SYMBOLS = 92 , /*[1D100]*/
    UBLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS = 93  , /*[1D400]*/
    UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B  = 94 , /*[20000]*/
    UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT = 95 , /*[2F800]*/
    UBLOCK_TAGS = 96, /*[E0000]*/

    /* New blocks in Unicode 3.2 */

    /**
     * Unicode 4.0.1 renames the "Cyrillic Supplementary" block to "Cyrillic Supplement".
     * @stable ICU 2.2
     */
    UBLOCK_CYRILLIC_SUPPLEMENTARY = 97, 
    UBLOCK_CYRILLIC_SUPPLEMENT = UBLOCK_CYRILLIC_SUPPLEMENTARY, /*[0500]*/
    UBLOCK_TAGALOG = 98, /*[1700]*/
    UBLOCK_HANUNOO = 99, /*[1720]*/
    UBLOCK_BUHID = 100, /*[1740]*/
    UBLOCK_TAGBANWA = 101, /*[1760]*/
    UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A = 102, /*[27C0]*/
    UBLOCK_SUPPLEMENTAL_ARROWS_A = 103, /*[27F0]*/
    UBLOCK_SUPPLEMENTAL_ARROWS_B = 104, /*[2900]*/
    UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B = 105, /*[2980]*/
    UBLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS = 106, /*[2A00]*/
    UBLOCK_KATAKANA_PHONETIC_EXTENSIONS = 107, /*[31F0]*/
    UBLOCK_VARIATION_SELECTORS = 108, /*[FE00]*/
    UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A = 109, /*[F0000]*/
    UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B = 110, /*[100000]*/

    /* New blocks in Unicode 4 */

    UBLOCK_LIMBU = 111, /*[1900]*/
    UBLOCK_TAI_LE = 112, /*[1950]*/
    UBLOCK_KHMER_SYMBOLS = 113, /*[19E0]*/
    UBLOCK_PHONETIC_EXTENSIONS = 114, /*[1D00]*/
    UBLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS = 115, /*[2B00]*/
    UBLOCK_YIJING_HEXAGRAM_SYMBOLS = 116, /*[4DC0]*/
    UBLOCK_LINEAR_B_SYLLABARY = 117, /*[10000]*/
    UBLOCK_LINEAR_B_IDEOGRAMS = 118, /*[10080]*/
    UBLOCK_AEGEAN_NUMBERS = 119, /*[10100]*/
    UBLOCK_UGARITIC = 120, /*[10380]*/
    UBLOCK_SHAVIAN = 121, /*[10450]*/
    UBLOCK_OSMANYA = 122, /*[10480]*/
    UBLOCK_CYPRIOT_SYLLABARY = 123, /*[10800]*/
    UBLOCK_TAI_XUAN_JING_SYMBOLS = 124, /*[1D300]*/
    UBLOCK_VARIATION_SELECTORS_SUPPLEMENT = 125, /*[E0100]*/

    /* New blocks in Unicode 4.1 */

    UBLOCK_ANCIENT_GREEK_MUSICAL_NOTATION = 126, /*[1D200]*/
    UBLOCK_ANCIENT_GREEK_NUMBERS = 127, /*[10140]*/
    UBLOCK_ARABIC_SUPPLEMENT = 128, /*[0750]*/
    UBLOCK_BUGINESE = 129, /*[1A00]*/
    UBLOCK_CJK_STROKES = 130, /*[31C0]*/
    UBLOCK_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT = 131, /*[1DC0]*/
    UBLOCK_COPTIC = 132, /*[2C80]*/
    UBLOCK_ETHIOPIC_EXTENDED = 133, /*[2D80]*/
    UBLOCK_ETHIOPIC_SUPPLEMENT = 134, /*[1380]*/
    UBLOCK_GEORGIAN_SUPPLEMENT = 135, /*[2D00]*/
    UBLOCK_GLAGOLITIC = 136, /*[2C00]*/
    UBLOCK_KHAROSHTHI = 137, /*[10A00]*/
    UBLOCK_MODIFIER_TONE_LETTERS = 138, /*[A700]*/
    UBLOCK_NEW_TAI_LUE = 139, /*[1980]*/
    UBLOCK_OLD_PERSIAN = 140, /*[103A0]*/
    UBLOCK_PHONETIC_EXTENSIONS_SUPPLEMENT = 141, /*[1D80]*/
    UBLOCK_SUPPLEMENTAL_PUNCTUATION = 142, /*[2E00]*/
    UBLOCK_SYLOTI_NAGRI = 143, /*[A800]*/
    UBLOCK_TIFINAGH = 144, /*[2D30]*/
    UBLOCK_VERTICAL_FORMS = 145, /*[FE10]*/

    /* New blocks in Unicode 5.0 */

    UBLOCK_NKO = 146, /*[07C0]*/
    UBLOCK_BALINESE = 147, /*[1B00]*/
    UBLOCK_LATIN_EXTENDED_C = 148, /*[2C60]*/
    UBLOCK_LATIN_EXTENDED_D = 149, /*[A720]*/
    UBLOCK_PHAGS_PA = 150, /*[A840]*/
    UBLOCK_PHOENICIAN = 151, /*[10900]*/
    UBLOCK_CUNEIFORM = 152, /*[12000]*/
    UBLOCK_CUNEIFORM_NUMBERS_AND_PUNCTUATION = 153, /*[12400]*/
    UBLOCK_COUNTING_ROD_NUMERALS = 154, /*[1D360]*/

    /* New blocks in Unicode 5.1 */

    UBLOCK_SUNDANESE = 155, /*[1B80]*/
    UBLOCK_LEPCHA = 156, /*[1C00]*/
    UBLOCK_OL_CHIKI = 157, /*[1C50]*/
    UBLOCK_CYRILLIC_EXTENDED_A = 158, /*[2DE0]*/
    UBLOCK_VAI = 159, /*[A500]*/
    UBLOCK_CYRILLIC_EXTENDED_B = 160, /*[A640]*/
    UBLOCK_SAURASHTRA = 161, /*[A880]*/
    UBLOCK_KAYAH_LI = 162, /*[A900]*/
    UBLOCK_REJANG = 163, /*[A930]*/
    UBLOCK_CHAM = 164, /*[AA00]*/
    UBLOCK_ANCIENT_SYMBOLS = 165, /*[10190]*/
    UBLOCK_PHAISTOS_DISC = 166, /*[101D0]*/
    UBLOCK_LYCIAN = 167, /*[10280]*/
    UBLOCK_CARIAN = 168, /*[102A0]*/
    UBLOCK_LYDIAN = 169, /*[10920]*/
    UBLOCK_MAHJONG_TILES = 170, /*[1F000]*/
    UBLOCK_DOMINO_TILES = 171, /*[1F030]*/

    UBLOCK_COUNT = 172,
    UBLOCK_INVALID_CODE=-1
};

typedef enum UBlockCode UBlockCode;


#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Returns the general category value for the code point.
 *
 * Same as java.lang.Character.getType().
 *
 * @param c the code point to be tested
 * @return the general category (UCharCategory) value
 *
 * @see UCharCategory
 * @stable ICU 2.0
 */
int8_t  u_charType(USChar32 c);

/**
 * Determines whether the specified code point is an alphanumeric character
 * (letter or digit) according to Java.
 * True for characters with general categories
 * "L" (letters) and "Nd" (decimal digit numbers).
 *
 * Same as java.lang.Character.isLetterOrDigit().
 *
 * In addition to being equivalent to a Java function, this also serves
 * as a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is an alphanumeric character according to Character.isLetterOrDigit()
 *
 * @stable ICU 2.0
 */
UBool u_isalnum(UChar32 c);

/**
 * Determines whether the specified code point is a printable character.
 * True for general categories <em>other</em> than "C" (controls).
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a printable character
 *
 * @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see u_iscntrl
 * @stable ICU 2.0
 */
UBool u_isprint(USChar32 c);

/**
 * Determines whether the specified code point is a digit character according to Java.
 * True for characters with general category "Nd" (decimal digit numbers).
 * Beginning with Unicode 4, this is the same as
 * testing for the Numeric_Type of Decimal.
 *
 * Same as java.lang.Character.isDigit().
 *
 * In addition to being equivalent to a Java function, this also serves
 * as a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a digit character according to Character.isDigit()
 *
 * @stable ICU 2.0
 */
UBool u_isdigit(USChar32 c);

/**
 * Returns the decimal digit value of a decimal digit character.
 * Such characters have the general category "Nd" (decimal digit numbers)
 * and a Numeric_Type of Decimal.
 *
 * Unlike ICU releases before 2.6, no digit values are returned for any
 * Han characters because Han number characters are often used with a special
 * Chinese-style number format (with characters for powers of 10 in between)
 * instead of in decimal-positional notation.
 * Unicode 4 explicitly assigns Han number characters the Numeric_Type
 * Numeric instead of Decimal.
 * See Jitterbug 1483 for more details.
 *
 * Use u_getIntPropertyValue(c, UCHAR_NUMERIC_TYPE) and u_getNumericValue()
 * for complete numeric Unicode properties.
 *
 * @param c the code point for which to get the decimal digit value
 * @return the decimal digit value of c,
 *         or -1 if c is not a decimal digit character
 *
 * @see u_getNumericValue
 * @stable ICU 2.0
 */
int32_t u_charDigitValue(USChar32 c);

/**
 * Determines whether the specified code point is a punctuation character.
 * True for characters with general categories "P" (punctuation).
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a punctuation character
 */
UBool u_ispunct(USChar32 c);

/**
 * Determines if the specified character is a space character or not.
 *
 * @param c the character to be tested
 * @return  true if the character is a space character; false otherwise.
 */
UBool u_isspace(USChar32 c);

/**
 * Determines whether the specified code point has the general category "Ll"
 * (lowercase letter).
 *
 * Same as java.lang.Character.isLowerCase().
 *
 * This misses some characters that are also lowercase but
 * have a different general category value.
 * In order to include those, use UCHAR_LOWERCASE.
 *
 * In addition to being equivalent to a Java function, this also serves
 * as a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is an Ll lowercase letter
 */
UBool u_islower(USChar32 c);

/**
 * Determines whether the specified code point is a "blank" or "horizontal space",
 * a character that visibly separates words on a line.
 * The following are equivalent definitions:
 *
 * TRUE for Unicode White_Space characters except for "vertical space controls"
 * where "vertical space controls" are the following characters:
 * U+000A (LF) U+000B (VT) U+000C (FF) U+000D (CR) U+0085 (NEL) U+2028 (LS) U+2029 (PS)
 *
 * same as
 *
 * TRUE for U+0009 (TAB) and characters with general category "Zs" (space separators)
 * except Zero Width Space (ZWSP, U+200B).
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a "blank"
 *
 * @stable ICU 2.6
 */
UBool u_isblank(USChar32 c);

/**
 * Determines whether the specified code point is a hexadecimal digit.
 * This is equivalent to u_digit(c, 16)>=0.
 * True for characters with general category "Nd" (decimal digit numbers)
 * as well as Latin letters a-f and A-F in both ASCII and Fullwidth ASCII.
 * (That is, for letters with code points
 * 0041..0046, 0061..0066, FF21..FF26, FF41..FF46.)
 *
 * In order to narrow the definition of hexadecimal digits to only ASCII
 * characters, use (c<=0x7f && u_isxdigit(c)).
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a hexadecimal digit
 *
 * @stable ICU 2.6
 */
UBool u_isxdigit(USChar32 c);

/**
 * Returns the Unicode allocation block that contains the character.
 *
 * @param c the code point to be tested
 * @return the block value (UBlockCode) for c
 *
 * @see UBlockCode
 * @stable ICU 2.0
 */
UBlockCode ublock_getCode(USChar32 c);

/**
 * Get the property value for an enumerated or integer Unicode property for a code point.
 * Also returns binary and mask property values.
 *
 * Unicode, especially in version 3.2, defines many more properties than the
 * original set in UnicodeData.txt.
 *
 * The properties APIs are intended to reflect Unicode properties as defined
 * in the Unicode Character Database (UCD) and Unicode Technical Reports (UTR).
 * For details about the properties see http://www.unicode.org/ .
 * For names of Unicode properties see the UCD file PropertyAliases.txt.
 *
 * Sample usage:
 * UEastAsianWidth ea=(UEastAsianWidth)u_getIntPropertyValue(c, UCHAR_EAST_ASIAN_WIDTH);
 * UBool b=(UBool)u_getIntPropertyValue(c, UCHAR_IDEOGRAPHIC);
 *
 * @param c Code point to test.
 * @param which UProperty selector constant, identifies which property to check.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT
 *        or UCHAR_INT_START<=which<UCHAR_INT_LIMIT
 *        or UCHAR_MASK_START<=which<UCHAR_MASK_LIMIT.
 * @return Numeric value that is directly the property value or,
 *         for enumerated properties, corresponds to the numeric value of the enumerated
 *         constant of the respective property value enumeration type
 *         (cast to enum type if necessary).
 *         Returns 0 or 1 (for FALSE/TRUE) for binary Unicode properties.
 *         Returns a bit-mask for mask properties.
 *         Returns 0 if 'which' is out of bounds or if the Unicode version
 *         does not have data for the property at all, or not for this code point.
 *
 * @stable ICU 2.2
 */
int32_t u_getIntPropertyValue(USChar32 c, UProperty which);

/** 
 * Gets the script code associated with the given codepoint.
 * Returns USCRIPT_MALAYALAM given 0x0D02 
 * @param codepoint UChar32 codepoint
 * @param err the error status code.
 * @return The UScriptCode, or 0 if codepoint is invalid 
 * @stable ICU 2.4
 */
UScriptCode uscript_getScript(USChar32 c, UErrorCode *pErrorCode);

/**
 * Returns the bidirectional category value for the code point,
 * which is used in the Unicode bidirectional algorithm
 * (UAX #9 http://www.unicode.org/reports/tr9/).
 * Note that some <em>unassigned</em> code points have bidi values
 * of R or AL because they are in blocks that are reserved
 * for Right-To-Left scripts.
 *
 * Same as java.lang.Character.getDirectionality()
 *
 * @param c the code point to be tested
 * @return the bidirectional category (UCharDirection) value
 *
 * @see UCharDirection
 * @stable ICU 2.0
 */
UCharDirection u_charDirection(USChar32 c);

/**
 * Maps the specified character to a "mirror-image" character.
 * For characters with the Bidi_Mirrored property, implementations
 * sometimes need a "poor man's" mapping to another Unicode
 * character (code point) such that the default glyph may serve
 * as the mirror-image of the default glyph of the specified
 * character. This is useful for text conversion to and from
 * codepages with visual order, and for displays without glyph
 * selecetion capabilities.
 *
 * @param c the code point to be mapped
 * @return another Unicode code point that may serve as a mirror-image
 *         substitute, or c itself if there is no such mapping or c
 *         does not have the Bidi_Mirrored property
 *
 * @see UCHAR_BIDI_MIRRORED
 * @see u_isMirrored
 * @stable ICU 2.0
 */
USChar32 u_charMirror(USChar32 c);


/**
 * The given character is mapped to its lowercase equivalent according to
 * UnicodeData.txt; if the character has no lowercase equivalent, the character
 * itself is returned.
 *
 * Same as java.lang.Character.toLowerCase().
 *
 * This function only returns the simple, single-code point case mapping.
 * Full case mappings should be used whenever possible because they produce
 * better results by working on whole strings.
 * They take into account the string context and the language and can map
 * to a result string with a different length as appropriate.
 * Full case mappings are applied by the string case mapping functions,
 * see ustring.h and the UnicodeString class.
 * See also the User Guide chapter on C/POSIX migration:
 * http://icu-project.org/userguide/posix.html#case_mappings
 *
 * @param c the code point to be mapped
 * @return the Simple_Lowercase_Mapping of the code point, if any;
 *         otherwise the code point itself.
 * @stable ICU 2.0
 */
USChar32 u_tolower(USChar32 c);

/**
 * The given character is mapped to its uppercase equivalent according to UnicodeData.txt;
 * if the character has no uppercase equivalent, the character itself is
 * returned.
 *
 * Same as java.lang.Character.toUpperCase().
 *
 * This function only returns the simple, single-code point case mapping.
 * Full case mappings should be used whenever possible because they produce
 * better results by working on whole strings.
 * They take into account the string context and the language and can map
 * to a result string with a different length as appropriate.
 * Full case mappings are applied by the string case mapping functions,
 * see ustring.h and the UnicodeString class.
 * See also the User Guide chapter on C/POSIX migration:
 * http://icu-project.org/userguide/posix.html#case_mappings
 *
 * @param c the code point to be mapped
 * @return the Simple_Uppercase_Mapping of the code point, if any;
 *         otherwise the code point itself.
 * @stable ICU 2.0
 */
USChar32 u_toupper(USChar32 c);

/**
 * The given character is mapped to its titlecase equivalent
 * according to UnicodeData.txt;
 * if none is defined, the character itself is returned.
 *
 * Same as java.lang.Character.toTitleCase().
 *
 * This function only returns the simple, single-code point case mapping.
 * Full case mappings should be used whenever possible because they produce
 * better results by working on whole strings.
 * They take into account the string context and the language and can map
 * to a result string with a different length as appropriate.
 * Full case mappings are applied by the string case mapping functions,
 * see ustring.h and the UnicodeString class.
 * See also the User Guide chapter on C/POSIX migration:
 * http://icu-project.org/userguide/posix.html#case_mappings
 *
 * @param c the code point to be mapped
 * @return the Simple_Titlecase_Mapping of the code point, if any;
 *         otherwise the code point itself.
 * @stable ICU 2.0
 */
USChar32 u_totitle(USChar32 c);

/**
 * Check if a code point has the Uppercase Unicode property.
 * Same as u_hasBinaryProperty(c, UCHAR_UPPERCASE).
 * This is different from u_isupper!
 * @param c Code point to test
 * @return true if the code point has the Uppercase Unicode property, false otherwise
 *
 * @see UCHAR_UPPERCASE
 * @see u_isupper
 * @see u_hasBinaryProperty
 * @stable ICU 2.1
 */
UBool u_isUUppercase(USChar32 c);

/**
 * The given character is mapped to its case folding equivalent according to
 * UnicodeData.txt and CaseFolding.txt;
 * if the character has no case folding equivalent, the character
 * itself is returned.
 *
 * This function only returns the simple, single-code point case mapping.
 * Full case mappings should be used whenever possible because they produce
 * better results by working on whole strings.
 * They take into account the string context and the language and can map
 * to a result string with a different length as appropriate.
 * Full case mappings are applied by the string case mapping functions,
 * see ustring.h and the UnicodeString class.
 * See also the User Guide chapter on C/POSIX migration:
 * http://icu-project.org/userguide/posix.html#case_mappings
 *
 * @param c the code point to be mapped
 * @param options Either U_FOLD_CASE_DEFAULT or U_FOLD_CASE_EXCLUDE_SPECIAL_I
 * @return the Simple_Case_Folding of the code point, if any;
 *         otherwise the code point itself.
 * @stable ICU 2.0
 */
USChar32 u_foldCase(USChar32 c, uint32_t options);

/**
 * Returns the combining class of the code point as specified in UnicodeData.txt.
 *
 * @param c the code point of the character
 * @return the combining class of the character
 * @stable ICU 2.0
 */
uint8_t u_getCombiningClass(USChar32 c);

#ifdef  __cplusplus
}
#endif

#endif
