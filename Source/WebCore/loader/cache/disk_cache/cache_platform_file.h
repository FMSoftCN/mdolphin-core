// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PLATFORM_FILE_H_
#define BASE_PLATFORM_FILE_H_



#include "config.h"

#include "CString.h"
#include "PlatformString.h"
#include "FileSystemMg.h"

#include "cache_platform_file.h"
#include "file_path.h"

class FilePath;

namespace base {


enum PlatformFileFlags {
    //  PLATFORM_FILE_OPEN = 1,
    PLATFORM_FILE_CREATE = 2, //if does not exist,then create it
    PLATFORM_FILE_READ = 16,
    PLATFORM_FILE_WRITE = 32,
};

//open or create a file, if flags is PLATFORM_FILE_CREATE, this function try 
//to open the file, if failed, it will create the file,and *create will be true
WebCore::HFile CreatePlatformFile(const FilePath& name, int flags,
        bool* created);

}  // namespace base

#endif  // BASE_PLATFORM_FILE_H_
