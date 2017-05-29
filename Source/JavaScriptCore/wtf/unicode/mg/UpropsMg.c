/*
** $Id: UpropsMg.c 123 2010-06-22 09:55:14Z xwyan $
**
** UpropsMg.c: Implementations for mostly non-core unicode
**             character properties.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-5-31
*/

#include "unicode/UpropsMg.h"

#include "unicode/UtypesMg.h"
#include "unicode/UcharMg.h"
#include "unicode/UnormMg.h"
#include "unicode/UbidiPropsMg.h"

/* bidi/shaping properties API ---------------------------------------------- */

/* get the UBiDiProps singleton, or else its dummy, once and for all */
static const UBiDiProps *getBiDiProps()
{
    /* the initial check is performed by the GET_BIDI_PROPS() macro */
    const UBiDiProps *bdp;
    UErrorCode errorCode = U_ZERO_ERROR;

    bdp = ubidi_getSingleton(&errorCode);
    return bdp;
}

/* general properties API functions ----------------------------------------- */

static const struct {
    int32_t column;
    uint32_t mask;
} binProps[UCHAR_BINARY_LIMIT]= {
    /*
     * column and mask values for binary properties from u_getUnicodeProperties().
     * Must be in order of corresponding UProperty,
     * and there must be exacly one entry per binary UProperty.
     *
     * Properties with mask 0 are handled in code.
     * For them, column is the UPropertySource value.
     */
    {  1,               U_MASK(UPROPS_ALPHABETIC) },
    {  1,               U_MASK(UPROPS_ASCII_HEX_DIGIT) },
    { UPROPS_SRC_BIDI,  0 },                                    /* UCHAR_BIDI_CONTROL */
    { UPROPS_SRC_BIDI,  0 },                                    /* UCHAR_BIDI_MIRRORED */
    {  1,               U_MASK(UPROPS_DASH) },
    {  1,               U_MASK(UPROPS_DEFAULT_IGNORABLE_CODE_POINT) },
    {  1,               U_MASK(UPROPS_DEPRECATED) },
    {  1,               U_MASK(UPROPS_DIACRITIC) },
    {  1,               U_MASK(UPROPS_EXTENDER) },
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_FULL_COMPOSITION_EXCLUSION */
    {  1,               U_MASK(UPROPS_GRAPHEME_BASE) },
    {  1,               U_MASK(UPROPS_GRAPHEME_EXTEND) },
    {  1,               U_MASK(UPROPS_GRAPHEME_LINK) },
    {  1,               U_MASK(UPROPS_HEX_DIGIT) },
    {  1,               U_MASK(UPROPS_HYPHEN) },
    {  1,               U_MASK(UPROPS_ID_CONTINUE) },
    {  1,               U_MASK(UPROPS_ID_START) },
    {  1,               U_MASK(UPROPS_IDEOGRAPHIC) },
    {  1,               U_MASK(UPROPS_IDS_BINARY_OPERATOR) },
    {  1,               U_MASK(UPROPS_IDS_TRINARY_OPERATOR) },
    { UPROPS_SRC_BIDI,  0 },                                    /* UCHAR_JOIN_CONTROL */
    {  1,               U_MASK(UPROPS_LOGICAL_ORDER_EXCEPTION) },
    { UPROPS_SRC_CASE,  0 },                                    /* UCHAR_LOWERCASE */
    {  1,               U_MASK(UPROPS_MATH) },
    {  1,               U_MASK(UPROPS_NONCHARACTER_CODE_POINT) },
    {  1,               U_MASK(UPROPS_QUOTATION_MARK) },
    {  1,               U_MASK(UPROPS_RADICAL) },
    { UPROPS_SRC_CASE,  0 },                                    /* UCHAR_SOFT_DOTTED */
    {  1,               U_MASK(UPROPS_TERMINAL_PUNCTUATION) },
    {  1,               U_MASK(UPROPS_UNIFIED_IDEOGRAPH) },
    { UPROPS_SRC_CASE,  0 },                                    /* UCHAR_UPPERCASE */
    {  1,               U_MASK(UPROPS_WHITE_SPACE) },
    {  1,               U_MASK(UPROPS_XID_CONTINUE) },
    {  1,               U_MASK(UPROPS_XID_START) },
    { UPROPS_SRC_CASE,  0 },                                    /* UCHAR_CASE_SENSITIVE */
    {  1,               U_MASK(UPROPS_S_TERM) },
    {  1,               U_MASK(UPROPS_VARIATION_SELECTOR) },
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_NFD_INERT */
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_NFKD_INERT */
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_NFC_INERT */
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_NFKC_INERT */
    { UPROPS_SRC_NORM,  0 },                                    /* UCHAR_SEGMENT_STARTER */
    {  1,               U_MASK(UPROPS_PATTERN_SYNTAX) },
    {  1,               U_MASK(UPROPS_PATTERN_WHITE_SPACE) },
    { UPROPS_SRC_CHAR_AND_PROPSVEC,  0 },                       /* UCHAR_POSIX_ALNUM */
    { UPROPS_SRC_CHAR,  0 },                                    /* UCHAR_POSIX_BLANK */
    { UPROPS_SRC_CHAR,  0 },                                    /* UCHAR_POSIX_GRAPH */
    { UPROPS_SRC_CHAR,  0 },                                    /* UCHAR_POSIX_PRINT */
    { UPROPS_SRC_CHAR,  0 }                                     /* UCHAR_POSIX_XDIGIT */
};

UBool u_hasBinaryProperty(USChar32 c, UProperty which)
{
    /* c is range-checked in the functions that are called from here */
    if(which < UCHAR_BINARY_START || UCHAR_BINARY_LIMIT <= which) {
        /* not a known binary property */
    } else {
        uint32_t mask=binProps[which].mask;
        int32_t column=binProps[which].column;
        if(mask!=0) {
            /* systematic, directly stored properties */
            return (u_getUnicodeProperties(c, column)&mask)!=0;
        } else {
            if(column==UPROPS_SRC_CASE) {
                return ucase_hasBinaryProperty(c, which);
            } else if(column==UPROPS_SRC_NORM) {
                switch(which) {
                case UCHAR_FULL_COMPOSITION_EXCLUSION:
                    return unorm_internalIsFullCompositionExclusion(c);
                case UCHAR_NFD_INERT:
                case UCHAR_NFKD_INERT:
                case UCHAR_NFC_INERT:
                case UCHAR_NFKC_INERT:
                    return unorm_isNFSkippable(c, (UNormalizationMode)(which-UCHAR_NFD_INERT+UNORM_NFD));
                case UCHAR_SEGMENT_STARTER:
                    return unorm_isCanonSafeStart(c);
                default:
                    break;
                }
            } else if(column==UPROPS_SRC_BIDI) {
                /* bidi/shaping properties */
                const UBiDiProps *bdp=getBiDiProps();
                if(bdp!=NULL) {
                    switch(which) {
                    case UCHAR_BIDI_MIRRORED:
                        return ubidi_isMirrored(bdp, c);
                    case UCHAR_BIDI_CONTROL:
                        return ubidi_isBidiControl(bdp, c);
                    case UCHAR_JOIN_CONTROL:
                        return ubidi_isJoinControl(bdp, c);
                    default:
                        break;
                    }
                }
            } else if(column==UPROPS_SRC_CHAR) {
                switch(which) {
                case UCHAR_POSIX_BLANK:
                    return u_isblank(c);
                case UCHAR_POSIX_GRAPH:
                    return u_isgraphPOSIX(c);
                case UCHAR_POSIX_PRINT:
                    return u_isprintPOSIX(c);
                case UCHAR_POSIX_XDIGIT:
                    return u_isxdigit(c);
                default:
                    break;
                }
            } else if(column==UPROPS_SRC_CHAR_AND_PROPSVEC) {
                switch(which) {
                case UCHAR_POSIX_ALNUM:
                    return u_isalnumPOSIX(c);
                default:
                    break;
                }
            }
        }
    }
    return FALSE;
}


int32_t u_getIntPropertyValue(USChar32 c, UProperty which)
{
    UErrorCode errorCode;
    int32_t type;

    if(which<UCHAR_BINARY_START) {
        return 0; /* undefined */
    } else if(which<UCHAR_BINARY_LIMIT) {
        return (int32_t)u_hasBinaryProperty(c, which);
    } else if(which<UCHAR_INT_START) {
        return 0; /* undefined */
    } else if(which<UCHAR_INT_LIMIT) {
        switch(which) {
        case UCHAR_BIDI_CLASS:
            return (int32_t)u_charDirection(c);
        case UCHAR_BLOCK:
            return (int32_t)ublock_getCode(c);
        case UCHAR_CANONICAL_COMBINING_CLASS:
            return u_getCombiningClass(c);
        case UCHAR_DECOMPOSITION_TYPE:
            return (int32_t)(u_getUnicodeProperties(c, 2)&UPROPS_DT_MASK);
        case UCHAR_EAST_ASIAN_WIDTH:
            return (int32_t)(u_getUnicodeProperties(c, 0)&UPROPS_EA_MASK)>>UPROPS_EA_SHIFT;
        case UCHAR_GENERAL_CATEGORY:
            return (int32_t)u_charType(c);
        case UCHAR_JOINING_GROUP:
            return ubidi_getJoiningGroup(getBiDiProps(), c);
        case UCHAR_JOINING_TYPE:
            return ubidi_getJoiningType(getBiDiProps(), c);
        case UCHAR_LINE_BREAK:
            return (int32_t)(u_getUnicodeProperties(c, UPROPS_LB_VWORD)&UPROPS_LB_MASK)>>UPROPS_LB_SHIFT;
        case UCHAR_NUMERIC_TYPE:
            type=(int32_t)GET_NUMERIC_TYPE(u_getUnicodeProperties(c, -1));
            if(type>U_NT_NUMERIC) {
                /* keep internal variants of U_NT_NUMERIC from becoming visible */
                type=U_NT_NUMERIC;
            }
            return type;
        case UCHAR_SCRIPT:
            errorCode=U_ZERO_ERROR;
            return (int32_t)uscript_getScript(c, &errorCode);
        case UCHAR_HANGUL_SYLLABLE_TYPE:
            return uchar_getHST(c);
        case UCHAR_NFD_QUICK_CHECK:
        case UCHAR_NFKD_QUICK_CHECK:
        case UCHAR_NFC_QUICK_CHECK:
        case UCHAR_NFKC_QUICK_CHECK:
            return (int32_t)unorm_getQuickCheck(c, (UNormalizationMode)(which-UCHAR_NFD_QUICK_CHECK+UNORM_NFD));
        case UCHAR_LEAD_CANONICAL_COMBINING_CLASS:
            return unorm_getFCD16FromCodePoint(c)>>8;
        case UCHAR_TRAIL_CANONICAL_COMBINING_CLASS:
            return unorm_getFCD16FromCodePoint(c)&0xff;
        case UCHAR_GRAPHEME_CLUSTER_BREAK:
            return (int32_t)(u_getUnicodeProperties(c, 2)&UPROPS_GCB_MASK)>>UPROPS_GCB_SHIFT;
        case UCHAR_SENTENCE_BREAK:
            return (int32_t)(u_getUnicodeProperties(c, 2)&UPROPS_SB_MASK)>>UPROPS_SB_SHIFT;
        case UCHAR_WORD_BREAK:
            return (int32_t)(u_getUnicodeProperties(c, 2)&UPROPS_WB_MASK)>>UPROPS_WB_SHIFT;
        default:
            return 0; /* undefined */
        }
    } else if(which==UCHAR_GENERAL_CATEGORY_MASK) {
        return U_MASK(u_charType(c));
    } else {
        return 0; /* undefined */
    }
}


