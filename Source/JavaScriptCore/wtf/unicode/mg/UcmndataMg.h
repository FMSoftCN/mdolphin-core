/*
** $Id: UcmndataMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** UcmndataMg.h: Dealing with unicode common data files.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-25
*/

#ifndef UcmndataMg_h
#define UcmndataMg_h

/**
 * UDataInfo contains the properties about the requested data.
 * This is meta data.
 *
 * <p>This structure may grow in the future, indicated by the
 * <code>size</code> field.</p>
 *
 * <p>The platform data property fields help determine if a data
 * file can be efficiently used on a given machine.
 * The particular fields are of importance only if the data
 * is affected by the properties - if there is integer data
 * with word sizes > 1 byte, char* text, or UChar* text.</p>
 *
 * <p>The implementation for the <code>udata_open[Choice]()</code>
 * functions may reject data based on the value in <code>isBigEndian</code>.
 * No other field is used by the <code>udata</code> API implementation.</p>
 *
 * <p>The <code>dataFormat</code> may be used to identify
 * the kind of data, e.g. a converter table.</p>
 *
 * <p>The <code>formatVersion</code> field should be used to
 * make sure that the format can be interpreted.
 * I may be a good idea to check only for the one or two highest
 * of the version elements to allow the data memory to
 * get more or somewhat rearranged contents, for as long
 * as the using code can still interpret the older contents.</p>
 *
 * <p>The <code>dataVersion</code> field is intended to be a
 * common place to store the source version of the data;
 * for data from the Unicode character database, this could
 * reflect the Unicode version.</p>
 * @stable ICU 2.0
 */
typedef struct {
    /** sizeof(UDataInfo)
     *  @stable ICU 2.0 */
    uint16_t size;

    /** unused, set to 0 
     *  @stable ICU 2.0*/
    uint16_t reservedWord;

    /* platform data properties */
    /** 0 for little-endian machine, 1 for big-endian
     *  @stable ICU 2.0 */
    uint8_t isBigEndian;

    /** see U_CHARSET_FAMILY values in utypes.h 
     *  @stable ICU 2.0*/
    uint8_t charsetFamily;

    /** sizeof(UChar), one of { 1, 2, 4 } 
     *  @stable ICU 2.0*/
    uint8_t sizeofUChar;

    /** unused, set to 0 
     *  @stable ICU 2.0*/
    uint8_t reservedByte;

    /** data format identifier 
     *  @stable ICU 2.0*/
    uint8_t dataFormat[4];

    /** versions: [0] major [1] minor [2] milli [3] micro 
     *  @stable ICU 2.0*/
    uint8_t formatVersion[4];

    /** versions: [0] major [1] minor [2] milli [3] micro 
     *  @stable ICU 2.0*/
    uint8_t dataVersion[4];
} UDataInfo;

typedef struct  {
    uint16_t    headerSize;
    uint8_t     magic1;
    uint8_t     magic2;
} MappedData;


typedef struct  {
    MappedData  dataHeader;
    UDataInfo   info;
} DataHeader;

struct  UDataMemory;
typedef struct UDataMemory UDataMemory;

/*
 *  "Virtual" functions for data lookup.
 *  To call one, given a UDataMemory *p, the code looks like this:
 *     p->vFuncs.Lookup(p, tocEntryName, pErrorCode);
 *          (I sure do wish this was written in C++, not C)
 */
typedef const DataHeader *
(* LookupFn)(const UDataMemory *pData,
             const char *tocEntryName,
             int32_t *pLength,
             UErrorCode *pErrorCode);

typedef uint32_t
(* NumEntriesFn)(const UDataMemory *pData);

typedef struct {
    LookupFn      Lookup;
    NumEntriesFn  NumEntries;
} commonDataFuncs;

struct UDataMemory {
    const commonDataFuncs  *vFuncs;/* Function Pointers for accessing TOC             */

    const DataHeader *pHeader;     /* Header of the memory being described by this    */
                                   /*   UDataMemory object.                           */
    const void       *toc;         /* For common memory, table of contents for        */
                                   /*   the pieces within.                            */
    UBool             heapAllocated;  /* True if this UDataMemory Object is on the    */
                                   /*  heap and thus needs to be deleted when closed. */

    void             *mapAddr;     /* For mapped or allocated memory, the start addr. */
                                   /* Only non-null if a close operation should unmap */
                                   /*  the associated data.                           */
    void             *map;         /* Handle, or other data, OS dependent.            */
                                   /* Only non-null if a close operation should unmap */
                                   /*  the associated data, and additional info       */
                                   /*   beyond the mapAddr is needed to do that.      */
    int32_t           length;      /* Length of the data in bytes; -1 if unknown.     */
};
#endif
