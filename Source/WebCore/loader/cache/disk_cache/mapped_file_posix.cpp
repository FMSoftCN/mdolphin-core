// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "net/disk_cache/mapped_file.h"

//#include <errno.h>
//#include <sys/mman.h>
//#include <stdio.h>

//#include "base/file_path.h"
//#include "base/logging.h"
//#include "net/disk_cache/disk_cache.h"

#include "mdolphin_debug.h"
#include "file_path.h"
#include "disk_cache.h"
#include "mdolphin_debug.h"

#include "cache_mapped_file.h"

namespace disk_cache {

void* MappedFile::Init(const FilePath& name, size_t size)
{
    //  DCHECK(!init_);
    if (init_ || !File::Init(name))
        return NULL;

    if (!size)
        size = GetLength();
    //TODO: suyong
    //FIXME: suyong
    // md_debug ("hello %p\n", (FILE*)platform_file());
    buffer_ = WebCore::mmapFile(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
            platform_file(), 0);
    // md_debug ("buffer %p\n", buffer_);

    init_ = true;
    //  DCHECK(reinterpret_cast<intptr_t>(buffer_) != -1);
    if (reinterpret_cast<intptr_t>(buffer_) == -1)
        buffer_ = 0;

    view_size_ = size;
    return buffer_;
}

//void* MappedFile::Init(const FilePath& name, size_t size) {	
void* MappedFile::Init(const WebCore::HFile plat_file, size_t size)
{
    //  DCHECK(!init_);
    if (init_ || !File::Init(plat_file))
        return NULL;

    if (!size)
        size = GetLength();
    //TODO: suyong
    //FIXME: suyong
    //  int fd;
    //  fd = fileno((FILE*)platform_file());
    buffer_ = WebCore::mmapFile(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
            platform_file(), 0);
    init_ = true;
    //  DCHECK(reinterpret_cast<intptr_t>(buffer_) != -1);
    if (reinterpret_cast<intptr_t>(buffer_) == -1)
        buffer_ = 0;

    view_size_ = size;
    return buffer_;
}

MappedFile::~MappedFile()
{
    if (!init_)
        return;
    //TODO: suyong
    //FIXME: suyong
    if (buffer_) {
        WebCore::munmapFile(buffer_, view_size_);
        // md_debug("****%s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
        //    DCHECK(0 == ret);
    }
}

bool MappedFile::Load(const FileBlock* block)
{
    size_t offset = block->offset() + view_size_;
    return Read(block->buffer(), block->size(), offset);
}

bool MappedFile::Store(const FileBlock* block)
{
    size_t offset = block->offset() + view_size_;
    return Write(block->buffer(), block->size(), offset);
}

}  // namespace disk_cache
