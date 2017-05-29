// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Defines the public interface of the disk cache. For more details see
// http://dev.chromium.org/developers/design-documents/disk-cache

#ifndef NET_DISK_CACHE_DISK_CACHE_H_
#define NET_DISK_CACHE_DISK_CACHE_H_

#include <string>
#include <vector>

#include "basictypes.h"
#include "cache_time.h"
#include "cache_type.h"

#include "PlatformString.h"

namespace net {
class IOBuffer;
typedef void(*CompletionCallback)(void*);
}

namespace disk_cache {

class Entry;
class Backend;
//by suyong
Backend* CreateCacheBackend(const char* path, bool force,
        int max_bytes, net::CacheType type);

// Returns an instance of the Backend. path points to a folder where
// the cached data will be stored. This cache instance must be the only object
// that will be reading or writing files to that folder. The returned object
// should be deleted when not needed anymore. If force is true, and there is
// a problem with the cache initialization, the files will be deleted and a
// new set will be created. max_bytes is the maximum size the cache can grow to.
// If zero is passed in as max_bytes, the cache will determine the value to use
// based on the available disk space. The returned pointer can be NULL if a
// fatal error is found.
//Backend* CreateCacheBackend(const FilePath& path, bool force,
//                           int max_bytes, net::CacheType type);

// The root interface for a disk cache instance.
class Backend {
    public:
        virtual ~Backend() {}

        // Opens an existing entry.  Upon success, the out param holds a pointer
        // to a Entry object representing the specified disk cache entry.
        // When the entry pointer is no longer needed, the Close method
        // should be called.
        virtual bool OpenEntry(const WTF::String& key, Entry** entry) = 0;

        // Creates a new entry.  Upon success, the out param holds a pointer
        // to a Entry object representing the newly created disk cache
        // entry.  When the entry pointer is no longer needed, the Close
        // method should be called.
        virtual bool CreateEntry(const WTF::String& key, Entry** entry) = 0;

        // Marks the entry, specified by the given key, for deletion.
        virtual bool DoomEntry(const WTF::String& key) = 0;

        // Marks all entries for deletion.
        virtual bool DoomAllEntries() = 0;

        // set disk cache size
        virtual bool SetMaxSize(int) = 0;

        // get disk cache size
        virtual bool GetMaxSize(int* size) = 0;

        virtual unsigned int GetUsedSize() = 0;


};

// This interface represents an entry in the disk cache.
class Entry {
    public:
        // Marks this cache entry for deletion.
        virtual void Doom() = 0;

        // Releases this entry. Calling this method does not cancel pending IO
        // operations on this entry. Even after the last reference to this object has
        // been released, pending completion callbacks may be invoked.
        virtual void Close() = 0;

        // Returns the key associated with this cache entry.
        virtual WTF::String GetKey() const = 0;

        // Returns the size of the cache data with the given index.
        virtual unsigned GetDataSize(int index) const = 0;

        // Copies cache data into the given buffer of length |buf_len|.  If
        // completion_callback is null, then this call blocks until the read
        // operation is complete.  Otherwise, completion_callback will be
        // called on the current thread once the read completes.  Returns the
        // number of bytes read or a network error code. If a completion callback is
        // provided then it will be called if this function returns ERR_IO_PENDING,
        // and a reference to |buf| will be retained until the callback is called.
        // Note that the callback will be invoked in any case, even after Close has
        // been called; in other words, the caller may close this entry without
        // having to wait for all the callbacks, and still rely on the cleanup
        // performed from the callback code.
        virtual int ReadData(int index, int offset, char* buf, int buf_len,
                net::CompletionCallback* completion_callback) = 0;

        // Copies cache data from the given buffer of length |buf_len|.  If
        // completion_callback is null, then this call blocks until the write
        // operation is complete.  Otherwise, completion_callback will be
        // called on the current thread once the write completes.  Returns the
        // number of bytes written or a network error code. If a completion callback
        // is provided then it will be called if this function returns ERR_IO_PENDING,
        // and a reference to |buf| will be retained until the callback is called.
        // Note that the callback will be invoked in any case, even after Close has
        // been called; in other words, the caller may close this entry without
        // having to wait for all the callbacks, and still rely on the cleanup
        // performed from the callback code.
        // If truncate is true, this call will truncate the stored data at the end of
        // what we are writing here.
        // virtual int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
        virtual int WriteData(int index, int offset, const char* buf, int buf_len,
                net::CompletionCallback* completion_callback,
                bool truncate) = 0;


    protected:
        virtual ~Entry() {}
};

}  // namespace disk_cache

#endif  // NET_DISK_CACHE_DISK_CACHE_H_
