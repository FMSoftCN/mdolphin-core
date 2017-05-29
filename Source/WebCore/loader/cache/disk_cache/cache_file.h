// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// See net/disk_cache/disk_cache.h for the public interface of the cache.

#ifndef NET_DISK_CACHE_FILE_H_
#define NET_DISK_CACHE_FILE_H_


//#include <string>  //suyong
#include "PlatformString.h"
#include "FileSystemMg.h"
#include "CString.h"
#include "cache_platform_file.h"
#include "cache_ref_counted.h"

/*
#include "base/platform_file.h"
#include "base/ref_counted.h"
*/
class FilePath;

namespace disk_cache {

// Simple wrapper around a file that allows asynchronous operations.
class File : public base::RefCounted<File> {
    friend class base::RefCounted<File>;
    public:
    File() : init_(false), mixed_(false) {}
    // mixed_mode set to true enables regular synchronous operations for the file.
    explicit File(bool mixed_mode) : init_(false), mixed_(mixed_mode) {}

    explicit File(WebCore::HFile file);  

    // Initializes the object to point to a given file. The file must aready exist
    // on disk, and allow shared read and write.
    bool Init(const FilePath& name);

    bool Init(const WebCore::HFile plat_file) ; //open the file, implemented with platform

    // Returns the handle or file descriptor.
    WebCore::HFile platform_file() const;

    // Returns true if the file was opened properly.
    bool IsValid() const;

    // Performs synchronous IO.
    bool Read(void* buffer, size_t buffer_len, size_t offset);//read from the file, offset is the 
                                                              //offset form the begining of file 
    bool Write(const void* buffer, size_t buffer_len, size_t offset);

    // Sets the file's length. The file is truncated or extended with zeros to
    // the new length.
    bool SetLength(size_t length);
    size_t GetLength();

    protected:
    virtual ~File();

    private:
    bool init_;
    bool mixed_;
    WebCore::HFile platform_file_; //by suyong

};

}  // namespace disk_cache

#endif  // NET_DISK_CACHE_FILE_H_
