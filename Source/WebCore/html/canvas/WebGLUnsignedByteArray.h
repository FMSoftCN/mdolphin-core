/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef WebGLUnsignedByteArray_h
#define WebGLUnsignedByteArray_h

#include "WebGLIntegralTypedArrayBase.h"

namespace WebCore {

class WebGLArrayBuffer;

class WebGLUnsignedByteArray : public WebGLIntegralTypedArrayBase<unsigned char> {
  public:
    static PassRefPtr<WebGLUnsignedByteArray> create(unsigned length);
    static PassRefPtr<WebGLUnsignedByteArray> create(unsigned char* array, unsigned length);
    static PassRefPtr<WebGLUnsignedByteArray> create(PassRefPtr<WebGLArrayBuffer> buffer, unsigned byteOffset, unsigned length);

    using WebGLTypedArrayBase<unsigned char>::set;
    using WebGLIntegralTypedArrayBase<unsigned char>::set;

  private:
    WebGLUnsignedByteArray(PassRefPtr<WebGLArrayBuffer> buffer,
                           unsigned byteOffset,
                           unsigned length);
    // Make constructor visible to superclass.
    friend class WebGLTypedArrayBase<unsigned char>;

    // Overridden from WebGLArray.
    virtual bool isUnsignedByteArray() const { return true; }
    virtual PassRefPtr<WebGLArray> slice(int start, int end) const;
};

} // namespace WebCore

#endif // WebGLUnsignedByteArray_h