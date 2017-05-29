/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef IndexedDatabaseRequest_h
#define IndexedDatabaseRequest_h

#include "ExceptionCode.h"
#include "IndexedDatabase.h"
#include "PlatformString.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

#if ENABLE(INDEXED_DATABASE)

namespace WebCore {

class IndexedDatabase;
class Frame;

class IndexedDatabaseRequest : public RefCounted<IndexedDatabaseRequest> {
public:
    static PassRefPtr<IndexedDatabaseRequest> create(IndexedDatabase* indexedDatabase, Frame* frame)
    {
        return adoptRef(new IndexedDatabaseRequest(indexedDatabase, frame));
    }
    ~IndexedDatabaseRequest();

    PassRefPtr<IDBRequest> open(const String& name, const String& description, bool modifyDatabase, ExceptionCode&);

    void disconnectFrame() { m_frame = 0; }

private:
    IndexedDatabaseRequest(IndexedDatabase*, Frame*);

    PassRefPtr<IndexedDatabase> m_indexedDatabase;
    Frame* m_frame;
};

} // namespace WebCore

#endif

#endif // IndexedDatabaseRequest_h

