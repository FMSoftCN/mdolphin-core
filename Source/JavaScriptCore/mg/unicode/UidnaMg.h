/*
** $Id: UidnaMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UidnaMg.h: head file for uidna.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-5-31
*/

#ifndef  _uidna_mg_h
#define  _uidna_mg_h

#include  "unicode/UcharMg.h"
#include  "unicode/UtypesMg.h"

typedef  struct tagUIDNADB{
        unsigned int   startcode;
        unsigned int   endcode;
}UIDNADB;


typedef  struct tagOneMapOne{
        unsigned int  origincode;
        unsigned int  mapcode;
}ONEMAPONE;


typedef  struct tagOneMapTwo{
        unsigned int  origincode;
        unsigned int  mapcode1;
        unsigned int  mapcode2;
}ONEMAPTWO;


typedef  struct tagOneMapThree{
        unsigned int  origincode;
        unsigned int  mapcode1;
        unsigned int  mapcode2;
        unsigned int  mapcode3;
}ONEMAPTHREE;


typedef  struct tagOneMapFour{
        unsigned int  origincode;
        unsigned int  mapcode1;
        unsigned int  mapcode2;
        unsigned int  mapcode3;
        unsigned int  mapcode4;
}ONEMAPFOUR;

/** 
 * Option to allow processing of unassigned code points in the input
 */
#define USPREP_ALLOW_UNASSIGNED 0x0001

/** 
 * Option to allow processing of unassigned codepoints in the input
 */
#define UIDNA_ALLOW_UNASSIGNED 0x0001

/** 
 * Option to check if input conforms to STD-3 ASCII rules
 */
#define UIDNA_USE_STD3_RULES   0x0002

//FIXME: no need? It defines in UErrorCode.
//#define U_IDNA_ZERO_LENGTH_LABEL_ERROR U_IDNA_ERROR_LIMIT


#ifdef __cplusplus
extern "C" {
#endif

int uidna_IDNToASCII(const UChar *src, int srcLength,
                   UChar* dest, int destCapacity,
                   int options,
                   UErrorCode *status);

#ifdef  __cplusplus
}
#endif
#endif   /* end of UidnaMg_h */


