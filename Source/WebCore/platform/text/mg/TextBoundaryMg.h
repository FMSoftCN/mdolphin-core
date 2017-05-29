/*
** $Id: TextBoundaryMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** TextBoundaryMg.h: head file for struct array type
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-4-6
*/

#ifndef  TextBoundaryMg_h
#define  TextBoundaryMg_h

#include  "unicode/UcharMg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagCodeAreaMg{
    unsigned int   startcode;
    unsigned int   endcode;
}CODEAREAMG;


typedef  enum grapheme_cluster_type{
        gc_ERROR_TYPE=-1,
        CR=1,
        LF,
        CONTROL,
        EXTEND,
        L,
        V,
        T,
        LV,
        LVT,
        gc_TYPE_ANY
}GC_TYPE;


typedef enum word_type{
        w_ERROR_TYPE=-1,
        FORMAT=1,
        KATAKANA,
        ALETTER,
        MIDLETTER,
        MIDNUM,
        NUMERIC,
        EXTENDNUMLET,
        w_TYPE_ANY
}WORD_TYPE;


typedef struct tagGraphemeClusterinfo{
        int current;
        GC_TYPE  cur_type;
}GRAPHEMECLUSTERINFO;

typedef  struct tagWordBoundaryInfo{
        int current;
        WORD_TYPE   cur_type;
}WORDBOUNDARY;


int  firstwordBoundary(const UChar* text, int textlength, WORDBOUNDARY* pwordinfo);
int  nextwordBoundary(const UChar* text, int textlength, WORDBOUNDARY* pwordinfo);

int  firstGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo);
int  nextGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo);
int  previousGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo);
int  followingGraphemeClusterBoundary(const UChar* text, int textlength, int offset,GRAPHEMECLUSTERINFO* pGraphemeClusterinfo);
int  precedingGraphemeClusterBoundary(const UChar* text, int textlength, int offset,GRAPHEMECLUSTERINFO* pGraphemeClusterinfo);

#ifdef __cplusplus
}
#endif

#endif
