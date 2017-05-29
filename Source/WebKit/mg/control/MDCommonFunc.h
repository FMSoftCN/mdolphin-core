/*
 ** $Id$
 **
 ** MDCommonFunc.h: some global function
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-15 
 */

#ifndef MDCommonFunc_h
#define MDCommonFunc_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "config.h"
#include "WTFString.h"
#include "CString.h"

using namespace WebCore;

static inline char* copyStringToUtf8Char(const WTF::String &str)
{
    if (str.isEmpty())
        return NULL;

    CString cstr = str.utf8();
    int len = cstr.length();
    char* p =  (char *)malloc(len+1);
    if (p) {
        memcpy(p, cstr.data(), len); // using strcpy?
        p[len] = '\0';
    }

    return p;
}

static inline void freeUtf8Char(void *str)
{
    if (str)
        free(str);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
