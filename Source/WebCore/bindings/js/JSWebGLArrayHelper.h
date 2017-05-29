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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
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

#ifndef JSWebGLArrayHelper_h
#define JSWebGLArrayHelper_h

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include <interpreter/CallFrame.h>
#include <runtime/ArgList.h>
#include <runtime/Error.h>
#include <runtime/JSObject.h>
#include <runtime/JSValue.h>

namespace WebCore {

template <class T>
JSC::JSValue setWebGLArrayHelper(JSC::ExecState* exec, T* impl, JSC::ArgList const& args, T* (*conversionFunc)(JSC::JSValue))
{
    if (args.size() < 1)
        return throwError(exec, JSC::SyntaxError);

    T* array = (*conversionFunc)(args.at(0));
    if (array) {
        // void set(in WebGL<T>Array array, [Optional] in unsigned long offset);
        unsigned offset = 0;
        if (args.size() == 2)
            offset = args.at(1).toInt32(exec);
        ExceptionCode ec = 0;
        impl->set(array, offset, ec);
        setDOMException(exec, ec);
        return JSC::jsUndefined();
    }

    if (args.at(0).isObject()) {
        // void set(in sequence<long> array, [Optional] in unsigned long offset);
        JSC::JSObject* array = JSC::asObject(args.at(0));
        uint32_t offset = 0;
        if (args.size() == 2)
            offset = args.at(1).toInt32(exec);
        uint32_t length = array->get(exec, JSC::Identifier(exec, "length")).toInt32(exec);
        if (offset > impl->length()
            || offset + length > impl->length()
            || offset + length < offset)
            setDOMException(exec, INDEX_SIZE_ERR);
        else {
            for (uint32_t i = 0; i < length; i++) {
                JSC::JSValue v = array->get(exec, i);
                if (exec->hadException())
                    return JSC::jsUndefined();
                impl->set(i + offset, v.toNumber(exec));
            }
        }

        return JSC::jsUndefined();
    }

    return JSC::throwError(exec, JSC::SyntaxError);
}

}

#endif // JSWebGLArrayHelper_h
