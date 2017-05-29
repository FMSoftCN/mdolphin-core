/*
 ** $Id: TextCodecMg.h 233 2010-07-26 10:03:48Z xwyan $
 **
 ** TextCodecMg.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-21 
 */

#ifndef TextCodecMg_h
#define TextCodecMg_h

#include "TextCodec.h"
#include "TextEncoding.h"

namespace WebCore {

class TextCodecMgPrivate;

class TextCodecMg : public TextCodec {
public:
    static void registerEncodingNames(EncodingNameRegistrar);
    static void registerCodecs(TextCodecRegistrar);

    TextCodecMg(const TextEncoding&);
    TextCodecMg(const char* name);
    virtual ~TextCodecMg();

    virtual String decode(const char*, size_t length, bool flush, bool stopOnError, bool& sawError);
    virtual CString encode(const UChar*, size_t length, UnencodableHandling);

private:
    void appendOmittingBOM(Vector<UChar>& v, const UChar* characters, size_t length);
    TextCodecMgPrivate* m_codec;
    size_t m_numBufferedBytes;
    unsigned char m_bufferedBytes[16]; // bigger than any single multi-byte character        
};

}
#endif

