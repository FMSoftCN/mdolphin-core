/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Copyright (c) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WebGLIntegralTypedArrayBase_h
#define WebGLIntegralTypedArrayBase_h

#include "WebGLTypedArrayBase.h"
#include <limits>
#include <wtf/MathExtras.h>

// Base class for all WebGL<T>Array types holding integral
// (non-floating-point) values.

namespace WebCore {

template <typename T>
class WebGLIntegralTypedArrayBase : public WebGLTypedArrayBase<T> {
  public:
    void set(unsigned index, double value)
    {
        if (index >= WebGLTypedArrayBase<T>::m_length)
            return;
        if (isnan(value)) // Clamp NaN to 0
            value = 0;
        if (value < std::numeric_limits<T>::min())
            value = std::numeric_limits<T>::min();
        else if (value > std::numeric_limits<T>::max())
            value = std::numeric_limits<T>::max();
        WebGLTypedArrayBase<T>::data()[index] = static_cast<T>(value);
    }

    // Invoked by the indexed getter. Does not perform range checks; caller
    // is responsible for doing so and returning undefined as necessary.
    T item(unsigned index) const
    {
        ASSERT(index < WebGLTypedArrayBase<T>::m_length);
        return WebGLTypedArrayBase<T>::data()[index];
    }

  protected:
    WebGLIntegralTypedArrayBase(PassRefPtr<WebGLArrayBuffer> buffer, unsigned byteOffset, unsigned length)
        : WebGLTypedArrayBase<T>(buffer, byteOffset, length)
    {
    }
};

} // namespace WebCore

#endif // WebGLIntegralTypedArrayBase_h
