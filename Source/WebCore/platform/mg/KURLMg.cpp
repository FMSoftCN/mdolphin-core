/*
 ** $Id: KURLMg.cpp 51 2010-06-04 05:52:37Z xwyan $
 **
 ** KURLMg.cpp: The platform implementation for KURL.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "config.h"
#include "KURL.h"

#include "NotImplemented.h"

namespace WebCore {

String KURL::fileSystemPath() const
{
    if (!isValid() || !protocolIs("file"))
        return String();

    return String(path());
}

}

