// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_HASH_H__
#define NET_DISK_CACHE_HASH_H__

//#include <string>

//#include "base/basictypes.h"
#include "PlatformString.h"
#include "CString.h"
#include "basictypes.h"

namespace disk_cache {

// From http://www.azillionmonkeys.com/qed/hash.html
// This is the hash used on WebCore/platform/stringhash
uint32 SuperFastHash(const char * data, int len);
/*
   inline uint32 Hash(const char* key, size_t length) {
   return SuperFastHash(key, static_cast<int>(length));
   }
   */
//inline uint32 Hash(const std::string& key) {
inline uint32 Hash(const WTF::String& key) {	
    //  if (key.empty())m_data
    if (key.isEmpty())
        return 0;
    //  return SuperFastHash(key.data(), static_cast<int>(key.size()));
    return SuperFastHash(key.utf8().data(), key.utf8().length());  
}

}  // namespace disk_cache

#endif  // NET_DISK_CACHE_HASH_H__
