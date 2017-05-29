/*
** $Id: UtrieMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UtrieMg.h: header file for unicode.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-6-16
*/

#ifndef UtrieMg_h
#define UtrieMg_h

#include  "unicode/UtypesMg.h"

/**
 * Runtime UTrie callback function.
 * Extract from a lead surrogate's data the
 * index array offset of the indexes for that lead surrogate.
 *
 * @param data data value for a surrogate from the trie, including the folding offset
 * @return offset>=UTRIE_BMP_INDEX_LENGTH, or 0 if there is no data for the lead surrogate
 */
typedef int32_t UTrieGetFoldingOffset(uint32_t data);

/**
 * Run-time Trie structure.
 *
 * Either the data table is 16 bits wide and accessed via the index
 * pointer, with each index item increased by indexLength;
 * in this case, data32==NULL.
 *
 * Or the data table is 32 bits wide and accessed via the data32 pointer.
 */
struct UTrie {
    const uint16_t *index;
    const uint32_t *data32; /* NULL if 16b data is used via index */

    /**
     * This function is not used in _FROM_LEAD, _FROM_BMP, and _FROM_OFFSET_TRAIL macros.
     * If convenience macros like _GET16 or _NEXT32 are used, this function must be set.
     *
     * utrie_unserialize() sets a default function which simply returns
     * the lead surrogate's value itself - which is the inverse of the default
     * folding function used by utrie_serialize().
     *
     * @see UTrieGetFoldingOffset
     */
    UTrieGetFoldingOffset *getFoldingOffset;

    int32_t indexLength, dataLength;
    uint32_t initialValue;
    char isLatin1Linear;
};

typedef struct UTrie UTrie;

/**
 * Trie constants, defining shift widths, index array lengths, etc.
 */
enum {
    /** Shift size for shifting right the input index. 1..9 */
    UTRIE_SHIFT=5,

    /** Number of data values in a stage 2 (data array) block. 2, 4, 8, .., 0x200 */
    UTRIE_DATA_BLOCK_LENGTH=1<<UTRIE_SHIFT,

    /** Mask for getting the lower bits from the input index. */
    UTRIE_MASK=UTRIE_DATA_BLOCK_LENGTH-1,

    /**
     * Lead surrogate code points' index displacement in the index array.
     * 0x10000-0xd800=0x2800
     */
    UTRIE_LEAD_INDEX_DISP=0x2800>>UTRIE_SHIFT,

    /**
     * Shift size for shifting left the index array values.
     * Increases possible data size with 16-bit index values at the cost
     * of compactability.
     * This requires blocks of stage 2 data to be aligned by UTRIE_DATA_GRANULARITY.
     * 0..UTRIE_SHIFT
     */
    UTRIE_INDEX_SHIFT=2,

    /** The alignment size of a stage 2 data block. Also the granularity for compaction. */
    UTRIE_DATA_GRANULARITY=1<<UTRIE_INDEX_SHIFT,

    /** Number of bits of a trail surrogate that are used in index table lookups. */
    UTRIE_SURROGATE_BLOCK_BITS=10-UTRIE_SHIFT,

    /**
     * Number of index (stage 1) entries per lead surrogate.
     * Same as number of index entries for 1024 trail surrogates,
     * ==0x400>>UTRIE_SHIFT
     */
    UTRIE_SURROGATE_BLOCK_COUNT=(1<<UTRIE_SURROGATE_BLOCK_BITS),

    /** Length of the BMP portion of the index (stage 1) array. */
    UTRIE_BMP_INDEX_LENGTH=0x10000>>UTRIE_SHIFT
};

/** Internal trie getter from an offset (0 if c16 is a BMP/lead units) and a 16-bit unit */
#define _UTRIE_GET_RAW(trie, data, offset, c16) \
    (trie)->data[ \
        ((int32_t)((trie)->index[(offset)+((c16)>>UTRIE_SHIFT)])<<UTRIE_INDEX_SHIFT)+ \
        ((c16)&UTRIE_MASK) \
    ]

/** Internal trie getter from a pair of surrogates */
#define _UTRIE_GET_FROM_PAIR(trie, data, c, c2, result, resultType) { \
    int32_t __offset; \
\
    /* get data for lead surrogate */ \
    (result)=_UTRIE_GET_RAW((trie), data, 0, (c)); \
    __offset=(trie)->getFoldingOffset(result); \
\
    /* get the real data from the folded lead/trail units */ \
    if(__offset>0) { \
        (result)=_UTRIE_GET_RAW((trie), data, __offset, (c2)&0x3ff); \
    } else { \
        (result)=(resultType)((trie)->initialValue); \
    } \
}

/** Internal trie getter from a BMP code point, treating a lead surrogate as a normal code point */
#define _UTRIE_GET_FROM_BMP(trie, data, c16) \
    _UTRIE_GET_RAW(trie, data, 0xd800<=(c16) && (c16)<=0xdbff ? UTRIE_LEAD_INDEX_DISP : 0, c16);

/**
 * Internal trie getter from a code point.
 * Could be faster(?) but longer with
 *   if((c32)<=0xd7ff) { (result)=_UTRIE_GET_RAW(trie, data, 0, c32); }
 */
#define _UTRIE_GET(trie, data, c32, result, resultType) \
    if((uint32_t)(c32)<=0xffff) { \
        /* BMP code points */ \
        (result)=_UTRIE_GET_FROM_BMP(trie, data, c32); \
    } else if((uint32_t)(c32)<=0x10ffff) { \
        /* supplementary code point */ \
        UChar __lead16=U16_LEAD(c32); \
        _UTRIE_GET_FROM_PAIR(trie, data, __lead16, c32, result, resultType); \
    } else { \
        /* out of range */ \
        (result)=(resultType)((trie)->initialValue); \
    }

/**
 * Get a 16-bit trie value from a code point.
 * Even lead surrogate code points are treated as normal code points,
 * with unfolded values that may differ from _FROM_LEAD() macro results for them.
 *
 * @param trie (const UTrie *, in) a pointer to the runtime trie structure
 * @param c32 (UChar32, in) the input code point
 * @param result (uint16_t, out) uint16_t variable for the trie lookup result
 */
#define UTRIE_GET16(trie, c32, result) _UTRIE_GET(trie, index, c32, result, uint16_t)

/**
 * Get a 32-bit trie value from a code point.
 * Even lead surrogate code points are treated as normal code points,
 * with unfolded values that may differ from _FROM_LEAD() macro results for them.
 *
 * @param trie (const UTrie *, in) a pointer to the runtime trie structure
 * @param c32 (UChar32, in) the input code point
 * @param result (uint32_t, out) uint32_t variable for the trie lookup result
 */
#define UTRIE_GET32(trie, c32, result) _UTRIE_GET(trie, data32, c32, result, uint32_t)

/**
 * Get a 32-bit trie value from a BMP code point (UChar, <=U+ffff).
 * c16 may be a lead surrogate, which may have a value including a folding offset.
 *
 * @param trie (const UTrie *, in) a pointer to the runtime trie structure
 * @param c16 (UChar, in) the input BMP code point
 * @return (uint32_t) trie lookup result
 */
#define UTRIE_GET32_FROM_LEAD(trie, c16) _UTRIE_GET_RAW(trie, data32, 0, c16)

/**
 * Get a 32-bit trie value from a folding offset (from the value of a lead surrogate)
 * and a trail surrogate.
 *
 * @param trie (const UTrie *, in) a pointer to the runtime trie structure
 * @param offset (int32_t, in) the folding offset from the value of a lead surrogate
 * @param c2 (UChar, in) a trail surrogate (only the 10 low bits are significant)
 * @return (uint32_t) trie lookup result
 */
#define UTRIE_GET32_FROM_OFFSET_TRAIL(trie, offset, c2) _UTRIE_GET_RAW(trie, data32, offset, (c2)&0x3ff)

#ifdef  __cplusplus
extern  "C" {
#endif

/**
 * Default implementation for UTrie.getFoldingOffset, set automatically by
 * utrie_unserialize().
 * Simply returns the lead surrogate's value itself - which is the inverse
 * of the default folding function used by utrie_serialize().
 * Exported for static const UTrie structures.
 *
 * @see UTrieGetFoldingOffset
 */
int32_t utrie_defaultGetFoldingOffset(uint32_t data);

#ifdef  __cplusplus
}
#endif
#endif
