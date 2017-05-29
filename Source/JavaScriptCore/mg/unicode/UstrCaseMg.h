/*
** $Id: UstrCaseMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UstrCaseMg.h: header file for unicode string functions.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-6-2
*/

#ifndef UstrCaseMg_h
#define UstrCaseMg_h

#include  "unicode/UtypesMg.h"
#include  "unicode/UcharMg.h"

#ifdef __cplusplus
extern "C" {
#endif
int32_t u_strFoldCase(UChar *dest, int32_t destCapacity,
              const UChar *src, int32_t srcLength,
              uint32_t options,
              UErrorCode *pErrorCode);
int32_t u_strToLower(UChar *dest, int32_t destCapacity,
             const UChar *src, int32_t srcLength,
             const char *locale,
             UErrorCode *pErrorCode);
int32_t u_strToUpper(UChar *dest, int32_t destCapacity,
             const UChar *src, int32_t srcLength,
             const char *locale,
             UErrorCode *pErrorCode);
int32_t u_memcasecmp(const UChar *s1, const UChar *s2, 
        int32_t length, uint32_t options);
UChar * u_memset(UChar *dest, UChar c, int32_t count);

int32_t u_terminateUChars(UChar *dest, int32_t destCapacity, 
        int32_t length, UErrorCode *pErrorCode);
int32_t u_strlen(const UChar *s);

#ifdef  __cplusplus
}
#endif


#endif

