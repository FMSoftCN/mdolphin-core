/*
 ** $Id: IUnknown.cpp 129 2010-07-09 01:11:56Z xwyan $
 **
 ** IUnknown.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-08 
 */

#include "IUnknown.h"

int mdAddRef(IUnknown* punkown)
{
    if(punkown)
        return punkown->addRef();
    return 0;
}

int mdRelease(IUnknown* punkown)
{
    if(punkown)
        return punkown->release();
    return 0;
}

IUnknown* mdAdoptRef(IUnknown* punkown)
{
    if(punkown)
        return punkown->adoptRef();
    return (IUnknown*)0;
}


