// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_REF_COUNTED_H_
#define BASE_REF_COUNTED_H_

//#include "base/atomic_ref_count.h"
//#include "base/thread_collision_warner.h"
//#include "atomic_ref_count.h"
//#include "thread_collision_warner.h"
#include "basictypes.h"
namespace base {

namespace subtle {

class RefCountedBase {
    public:
        static bool ImplementsThreadSafeReferenceCounting() { return false; }

        bool HasOneRef() const { return ref_count_ == 1; }

    protected:
        RefCountedBase();
        ~RefCountedBase();

        void AddRef();

        // Returns true if the object should self-delete.
        bool Release();

    private:
        int ref_count_;
#ifndef NDEBUG
        bool in_dtor_;
#endif

};

class RefCountedThreadSafeBase {
    public:
        static bool ImplementsThreadSafeReferenceCounting() { return true; }

        bool HasOneRef() const;

    protected:
        RefCountedThreadSafeBase();
        ~RefCountedThreadSafeBase();

        void AddRef();

        // Returns true if the object should self-delete.
        bool Release();

    private:
        int ref_count_;
        //AtomicRefCount ref_count_;
#ifndef NDEBUG
        bool in_dtor_;
#endif

        DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);
};

}  // namespace subtle

//
// A base class for reference counted classes.  Otherwise, known as a cheap
// knock-off of WebKit's RefCounted<T> class.  To use this guy just extend your
// class from it like so:
//
//   class MyFoo : public base::RefCounted<MyFoo> {
//    ...
//   };
//
template <class T>
class RefCounted : public subtle::RefCountedBase {
    public:
        RefCounted() { }
        ~RefCounted() { }

        void AddRef()
        {
            subtle::RefCountedBase::AddRef();
        }

        void Release()
        {
            if (subtle::RefCountedBase::Release()) {
                delete static_cast<T*>(this);
            }
        }

    private:
        DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
};

// Forward declaration.
template <class T, typename Traits> class RefCountedThreadSafe;

// Default traits for RefCountedThreadSafe<T>.  Deletes the object when its ref
// count reaches 0.  Overload to delete it on a different thread etc.
template<typename T>
struct DefaultRefCountedThreadSafeTraits {
    static void Destruct(T* x) {
        // Delete through RefCountedThreadSafe to make child classes only need to be
        // friend with RefCountedThreadSafe instead of this struct, which is an
        // implementation detail.
        RefCountedThreadSafe<T, DefaultRefCountedThreadSafeTraits>::DeleteInternal(x);
    }
};

//
// A thread-safe variant of RefCounted<T>
//
//   class MyFoo : public base::RefCountedThreadSafe<MyFoo> {
//    ...
//   };
//
// If you're using the default trait, then you may choose to add compile time
// asserts that no one else is deleting your object.  i.e.
//    private:
//     friend struct base::RefCountedThreadSafe<MyFoo>;
//     ~MyFoo();
template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase {
    public:
        RefCountedThreadSafe() { }
        ~RefCountedThreadSafe() { }

        void AddRef()
        {
            subtle::RefCountedThreadSafeBase::AddRef();
        }

        void Release()
        {
            if (subtle::RefCountedThreadSafeBase::Release()) {
                Traits::Destruct(static_cast<T*>(this));
            }
        }

    private:
        friend struct DefaultRefCountedThreadSafeTraits<T>;
        static void DeleteInternal(T* x) { delete x; }

        DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe);
};

//
// A wrapper for some piece of data so we can place other things in
// scoped_refptrs<>.
//
template<typename T>
class RefCountedData : public base::RefCounted< base::RefCountedData<T> > {
    public:
        RefCountedData() : data() {}
        RefCountedData(const T& in_value) : data(in_value) {}

        T data;
};

}  // namespace base

template <class T>
class scoped_refptr {
    public:
        scoped_refptr() : ptr_(NULL)
        {
        }

        scoped_refptr(T* p) : ptr_(p)
        {
            if (ptr_)
                ptr_->AddRef();
        }

        scoped_refptr(const scoped_refptr<T>& r) : ptr_(r.ptr_)
        {
            if (ptr_)
                ptr_->AddRef();
        }

        ~scoped_refptr()
        {
            if (ptr_)
                ptr_->Release();
        }

        T* get() const { return ptr_; }
        operator T*() const { return ptr_; }
        T* operator->() const { return ptr_; }

        // Release a pointer.
        // The return value is the current pointer held by this object.
        // If this object holds a NULL pointer, the return value is NULL.
        // After this operation, this object will hold a NULL pointer,
        // and will not own the object any more.
        T* release()
        {
            T* retVal = ptr_;
            ptr_ = NULL;
            return retVal;
        }

        scoped_refptr<T>& operator=(T* p)
        {
            // AddRef first so that self assignment should work
            if (p)
                p->AddRef();
            if (ptr_ )
                ptr_ ->Release();
            ptr_ = p;
            return *this;
        }

        scoped_refptr<T>& operator=(const scoped_refptr<T>& r)
        {
            return *this = r.ptr_;
        }

        void swap(T** pp)
        {
            T* p = ptr_;
            ptr_ = *pp;
            *pp = p;
        }

        void swap(scoped_refptr<T>& r)
        {
            swap(&r.ptr_);
        }

    protected:
        T* ptr_;
};

#endif  // BASE_REF_COUNTED_H_
