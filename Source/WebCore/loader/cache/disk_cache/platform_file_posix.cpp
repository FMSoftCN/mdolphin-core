// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdio.h"
#include "config.h"

#include "CString.h"
#include "PlatformString.h"
#include "FileSystemMg.h"

#include "cache_platform_file.h"
#include "file_path.h"

namespace base {

// TODO(erikkay): does it make sense to support PLATFORM_FILE_EXCLUSIVE_* here?
WebCore::HFile CreatePlatformFile (const FilePath& name, int flags,
        bool* created)
{
    char open_flags [4];
    WebCore::HFile descriptor = NULL;

    bool file_exit = WebCore::fileExists(name.value());

    if((flags & PLATFORM_FILE_READ) && !(flags & PLATFORM_FILE_WRITE) 
            && !(flags & PLATFORM_FILE_CREATE)) {
        if (!file_exit)
            return NULL;

        open_flags[0] = 'r';
        open_flags[1] = '\0';
        descriptor = WebCore::openFile(name.value().utf8().data(), open_flags);
        return descriptor;
    }

    if ( (flags & PLATFORM_FILE_WRITE) && !(flags & PLATFORM_FILE_CREATE)) {
        open_flags[0] = 'r';
        open_flags[1] = '+';
        open_flags[2] = '\0';       
        descriptor = WebCore::openFile(name.value().utf8().data(), open_flags);    
        return descriptor;         
    }

    if (flags & PLATFORM_FILE_CREATE) {
        open_flags[0] = 'r';
        open_flags[1] = '+';
        open_flags[2] = '\0';       
        descriptor = WebCore::openFile(name.value().utf8().data(), open_flags);    
        if(descriptor != NULL){
            return descriptor;
        }

        open_flags[0] = 'w';
        open_flags[1] = '+';
        open_flags[2] = '\0';    
        descriptor = WebCore::openFile(name.value().utf8().data(), open_flags);    
        if (descriptor == NULL) {
            if (created)
                *created = false;
        } else {
            if (created)
                *created = true;
        }        
    }  

    return descriptor;    
}

}  // namespace base

