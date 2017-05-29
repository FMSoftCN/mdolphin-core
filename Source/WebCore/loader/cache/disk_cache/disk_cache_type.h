#ifndef _DISK_CACHE_TYPE_
#define _DISK_CACHE_TYPE_

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

#ifndef offsetof
#define offsetof(type, member) ( (int) & ((type*)0) -> member )
#endif

// The COMPILE_ASSERT macro can be used to verify that a compile time
// // expression is true. For example, you could use it to verify the
// // size of a static array:
// //
// //   COMPILE_ASSERT(sizeof(num_content_type_names) == sizeof(int),
// //                  content_type_names_incorrect_size);
// //
// // or to make sure a struct is smaller than a certain size:
// //
// //   COMPILE_ASSERT(sizeof(foo) < 128, foo_too_large);
// //
// // The second argument to the macro is the name of the variable. If
// // the expression is false, most compilers will issue a warning/error
// // containing the name of the variable.
// //
// // Implementation details of COMPILE_ASSERT:
// //
// // - COMPILE_ASSERT works by defining an array type that has -1
// //   elements (and thus is invalid) when the expression is false.
// //
// // - The simpler definition
// //
// //     #define COMPILE_ASSERT(expr, msg) typedef char msg[(expr) ? 1 : -1]
// //
// //   does not work, as gcc supports variable-length arrays whose sizes
// //   are determined at run-time (this is gcc's extension and not part
// //   of the C++ standard).  As a result, gcc fails to reject the
// //   following code with the simple definition:
// //
// //     int foo;
// //     COMPILE_ASSERT(foo, msg); // not supposed to compile as foo is
// //                               // not a compile-time constant.
// //
// // - By using the type CompileAssert<(bool(expr))>, we ensures that
// //   expr is a compile-time constant.  (Template arguments must be
// //   determined at compile-time.)
// //
// // - The outter parentheses in CompileAssert<(bool(expr))> are necessary
// //   to work around a bug in gcc 3.4.4 and 4.0.1.  If we had written
// //
// //     CompileAssert<bool(expr)>
// //
// //   instead, these compilers will refuse to compile
// //
// //     COMPILE_ASSERT(5 > 0, some_message);
// //
// //   (They seem to think the ">" in "5 > 0" marks the end of the
// //   template argument list.)
// //
// // - The array size is (bool(expr) ? 1 : -1), instead of simply
// //
// //     ((expr) ? 1 : -1).
// //
// //   This is to avoid running into a bug in MS VC 7.1, which
// //   causes ((0.0) ? 1 : -1) to incorrectly evaluate to 1.
//
template <bool>
struct CompileAssert {
};

#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) \
    typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

#endif
