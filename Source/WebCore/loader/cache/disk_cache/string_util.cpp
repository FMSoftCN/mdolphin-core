// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "string_util.h"
#include "PlatformString.h"

using namespace WebCore;

bool startswithascii(const String& str,
                     const String& search,
                     bool case_sensitive)
{
    return str.startsWith(search, case_sensitive);

#if 0
    if (case_sensitive)
        return str.compare(0, search.length(), search) == 0;
    else
        return base::strncasecmp(str.c_str(), search.c_str(), search.length()) == 0;
#endif
}

template <typename CHAR>
size_t lcpyT(CHAR* dst, const CHAR* src, size_t dst_size)
{
    for (size_t i = 0; i < dst_size; ++i) {
        if ((dst[i] = src[i]) == 0)  // We hit and copied the terminating NULL.
            return i;
    }

    // We were left off at dst_size.  We over copied 1 byte.  Null terminate.
    if (dst_size != 0)
        dst[dst_size - 1] = 0;

    // Count the rest of the |src|, and return it's length in characters.
    while (src[dst_size]) ++dst_size;
    return dst_size;
}

size_t strlcpy(char* dst, const char* src, size_t dst_size)
{
    return lcpyT<char>(dst, src, dst_size);
}

