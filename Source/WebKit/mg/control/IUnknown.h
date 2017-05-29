/*
 ** $Id: IUnknown.h 232 2010-07-26 08:51:55Z xwyan $
 **
 ** IUnknown.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-08 
 */

#ifndef IUnknown_H
#define IUnknown_H

#ifdef __cplusplus

class IUnknown 
{
    int ref;
public:
    IUnknown():ref(1) { }
    virtual ~IUnknown() { }

    int addRef() { return ++ref; }
    int release() { if (--ref == 0) { delete this; return 0; } return ref; }

    IUnknown * adoptRef() { --ref; return this; }
};

#else
//FIXEDME 
typedef struct _IUnknown IUnknown; 

#endif

#ifdef __cplusplus
extern "C" {
#endif

int mdAddRef(IUnknown*);

int mdRelease(IUnknown*);

IUnknown* mdAdoptRef(IUnknown*);


#ifdef __cplusplus
}
#endif

#endif

