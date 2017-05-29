// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file defines utility functions for working with strings.

#ifndef BASE_STRING_UTIL_H_
#define BASE_STRING_UTIL_H_

#include "PlatformString.h"

using WTF::String;


bool StartsWithASCII(const String& str,
                     const String& search,
                     bool case_sensitive);

size_t strlcpy(char* dst, const char* src, size_t dst_size);
#endif  // BASE_STRING_UTIL_H_
