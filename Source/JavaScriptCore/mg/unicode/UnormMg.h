/*
** $Id: UnormMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UnormMg.h: header file for unicode.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-6-2
*/

#ifndef UnormMg_h
#define UnormMg_h

#include  "unicode/UtypesMg.h"
#include  "unicode/UcharMg.h"

/**
 * Constants for options flags for normalization.
 * Use 0 for default options,
 */
enum {
    /**
     * Options bit set value to select Unicode 3.2 normalization
     * (except NormalizationCorrections).
     * At most one Unicode version can be selected at a time.
     */
    UNORM_UNICODE_3_2=0x20
};

/* value constants for auxTrie */
enum {
    _NORM_AUX_COMP_EX_SHIFT=10,
    _NORM_AUX_UNSAFE_SHIFT=11,
    _NORM_AUX_NFC_SKIPPABLE_F_SHIFT=12
};

#define _NORM_AUX_MAX_FNC           ((int32_t)1<<_NORM_AUX_COMP_EX_SHIFT)

#define _NORM_AUX_FNC_MASK          (uint32_t)(_NORM_AUX_MAX_FNC-1)
#define _NORM_AUX_COMP_EX_MASK      ((uint32_t)1<<_NORM_AUX_COMP_EX_SHIFT)
#define _NORM_AUX_UNSAFE_MASK       ((uint32_t)1<<_NORM_AUX_UNSAFE_SHIFT)
#define _NORM_AUX_NFC_SKIP_F_MASK   ((uint32_t)1<<_NORM_AUX_NFC_SKIPPABLE_F_SHIFT)


/* norm32 value constants using >16 bits */
#define _NORM_MIN_SPECIAL       0xfc000000
#define _NORM_SURROGATES_TOP    0xfff00000
#define _NORM_MIN_HANGUL        0xfff00000
#define _NORM_MIN_JAMO_V        0xfff20000
#define _NORM_JAMO_V_TOP        0xfff30000

/**
 * Bitmask values to be passed to uset_openPatternOptions() or
 * uset_applyPattern() taking an option parameter.
 * @stable ICU 2.4
 */
enum {
    /**
     * Ignore white space within patterns unless quoted or escaped.
     */
    USET_IGNORE_SPACE = 1,  

    USET_CASE_INSENSITIVE = 2,  

    USET_ADD_CASE_MAPPINGS = 4,
    
    USET_SERIALIZED_STATIC_ARRAY_CAPACITY=8
};


/**
 * Internal option for unorm_cmpEquivFold() for strncmp style.
 * If set, checks for both string length and terminating NUL.
 * @internal
 */
#define _STRNCMP_STYLE 0x1000

/**
 * Option bit for unorm_compare:
 * Perform case-insensitive comparison.
 * @draft ICU 2.2
 */
#define U_COMPARE_IGNORE_CASE       0x10000

#ifndef HAVE_UNICODE_UTF_OLD_H

#define UTF_IS_SECOND_SURROGATE(uchar)  U16_IS_TRAIL(uchar)
#define UTF_IS_TRAIL(uchar)             U16_IS_TRAIL(uchar)
#define UTF_IS_SURROGATE(uchar)         U_IS_SURROGATE(uchar)
#define UTF_IS_SURROGATE_FIRST(c)       U16_IS_SURROGATE_LEAD(c)
#define UTF_IS_FIRST_SURROGATE(uchar)   U_IS_SURROGATE(uchar)
#define UTF_IS_LEAD(uchar)              U16_IS_LEAD(uchar)

#endif /* HAVE_UNICODE_UTF_OLD_H */

enum {
    /* FCD check: everything below this code point is known to have a 0 lead combining class */
    _NORM_MIN_WITH_LEAD_CC=0x300
};

enum {
    /**
     * Bit 7 of the length byte for a decomposition string in extra data is
     * a flag indicating whether the decomposition string is
     * preceded by a 16-bit word with the leading and trailing cc
     * of the decomposition (like for A-umlaut);
     * if not, then both cc's are zero (like for compatibility ideographs).
     */
    _NORM_DECOMP_FLAG_LENGTH_HAS_CC=0x80,
    /**
     * Bits 6..0 of the length byte contain the actual length.
     */
    _NORM_DECOMP_LENGTH_MASK=0x7f
};

typedef enum UNormalizationCheckResult {
  /** 
   * Indicates that string is not in the normalized format
   */
  UNORM_NO,
  /** 
   * Indicates that string is in the normalized format
   */
  UNORM_YES,
  /** 
   * Indicates that string cannot be determined if it is in the normalized 
   * format without further thorough checks.
   */
  UNORM_MAYBE
} UNormalizationCheckResult;

/* Constants for options flags for normalization. */
enum {
    UNORM_NX_HANGUL=1,
    UNORM_NX_CJK_COMPAT=2,
    UNORM_BEFORE_PRI_29=0x100
};

/**
 * Constants for normalization modes.
 * @stable ICU 2.0
 */
typedef enum {
  UNORM_NONE = 1, 
  UNORM_NFD = 2,
  UNORM_NFKD = 3,
  UNORM_NFC = 4,
  UNORM_DEFAULT = UNORM_NFC, 
  UNORM_NFKC =5,
  UNORM_FCD = 6,

  UNORM_MODE_COUNT
} UNormalizationMode;


enum {
    /* quick check flags 0..3 set mean "no" for their forms */
    _NORM_QC_NFC=0x11,          /* no|maybe */
    _NORM_QC_NFKC=0x22,         /* no|maybe */
    _NORM_QC_NFD=4,             /* no */
    _NORM_QC_NFKD=8,            /* no */

    _NORM_QC_ANY_NO=0xf,

    /* quick check flags 4..5 mean "maybe" for their forms; test flags>=_NORM_QC_MAYBE */
    _NORM_QC_MAYBE=0x10,
    _NORM_QC_ANY_MAYBE=0x30,

    _NORM_QC_MASK=0x3f,

    _NORM_COMBINES_FWD=0x40,
    _NORM_COMBINES_BACK=0x80,
    _NORM_COMBINES_ANY=0xc0,

    _NORM_CC_SHIFT=8,           /* UnicodeData.txt combining class in bits 15..8 */
    _NORM_CC_MASK=0xff00,

    _NORM_EXTRA_SHIFT=16,               /* 16 bits for the index to UChars and other extra data */
    _NORM_EXTRA_INDEX_TOP=0xfc00,       /* start of surrogate specials after shift */

    _NORM_EXTRA_SURROGATE_MASK=0x3ff,
    _NORM_EXTRA_SURROGATE_TOP=0x3f0,    /* hangul etc. */

    _NORM_EXTRA_HANGUL=_NORM_EXTRA_SURROGATE_TOP,
    _NORM_EXTRA_JAMO_L,
    _NORM_EXTRA_JAMO_V,
    _NORM_EXTRA_JAMO_T
};


enum {
    JAMO_L_BASE=0x1100,     /* "lead" jamo */
    JAMO_V_BASE=0x1161,     /* "vowel" jamo */
    JAMO_T_BASE=0x11a7,     /* "trail" jamo */

    HANGUL_BASE=0xac00,

    JAMO_L_COUNT=19,
    JAMO_V_COUNT=21,
    JAMO_T_COUNT=28,

    HANGUL_COUNT=JAMO_L_COUNT*JAMO_V_COUNT*JAMO_T_COUNT
};


/* indexes[] value names */
enum {
    _NORM_INDEX_TRIE_SIZE,              /* number of bytes in normalization trie */
    _NORM_INDEX_UCHAR_COUNT,            /* number of UChars in extra data */

    _NORM_INDEX_COMBINE_DATA_COUNT,     /* number of uint16_t words for combining data */
    _NORM_INDEX_COMBINE_FWD_COUNT,      /* number of code points that combine forward */
    _NORM_INDEX_COMBINE_BOTH_COUNT,     /* number of code points that combine forward and backward */
    _NORM_INDEX_COMBINE_BACK_COUNT,     /* number of code points that combine backward */

    _NORM_INDEX_MIN_NFC_NO_MAYBE,       /* first code point with quick check NFC NO/MAYBE */
    _NORM_INDEX_MIN_NFKC_NO_MAYBE,      /* first code point with quick check NFKC NO/MAYBE */
    _NORM_INDEX_MIN_NFD_NO_MAYBE,       /* first code point with quick check NFD NO/MAYBE */
    _NORM_INDEX_MIN_NFKD_NO_MAYBE,      /* first code point with quick check NFKD NO/MAYBE */

    _NORM_INDEX_FCD_TRIE_SIZE,          /* number of bytes in FCD trie */

    _NORM_INDEX_AUX_TRIE_SIZE,          /* number of bytes in the auxiliary trie */
    _NORM_INDEX_CANON_SET_COUNT,        /* number of uint16_t in the array of serialized USet */

    _NORM_INDEX_TOP=32                  /* changing this requires a new formatVersion */
};

/*
Please keep the order of enums declared in same order
as the cleanup functions are suppose to be called. */
typedef enum ECleanupCommonType {
    UCLN_COMMON_START = -1,
    UCLN_COMMON_USPREP,
    UCLN_COMMON_BREAKITERATOR,
    UCLN_COMMON_BREAKITERATOR_DICT,
    UCLN_COMMON_SERVICE,
    UCLN_COMMON_URES,
    UCLN_COMMON_LOCALE,
    UCLN_COMMON_ULOC,
    UCLN_COMMON_UNORM,
    UCLN_COMMON_USET,
    UCLN_COMMON_UNAMES,
    UCLN_COMMON_PNAME,
    UCLN_COMMON_UPROPS,
    UCLN_COMMON_UBIDI,
    UCLN_COMMON_UCASE,
    UCLN_COMMON_UCHAR,
    UCLN_COMMON_UCNV,
    UCLN_COMMON_UCNV_IO,
    UCLN_COMMON_UDATA,
    UCLN_COMMON_PUTIL,
    UCLN_COMMON_COUNT /* This must be last */
} ECleanupCommonType;

/* canonStartSets[0..31] contains indexes for what is in the array */
enum {
    _NORM_SET_INDEX_CANON_SETS_LENGTH,      /* number of uint16_t in canonical starter sets */
    _NORM_SET_INDEX_CANON_BMP_TABLE_LENGTH, /* number of uint16_t in the BMP search table (contains pairs) */
    _NORM_SET_INDEX_CANON_SUPP_TABLE_LENGTH,/* number of uint16_t in the supplementary search table (contains triplets) */

    /* from formatVersion 2.3: */
    _NORM_SET_INDEX_NX_CJK_COMPAT_OFFSET,   /* uint16_t offset from canonStartSets[0] to the
                                               exclusion set for CJK compatibility characters */
    _NORM_SET_INDEX_NX_UNICODE32_OFFSET,    /* uint16_t offset from canonStartSets[0] to the
                                               exclusion set for Unicode 3.2 characters */
    _NORM_SET_INDEX_NX_RESERVED_OFFSET,     /* uint16_t offset from canonStartSets[0] to the
                                               end of the previous exclusion set */

    _NORM_SET_INDEX_TOP=32                  /* changing this requires a new formatVersion */
};


typedef enum ECleanupLibraryType {
    UCLN_START = -1,
    UCLN_CUSTOM,    /* Custom is for anyone else. */
    UCLN_CTESTFW,
    UCLN_LAYOUTEX,
    UCLN_LAYOUT,
    UCLN_IO,
    UCLN_I18N,
    UCLN_COMMON /* This must be the last one to cleanup. */
} ECleanupLibraryType;

/**
 * Bitmask values to be passed to uset_openPatternOptions() or
 * uset_applyPattern() taking an option parameter.
 */
typedef struct USerializedSet {
    const uint16_t *array;
    int32_t bmpLength;
    int32_t length;
    uint16_t staticArray[USET_SERIALIZED_STATIC_ARRAY_CAPACITY];
} USerializedSet;

int32_t getFoldingNormOffset(uint32_t norm32);
int32_t getFoldingAuxOffset(uint32_t data);

#ifdef __cplusplus
extern "C" {
#endif

UBool unorm_internalIsFullCompositionExclusion(USChar32 c);
UBool unorm_isCanonSafeStart(USChar32 c);
UBool unorm_isNFSkippable(USChar32 c, UNormalizationMode mode);
UNormalizationCheckResult unorm_getQuickCheck(USChar32 c, UNormalizationMode mode);
uint16_t unorm_getFCD16FromCodePoint(USChar32 c);

int32_t unorm_normalize(const UChar *src, int32_t srcLength, 
        UNormalizationMode mode, int32_t options, UChar *dest, 
        int32_t destCapacity, UErrorCode *pErrorCode);

/**
 * Performing quick check on a string, to quickly determine if the string is 
 * in a particular normalization format.
 * Three types of result can be returned UNORM_YES, UNORM_NO or
 * UNORM_MAYBE. Result UNORM_YES indicates that the argument
 * string is in the desired normalized format, UNORM_NO determines that
 * argument string is not in the desired normalized format. A 
 * UNORM_MAYBE result indicates that a more thorough check is required, 
 * the user may have to put the string in its normalized form and compare the 
 * results.
 *
 * @param src          string for determining if it is in a normalized format
 * @param srcLength    length of source to test, or -1 if NUL-terminated
 * @param mode         which normalization form to test for
 * @param pErrorCode   a pointer to a UErrorCode to receive any errors
 * @return UNORM_YES, UNORM_NO or UNORM_MAYBE
 *
 */
UNormalizationCheckResult unorm_quickCheck(const UChar *src, 
        int32_t srcLength, UNormalizationMode mode, UErrorCode *pErrorCode);

UBool u_lib_cleanup(void);

#ifdef  __cplusplus
}
#endif

#endif
