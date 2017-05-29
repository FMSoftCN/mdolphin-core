// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "cache_file.h"

//#include <fcntl.h>
#include <wtf/Platform.h>
#include "FileSystemMg.h"
#include "file_path.h"

#include "disk_cache.h"

namespace disk_cache {
File::File(WebCore::HFile file)
{
    init_ = true;
    mixed_ = true;
    platform_file_ = file; 

}

bool File::Init(const FilePath& name)
{
    if (init_)
        return false;   
		
    int flags = base::PLATFORM_FILE_READ | base::PLATFORM_FILE_WRITE;

    platform_file_ = base::CreatePlatformFile(name, flags, NULL);
    if (platform_file_ == NULL) {
        return false;
    }

    init_ = true;
    return true;  
}

bool File::Init(const WebCore::HFile plat_file)
{
    if (init_)
        return false;

    platform_file_ = plat_file;
    if (platform_file_ == NULL) {
        return false;
    }

    init_ = true;
    return true;  
}

File::~File()
{
    if (platform_file_)
        WebCore::closeFile(platform_file_);
}

WebCore::HFile File::platform_file() const
{
    return platform_file_;
}

bool File::IsValid() const
{
    if (!init_)
        return false;
    return true;//FIXME: suyong
    //  return (base::kInvalidPlatformFileValue != platform_file_);
}

bool File::Read(void* buffer, size_t buffer_len, size_t offset)
{
    //  DCHECK(init_);
    if (buffer_len > ULONG_MAX || offset > LONG_MAX)
        return false;

    //  int ret = pread(platform_file_, buffer, buffer_len, offset);
    if (WebCore::seekFile(platform_file_, offset, SEEK_SET))
        return false;

    int ret = WebCore::readFile(buffer, 1, buffer_len,  platform_file_);
    return (static_cast<size_t>(ret) == buffer_len);
}

bool File::Write(const void* buffer, size_t buffer_len, size_t offset)
{
    //  DCHECK(init_);
    if (buffer_len > ULONG_MAX || offset > ULONG_MAX)
        return false;

    if (WebCore::seekFile(platform_file_, offset, SEEK_SET))
        return false;  

    int ret = WebCore::writeFile((void*)buffer, 1, buffer_len, platform_file_);

    //  fflush((FILE*)platform_file_);//FIXME: suyong
    WebCore::fflushFile(platform_file_);//FIXME: suyong

    return (static_cast<size_t>(ret) == buffer_len);
}

bool File::SetLength(size_t length)
{
    //  DCHECK(init_);
    if (length > ULONG_MAX)
        return false;
    //TODO: SET THE FILE LENGTH suyong
    int fd;
    //  fd = fileno((FILE*)platform_file_);//FIXME: suyong
    fd = WebCore::filenoFile(platform_file_);//FIXME: suyong  

    long ret = 0;
    if (!WebCore::seekFile(platform_file_, 0, 2))
        //     ret = ftell((FILE *)platform_file_); //FIXME: suyong
        ret = WebCore::ftellFile(platform_file_);  

    //  ret = ftruncate(fd, length);
    ret = WebCore::ftruncateFile(fd, length);  //FIXME: suyong
    return 0 == ret; //FIXME:suyong

}

size_t File::GetLength()
{
    //  DCHECK(init_);
    //  size_t ret = WebCore::seekFile(platform_file_, 0, SEEK_END);//FIXME: suyong
    size_t ret = 0;
    if (!WebCore::seekFile(platform_file_, 0, 2))
        //     ret = ftell((FILE *)platform_file_);
        ret = WebCore::ftellFile(platform_file_);  //FIXME: suyong
    return ret;
}

}  // namespace disk_cache
