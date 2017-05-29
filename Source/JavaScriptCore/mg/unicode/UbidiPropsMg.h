/*
** $Id: UbidiPropsMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UbidiPropsMg.h: unicode biditext implements.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-24
*/

#ifndef UbidiPropsMg_h
#define UbidiPropsMg_h

#include  "unicode/UtypesMg.h"
#include  "unicode/UcharMg.h"

struct UBiDiProps;
typedef struct UBiDiProps UBiDiProps;


/* indexes into indexes[] */
enum {
    UBIDI_IX_INDEX_TOP,
    UBIDI_IX_LENGTH,
    UBIDI_IX_TRIE_SIZE,
    UBIDI_IX_MIRROR_LENGTH,

    UBIDI_IX_JG_START,
    UBIDI_IX_JG_LIMIT,

    UBIDI_MAX_VALUES_INDEX=15,
    UBIDI_IX_TOP=16
};

/* definitions for 16-bit bidi/shaping properties word ---------------------- */

enum {
 /* UBIDI_CLASS_SHIFT=0, */     /* bidi class: 5 bits (4..0) */
    UBIDI_JT_SHIFT=5,           /* joining type: 3 bits (7..5) */

    /* UBIDI__SHIFT=8, reserved: 2 bits (9..8) */

    UBIDI_JOIN_CONTROL_SHIFT=10,
    UBIDI_BIDI_CONTROL_SHIFT=11,

    UBIDI_IS_MIRRORED_SHIFT=12,         /* 'is mirrored' */
    UBIDI_MIRROR_DELTA_SHIFT=13,        /* bidi mirroring delta: 3 bits (15..13) */

    UBIDI_MAX_JG_SHIFT=16               /* max JG value in indexes[UBIDI_MAX_VALUES_INDEX] bits 23..16 */
};

#define UBIDI_CLASS_MASK        0x0000001f
#define UBIDI_JT_MASK           0x000000e0

#define UBIDI_GET_CLASS(props) ((props)&UBIDI_CLASS_MASK)
#define UBIDI_GET_FLAG(props, shift) (((props)>>(shift))&1)

enum {
    UBIDI_ESC_MIRROR_DELTA=-4,
    UBIDI_MIN_MIRROR_DELTA=-3,
    UBIDI_MAX_MIRROR_DELTA=3
};

/* definitions for 32-bit mirror table entry -------------------------------- */

enum {
    /* the source Unicode code point takes 21 bits (20..0) */
    UBIDI_MIRROR_INDEX_SHIFT=21,
    UBIDI_MAX_MIRROR_INDEX=0x7ff
};

#define UBIDI_GET_MIRROR_CODE_POINT(m) (USChar32)((m)&0x1fffff)

#define UBIDI_GET_MIRROR_INDEX(m) ((m)>>UBIDI_MIRROR_INDEX_SHIFT)

#ifdef  __cplusplus
extern "C" {
#endif

UBool ubidi_isMirrored(const UBiDiProps *bdp, USChar32 c);
UBool ubidi_isBidiControl(const UBiDiProps *bdp, USChar32 c);
UBool ubidi_isJoinControl(const UBiDiProps *bdp, USChar32 c);

UJoiningType ubidi_getJoiningType(const UBiDiProps *bdp, USChar32 c);
UJoiningGroup ubidi_getJoiningGroup(const UBiDiProps *bdp, USChar32 c);

const UBiDiProps* ubidi_getSingleton(UErrorCode *pErrorCode);

#ifdef __cplusplus
}
#endif


#endif
