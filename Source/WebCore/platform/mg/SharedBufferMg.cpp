/*
 ** $Id: SharedBufferMg.cpp 236 2010-07-28 01:14:50Z lijiangwei $
 **
 ** SharedBufferMg.cpp: The platform implementation for SharedBuffer.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-07 
 */

#include "config.h"
#include "SharedBuffer.h"

#include <wtf/text/CString.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace WebCore {

PassRefPtr<SharedBuffer> SharedBuffer::createWithContentsOfFile(const String& filePath)
{
    FILE* file;
    struct stat fileStat;
    RefPtr<SharedBuffer> result;

    if (filePath.isEmpty())
        return 0;
    
    if (!(file = fopen(filePath.utf8().data(), "rb")))
        return 0;

    if (fstat(fileno(file), &fileStat)) {
        fclose(file);
        return 0;
    }

    int size = fileStat.st_size;
    char* ptr = (char*)malloc(size);
    if (!ptr)
        return 0;
    fread(ptr, 1, size, file);
    fclose(file);
    result = SharedBuffer::create(ptr, size); 
    free(ptr);
    return result.release();
}

} // namespace WebCore

