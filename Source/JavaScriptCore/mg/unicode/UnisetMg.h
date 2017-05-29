/*
** $Id: UnisetMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UnisetMg.h: header file for unicode hash.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-6-2
*/

#ifndef UnisetMg_h
#define UnisetMg_h

#include "unicode/UtypesMg.h"
#include "unicode/UcharMg.h"
#include "UtrieMg.h"

/**
 * A key or value within the hashtable.  It may be either a 32-bit
 * integral value or an opaque void* pointer.  The void* pointer may
 * be smaller than 32 bits (e.g. 24 bits) or may be larger (e.g. 64
 * bits).  The hashing and comparison functions take a pointer to a
 * UHashTok, but the deleter receives the void* pointer within it.
 *
 * Because a UHashTok is the size of a native pointer or a 32-bit
 * integer, we pass it around by value.
 */
union UHashTok {
    void*   pointer;
    int32_t integer;
};
typedef union UHashTok UHashTok;

/**
 * A token comparison function.
 * @param tok1 A token (object or integer)
 * @param tok2 A token (object or integer)
 * @return 0 if the two tokens are equal, -1 if tok1 is < tok2, or
 * +1 if tok1 is > tok2.
 */
typedef int8_t USortComparator(UHashTok tok1, UHashTok tok2);

/**
 * A token assignment function.  It may copy an integer, copy
 * a pointer, or clone a pointer, as appropriate.
 * @param dst The token to be assigned to
 * @param src The token to assign from
 */
typedef void UTokenAssigner(UHashTok *dst, UHashTok *src);

/**
  *  Pointer type for a user supplied atomic increment or decrement function.
  *  @param context user supplied value, obtained from from u_setAtomicIncDecFunctions().
  *  @param p   Pointer to a 32 bit int to be incremented or decremented
  *  @return    The value of the variable after the inc or dec operation.
  *  @stable ICU 2.8
  *  @system
  */
typedef int32_t UMtxAtomicFn(const void *context, int32_t *p);


enum UMatchDegree {
    U_MISMATCH,
    U_PARTIAL_MATCH,
    U_MATCH
};


/**
 * A key comparison function.
 * @param key1 A key stored in a hashtable
 * @param key2 A key stored in a hashtable
 * @return TRUE if the two keys are equal.
 */
typedef UBool UKeyComparator(const UHashTok key1, const UHashTok key2);

/**
 * A function called by <TT>uhash_remove</TT>,
 * <TT>uhash_close</TT>, or <TT>uhash_put</TT> to delete
 * an existing key or value.
 * @param obj A key or value stored in a hashtable
 */
typedef void UObjectDeleter(void* obj);

/**  U_OVERRIDE_CXX_ALLOCATION - Define this to override operator new and
 *                               delete in UMemory. Enabled by default for ICU.
 *
 *         Enabling forces all allocation of ICU object types to use ICU's
 *         memory allocation. On Windows, this allows the ICU DLL to be used by
 *         applications that statically link the C Runtime library, meaning that
 *         the app and ICU will be using different heaps.
 */
#ifndef U_OVERRIDE_CXX_ALLOCATION
#define U_OVERRIDE_CXX_ALLOCATION 1
#endif

/**
 * \def U_HAVE_PLACEMENT_NEW
 *  Define this to define the placement new and
 *                          delete in UMemory for STL.
 */
#ifndef U_HAVE_PLACEMENT_NEW
#define U_HAVE_PLACEMENT_NEW 1
#endif

/**
 * \def U_HAVE_DEBUG_LOCATION_NEW
 * Define this to define the MFC debug
 * version of the operator new.
 */
#ifndef U_HAVE_DEBUG_LOCATION_NEW
#define U_HAVE_DEBUG_LOCATION_NEW 0
#endif

class UMemory
{
public:

#if U_OVERRIDE_CXX_ALLOCATION
    static void * operator new(size_t size);
    static void * operator new[](size_t size);
    static void operator delete(void *p);
    static void operator delete[](void *p);

#if U_HAVE_PLACEMENT_NEW
    static inline void * operator new(size_t, void *ptr) {
        return ptr;
    }
    static inline void operator delete(void *, void *) {}
#endif /* U_HAVE_PLACEMENT_NEW */
#if U_HAVE_DEBUG_LOCATION_NEW
    static void * operator new(size_t size, const char* file, int line);
    static void operator delete(void* p, const char* file, int line);
#endif /* U_HAVE_DEBUG_LOCATION_NEW */
#endif /* U_OVERRIDE_CXX_ALLOCATION */

};

class UObject : public UMemory
{
public:
    virtual ~UObject();
};

class UnicodeMatcher
{

public:
    virtual ~UnicodeMatcher();
};


class UnicodeReplacer
{

public:
    virtual ~UnicodeReplacer();
};



class UnicodeFunctor : public UObject
{

public:
    virtual ~UnicodeFunctor();
};

class UnicodeFilter : public UnicodeFunctor, public UnicodeMatcher
{

public:
    virtual ~UnicodeFilter();
};

class UVector : public UObject
{
private:
    int32_t count;

    int32_t capacity;

    UHashTok* elements;

    UObjectDeleter *deleter;

    UKeyComparator *comparer;

public:
    UVector(UObjectDeleter *d, UKeyComparator *c, UErrorCode &status);

    virtual ~UVector();

    UBool operator==(const UVector& other);

    inline UBool operator!=(const UVector& other);

    void sortedInsert(void* obj, USortComparator *compare, UErrorCode& ec);

    void* elementAt(int32_t index) const;

    int32_t size(void) const;

    void setSize(int32_t newSize);

    void* orphanElementAt(int32_t index);

    int32_t indexOf(void* obj, int32_t startIndex = 0) const;

    int32_t indexOf(int32_t obj, int32_t startIndex = 0) const;

    UBool contains(void* obj) const;

    UBool contains(int32_t obj) const;

    void assign(const UVector& other, UTokenAssigner *assign, UErrorCode &ec);

    void removeElementAt(int32_t index);

    UBool ensureCapacity(int32_t minimumCapacity, UErrorCode &status);

    void removeAllElements();
private:
    void _init(int32_t initialCapacity, UErrorCode &status);

    int32_t indexOf(UHashTok key, int32_t startIndex = 0, int8_t hint = 0) const;

    void sortedInsert(UHashTok tok, USortComparator *compare, UErrorCode& ec);

};

inline UBool UVector::contains(void* obj) const
{
    return indexOf(obj) >= 0;
}

inline UBool UVector::contains(int32_t obj) const
{
    return indexOf(obj) >= 0;
}

inline UBool UVector::operator!=(const UVector& other)
{
    return !operator==(other);
}


inline int32_t UVector::size(void) const
{
    return count;
}

class Replaceable : public UObject
{

public:
    virtual ~Replaceable();

protected:

    Replaceable();
};


class UnicodeString : public Replaceable
{
public:

    enum EInvariant {
        kInvariant
    };

    inline int32_t length(void) const;
    inline UBool isBogus(void) const;
    inline UBool operator== (const UnicodeString& text) const;
    inline UBool operator!= (const UnicodeString& text) const;
    inline UBool operator> (const UnicodeString& text) const;
    inline UBool operator< (const UnicodeString& text) const;
    inline UBool operator>= (const UnicodeString& text) const;
    inline UBool operator<= (const UnicodeString& text) const;
    UnicodeString &operator=(const UnicodeString &srcText);
    inline  UnicodeString& operator+= (UChar ch);
    inline  UnicodeString& operator+= (USChar32 ch);
    inline UnicodeString& operator+= (const UnicodeString& srcText);

    void setToBogus();
    inline int8_t compare(const UnicodeString& text) const;
    UnicodeString();
    UnicodeString(int32_t capacity, USChar32 c, int32_t count);
    UnicodeString(UChar ch);
    UnicodeString(USChar32 ch);
    UnicodeString(const UChar *text);

    UnicodeString(const UnicodeString& that);

    virtual ~UnicodeString();

    inline UChar charAt(int32_t offset) const;
    inline USChar32 char32At(int32_t offset) const;
    inline UBool truncate(int32_t targetLength);

private:

    inline int8_t
    doCompare(int32_t start,
              int32_t length,
              const UnicodeString& srcText,
              int32_t srcStart,
              int32_t srcLength) const;

    UBool cloneArrayIfNeeded(int32_t newCapacity = -1,
                             int32_t growCapacity = -1,
                             UBool doCopyArray = TRUE,
                             int32_t **pBufferToDelete = 0,
                             UBool forceClone = FALSE);

    int32_t removeRef(void);
    void addRef(void);
    int32_t refCount(void) const;
    // constants
    enum {
        US_STACKBUF_SIZE=7, // Size of stack buffer for small strings
        kInvalidUChar=0xffff, // invalid UChar index
        kGrowSize=128, // grow size for this buffer
        kInvalidHashCode=0, // invalid hash code
        kEmptyHashCode=1, // hash code for empty string

        // bit flag values for fFlags
        kIsBogus=1,         // this string is bogus, i.e., not valid or NULL
        kUsingStackBuffer=2,// fArray==fStackBuffer
        kRefCounted=4,      // there is a refCount field before the characters in fArray
        kBufferIsReadonly=8,// do not write to this buffer
        kOpenGetBuffer=16,  // getBuffer(minCapacity) was called (is "open"),
        // and releaseBuffer(newLength) must be called

        kShortString=kUsingStackBuffer,
        kLongString=kRefCounted,
        kReadonlyAlias=kBufferIsReadonly,
        kWritableAlias=0
    };


    inline UChar doCharAt(int32_t offset)  const;

    void releaseArray(void);

    UnicodeString& doReplace(int32_t start,
                             int32_t length,
                             const UChar *srcChars,
                             int32_t srcStart,
                             int32_t srcLength);

    inline UChar* getArrayStart(void);
    inline const UChar* getArrayStart(void) const;

    void unBogus();

    UBool allocate(int32_t capacity);

    inline void pinIndices(int32_t& start,
                           int32_t& length) const;
    UnicodeString &copyFrom(const UnicodeString &src, UBool fastCopy=FALSE);

    //friend class StringCharacterIterator;   //changed by malin 07-03-28
    //friend class StringThreadTest;


    int32_t   fLength;        // number of characters in fArray
    int32_t   fCapacity;      // sizeof fArray
    UChar     *fArray;        // the Unicode data
    uint16_t  fFlags;         // bit flags: see constants above
    UChar     fStackBuffer [ US_STACKBUF_SIZE ]; // buffer for small strings

};

inline UBool
UnicodeString::isBogus() const
{
    return (UBool)(fFlags & kIsBogus);
}


inline UChar*
UnicodeString::getArrayStart()
{
    return fArray;
}

inline const UChar*
UnicodeString::getArrayStart() const
{
    return fArray;
}


inline void
UnicodeString::pinIndices(int32_t& start,
                          int32_t& _length) const
{
    if(start < 0) {
        start = 0;
    } else if(start > fLength) {
        start = fLength;
    }
    if(_length < 0) {
        _length = 0;
    } else if(_length > (fLength - start)) {
        _length = (fLength - start);
    }
}

inline int8_t
UnicodeString::compare(const UnicodeString& text) const
{
    return doCompare(0, fLength, text, 0, text.fLength);
}

inline int8_t
UnicodeString::doCompare(int32_t start,
                         int32_t length,
                         const UnicodeString& srcText,
                         int32_t srcStart,
                         int32_t srcLength) const
{
    if(srcText.isBogus()) {
        return (int8_t)!isBogus(); // 0 if both are bogus, 1 otherwise
    } else {
        srcText.pinIndices(srcStart, srcLength);
        return doCompare(start, length, srcText.fArray, srcStart, srcLength);
    }
}

inline int32_t
UnicodeString::length() const
{
    return fLength;
}

class ParsePosition : public UObject
{
public:
    ParsePosition()
        : UObject(),
          index(0),
          errorIndex(-1)
    {}

    ParsePosition(int32_t newIndex)
        : UObject(),
          index(newIndex),
          errorIndex(-1)
    {}

    ParsePosition(const ParsePosition& copy)
        : UObject(copy),
          index(copy.index),
          errorIndex(copy.errorIndex)
    {}
    virtual ~ParsePosition();

    ParsePosition&      operator=(const ParsePosition& copy);

    UBool              operator==(const ParsePosition& that) const;

    UBool              operator!=(const ParsePosition& that) const;

private:
    int32_t index;
    int32_t errorIndex;
};

class SymbolTable
{
public:
    enum { SYMBOL_REF = 0x0024 /*$*/ };
    virtual ~SymbolTable();
};


class UnicodeSet : public UnicodeFilter
{

    int32_t len; // length of list used; 0 <= len <= capacity
    int32_t capacity; // capacity of list
    int32_t bufferCapacity; // capacity of buffer
    USChar32* list; // MUST be terminated with HIGH
    USChar32* buffer; // internal buffer, may be NULL

    UVector* strings; // maintained in sorted order
    UnicodeString pat;

public:

    enum {
        MIN_VALUE = 0,
        MAX_VALUE = 0x10ffff
    };

public:

    UnicodeSet();

    UnicodeSet(USChar32 start, USChar32 end);

    UnicodeSet(const UnicodeString& pattern,
               UErrorCode& status);

    UnicodeSet(const UnicodeString& pattern,
               uint32_t options,
               const SymbolTable* symbols,
               UErrorCode& status);
    UnicodeSet(const UnicodeString& pattern, ParsePosition& pos,
               uint32_t options,
               const SymbolTable* symbols,
               UErrorCode& status);

    UnicodeSet(const UnicodeSet& o);

    virtual ~UnicodeSet();
    UnicodeSet& operator=(const UnicodeSet& o);

    virtual UBool operator==(const UnicodeSet& o) const;

    UBool operator!=(const UnicodeSet& o) const;

    virtual UBool contains(USChar32 c) const;

    virtual UBool contains(USChar32 start, USChar32 end) const;

    UBool contains(const UnicodeString& s) const;
    virtual UnicodeSet& complement(USChar32 start, USChar32 end);

    virtual UnicodeSet& add(USChar32 start, USChar32 end);
    UnicodeSet& add(USChar32 c);
    virtual UnicodeSet& addAll(const UnicodeSet& c);

    UnicodeSet& addAll(const UnicodeString& s);
private:

    int32_t findCodePoint(USChar32 c) const;

private:
    static int32_t getSingleCP(const UnicodeString& s);

    void _add(const UnicodeString& s);


    void ensureCapacity(int32_t newLen);
    void ensureBufferCapacity(int32_t newLen);
    void swapBuffers(void);
    UBool allocateStrings();
    void exclusiveOr(const USChar32* other, int32_t otherLen, int8_t polarity);

    void add(const USChar32* other, int32_t otherLen, int8_t polarity);
};


#endif
