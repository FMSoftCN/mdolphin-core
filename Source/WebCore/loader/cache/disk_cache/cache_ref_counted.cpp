// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "base/ref_counted.h"

//#include "base/logging.h"
//#include "base/thread_collision_warner.h"

#include "cache_ref_counted.h"

//#include "logging.h"
//#include "thread_collision_warner.h"
namespace base {

namespace subtle {

RefCountedBase::RefCountedBase() : ref_count_(0)
{
#ifndef NDEBUG
    in_dtor_ = false;
#endif
}

RefCountedBase::~RefCountedBase()
{
#ifndef NDEBUG
    //  DCHECK(in_dtor_) << "RefCounted object deleted without calling Release()";
#endif
}

void RefCountedBase::AddRef()
{
    // TODO(maruel): Add back once it doesn't assert 500 times/sec.
    // Current thread books the critical section "AddRelease" without release it.
    // DFAKE_SCOPED_LOCK_THREAD_LOCKED(add_release_);
#ifndef NDEBUG
    //  DCHECK(!in_dtor_);
#endif
    ++ref_count_;
}

bool RefCountedBase::Release()
{
    // TODO(maruel): Add back once it doesn't assert 500 times/sec.
    // Current thread books the critical section "AddRelease" without release it.
    // DFAKE_SCOPED_LOCK_THREAD_LOCKED(add_release_);
#ifndef NDEBUG
    //  DCHECK(!in_dtor_);
#endif
    if (--ref_count_ == 0) {
#ifndef NDEBUG
        in_dtor_ = true;
#endif
        return true;
    }
    return false;
}

RefCountedThreadSafeBase::RefCountedThreadSafeBase() : ref_count_(0)
{
#ifndef NDEBUG
    in_dtor_ = false;
#endif
}

RefCountedThreadSafeBase::~RefCountedThreadSafeBase()
{
#ifndef NDEBUG
    //  DCHECK(in_dtor_) << "RefCountedThreadSafe object deleted without "
    //                      "calling Release()";
#endif
}

void RefCountedThreadSafeBase::AddRef()
{
#ifndef NDEBUG
    //  DCHECK(!in_dtor_);
#endif
    ++ref_count_;
    //AtomicRefCountInc(&ref_count_);
}

bool RefCountedThreadSafeBase::Release()
{
#ifndef NDEBUG
    //  DCHECK(!in_dtor_);
    //  DCHECK(!AtomicRefCountIsZero(&ref_count_));
#endif
    //if (!AtomicRefCountDec(&ref_count_)) {
    if (!(--ref_count_)) {
#ifndef NDEBUG
        in_dtor_ = true;
#endif
        return true;
    }
    return false;
}

bool RefCountedThreadSafeBase::HasOneRef() const
{
    return true;
    /*
       return AtomicRefCountIsOne(
       &const_cast<RefCountedThreadSafeBase*>(this)->ref_count_);
       */
}

}  // namespace subtle

}  // namespace base
