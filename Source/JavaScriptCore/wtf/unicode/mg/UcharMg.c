/*
** $Id: UcharMg.c 123 2010-06-22 09:55:14Z xwyan $
**
** UcharMg.c: Implementation for unicode characters.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-25
*/

#include  "unicode/UcharMg.h"
#include  "unicode/UpropsMg.h"

#include  "UtrieMg.h"
#include  "UcharPropsDataMg.c"

#define GET_PROPS(c, result) UTRIE_GET16(&propsTrie, c, result)

/* Checks if c is a letter or a decimal digit */
UBool u_isalnum(UChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_ND_MASK))!=0);
}

/* Checks if c is a decimal digit. */
UBool u_isdigit(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

/* Gets the Unicode character's general category.*/
int8_t u_charType(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (int8_t)GET_CATEGORY(props);
}

int32_t u_charDigitValue(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);

    if(GET_NUMERIC_TYPE(props)==1) {
        return GET_NUMERIC_VALUE(props);
    } else {
        return -1;
    }
}

/* Some control characters that are used as space. */
#define IS_THAT_CONTROL_SPACE(c) \
    (c<=0x9f && ((c>=0x09 && c<=0x0d) || (c>=0x1c && c <=0x1f) || c==0x85))
//TAB=0x09, CR=0x0d, NL=0x85

/* Checks if the Unicode character is a space character.*/
UBool u_isspace(USChar32 c) {
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_Z_MASK)!=0 || IS_THAT_CONTROL_SPACE(c));
}

UBool u_ispunct(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)((CAT_MASK(props)&U_GC_P_MASK)!=0);
}

/* Checks if c is a lower case letter.*/
UBool u_islower(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_LOWERCASE_LETTER);
}

uint32_t u_getUnicodeProperties(USChar32 c, int32_t column)
{
    uint16_t vecIndex;

    if(column==-1) {
        uint32_t props;
        GET_PROPS(c, props);
        return props;
    } else if(column<0 || column>=propsVectorsColumns) {
        return 0;
    } else {
        UTRIE_GET16(&propsVectorsTrie, c, vecIndex);
        return propsVectors[vecIndex+column];
    }
}

UBool u_isblank(USChar32 c)
{
    if((uint32_t)c<=0x9f) {
        return c==9 || c==0x20; /* TAB or SPACE */
    } else {
        /* Zs */
        uint32_t props;
        GET_PROPS(c, props);
        return (UBool)(GET_CATEGORY(props)==U_SPACE_SEPARATOR);
    }
}

/**
 * Checks if c is in
 * [^\p{space}\p{gc=Control}\p{gc=Surrogate}\p{gc=Unassigned}]
 * with space=\p{Whitespace} and Control=Cc.
 * Implements UCHAR_POSIX_GRAPH.
 * @internal
 */
UBool u_isgraphPOSIX(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    /* \p{space}\p{gc=Control} == \p{gc=Z}\p{Control} */
    /* comparing ==0 returns FALSE for the categories mentioned */
    return (UBool)((CAT_MASK(props)&
                    (U_GC_CC_MASK|U_GC_CS_MASK|U_GC_CN_MASK|U_GC_Z_MASK))
                   ==0);
}

/**
 * Checks if c is in \p{graph}\p{blank} - \p{cntrl}.
 * Implements UCHAR_POSIX_PRINT.
 * @internal
 */
UBool u_isprintPOSIX(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    /*
     * The only cntrl character in graph+blank is TAB (in blank).
     * Here we implement (blank-TAB)=Zs instead of calling u_isblank().
     */
    return (UBool)((GET_CATEGORY(props)==U_SPACE_SEPARATOR) || u_isgraphPOSIX(c));
}

UBool u_isxdigit(USChar32 c)
{
    uint32_t props;
    /* check ASCII and Fullwidth ASCII a-fA-F */
    if(
        (c<=0x66 && c>=0x41 && (c<=0x46 || c>=0x61)) ||
        (c>=0xff21 && c<=0xff46 && (c<=0xff26 || c>=0xff41))
    ) {
        return TRUE;
    }

    GET_PROPS(c, props);
    return (UBool)(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

UBool u_isUAlphabetic(USChar32 c)
{
    return (u_getUnicodeProperties(c, 1)&U_MASK(UPROPS_ALPHABETIC))!=0;
}

/**
 * Checks if c is alphabetic, or a decimal digit; implements UCHAR_POSIX_ALNUM.
 * @internal
 */
UBool u_isalnumPOSIX(USChar32 c)
{
    return (UBool)(u_isUAlphabetic(c) || u_isdigit(c));
}

UBlockCode ublock_getCode(USChar32 c)
{
    return (UBlockCode)((u_getUnicodeProperties(c, 0)&UPROPS_BLOCK_MASK)>>UPROPS_BLOCK_SHIFT);
}

UScriptCode uscript_getScript(USChar32 c, UErrorCode *pErrorCode)
{
    if(pErrorCode==NULL || U_FAILURE(*pErrorCode)) {
        return USCRIPT_INVALID_CODE;
    }
    if((uint32_t)c>0x10ffff) {
        *pErrorCode=U_ILLEGAL_ARGUMENT_ERROR;
        return USCRIPT_INVALID_CODE;
    }

    return (UScriptCode)(u_getUnicodeProperties(c, 0)&UPROPS_SCRIPT_MASK);
}

/*
 * get Hangul Syllable Type
 */
UHangulSyllableType
uchar_getHST(USChar32 c)
{
    /* purely algorithmic; hardcode known characters, check for assigned new ones */
    if(c<JAMO_L_BASE) {
        /* U_HST_NOT_APPLICABLE */
    } else if(c<=0x11ff) {
        /* Jamo range */
        if(c<=0x115f) {
            /* Jamo L range, HANGUL CHOSEONG ... */
            if(c==0x115f || c<=0x1159 || u_charType(c)==U_OTHER_LETTER) {
                return U_HST_LEADING_JAMO;
            }
        } else if(c<=0x11a7) {
            /* Jamo V range, HANGUL JUNGSEONG ... */
            if(c<=0x11a2 || u_charType(c)==U_OTHER_LETTER) {
                return U_HST_VOWEL_JAMO;
            }
        } else {
            /* Jamo T range */
            if(c<=0x11f9 || u_charType(c)==U_OTHER_LETTER) {
                return U_HST_TRAILING_JAMO;
            }
        }
    } else if((c-=HANGUL_BASE)<0) {
        /* U_HST_NOT_APPLICABLE */
    } else if(c<HANGUL_COUNT) {
        /* Hangul syllable */
        return c%JAMO_T_COUNT==0 ? U_HST_LV_SYLLABLE : U_HST_LVT_SYLLABLE;
    }
    return U_HST_NOT_APPLICABLE;
}

/* Checks if the Unicode character is printable.*/
UBool u_isprint(USChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    /* comparing ==0 returns FALSE for the categories mentioned */
    return (UBool)((CAT_MASK(props)&U_GC_C_MASK)==0);
}
