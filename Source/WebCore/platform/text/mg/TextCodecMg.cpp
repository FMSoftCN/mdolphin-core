/*
 ** $Id: TextCodecMg.cpp 486 2011-02-18 10:27:00Z xwyan $
 **
 ** TextCodecMg.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-18 
 */
#include "minigui.h"

#include "config.h"
#include "TextCodecMg.h"

#include "Logging.h"
#include "PlatformString.h"
#include <wtf/Assertions.h>
#include <wtf/HashMap.h>
#include <wtf/text/CString.h>
#include "HashSet.h"

using std::min;
using std::auto_ptr;
const size_t gMaxBufferSize = 4096;

namespace WebCore {

static HashSet<TextCodecMgPrivate*> *CodecCache()
{
    static HashSet<TextCodecMgPrivate*> *Codec = new HashSet<TextCodecMgPrivate*>;
    return Codec;
}

static PLOGFONT mgSbcLogFont()
{
    static PLOGFONT hfont = CreateLogFont (NULL, "arial", "ISO-8859-1",
                  FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                  FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                  12, 0);
    return hfont;
}

class TextCodecMgPrivate
{
public:
    static TextCodecMgPrivate * getCodecPrivate (const TextEncoding& encoding);
    static void initCodecPrivate();

    void ref()
    {
        m_refcount++;
    }
    void deref()
    {
        m_refcount--;
        if (m_refcount <= 0)
            delete this;
    }

    PLOGFONT hfont(void) const {return m_hfont;}
    char * encoding(void)const {return m_hfont->charset;}
private:
    TextCodecMgPrivate(const TextEncoding& encoding)
        :m_refcount(0),m_hfont(NULL)
    {
        const char *charset = encoding.name();
        m_hfont = CreateLogFont (NULL, "arial", charset,
                  FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                  FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                  12, 0);
    }

    TextCodecMgPrivate(const char* encoding)
        :m_refcount(0),m_hfont(NULL)
    {
        const char *charset = encoding;
        m_hfont = CreateLogFont (NULL, "arial", charset,
                  FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                  FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                  12, 0);
    }
    ~TextCodecMgPrivate()
    {
        if (CodecCache()->contains(this))
            CodecCache()->remove(this);

        DestroyLogFont(m_hfont);
    }

    int m_refcount;
    PLOGFONT m_hfont;
};


void TextCodecMgPrivate::initCodecPrivate()
{
    static TextCodecMgPrivate* hfont_utf8 = NULL;
    static TextCodecMgPrivate* hfont_utf16 = NULL;

    if (hfont_utf8 == NULL)
    {
        hfont_utf8 = new TextCodecMgPrivate("UTF-8");
        CodecCache()->add(hfont_utf8);
    }
    if (hfont_utf16 == NULL)
    {
#if PLATFORM(BIG_ENDIAN)
        hfont_utf16 = new TextCodecMgPrivate("UTF-16BE");
#else
        hfont_utf16 = new TextCodecMgPrivate("UTF-16LE");
#endif
        CodecCache()->add(hfont_utf16);
    }
}

TextCodecMgPrivate * TextCodecMgPrivate::getCodecPrivate (const TextEncoding& encoding)
{
    TextCodecMgPrivate* textCodec = NULL;
    if (CodecCache()->size() <= 0) 
    {
        TextCodecMgPrivate::initCodecPrivate();
    }
    for(HashSet<TextCodecMgPrivate*>::iterator it = CodecCache()->begin(); it != CodecCache()->end(); ++it)
        if (strcasecmp((*it)->encoding(), encoding.name()) == 0){
            textCodec = (*it);
            break;
        }

    if (!textCodec) {
        textCodec = new TextCodecMgPrivate(encoding);
        CodecCache()->add(textCodec);
        return textCodec;
    } else {
        return textCodec;
    }
}

/*
 * the constructure function of class TextCodecMg
 * Parameters:
 *    encoding         a TextEncoding object which contain a character set encoding cononical name
 * Returns:
 *    none
 */
TextCodecMg::TextCodecMg(const TextEncoding& encoding)
    : m_numBufferedBytes(0)
{
    m_codec = TextCodecMgPrivate::getCodecPrivate(encoding);
}

TextCodecMg::TextCodecMg(const char* name)
    : m_numBufferedBytes(0)
{
    m_codec = TextCodecMgPrivate::getCodecPrivate(TextEncoding(name));
}

TextCodecMg::~TextCodecMg()
{
}

PassOwnPtr<TextCodec> newTextCodecMg(const TextEncoding& encoding, const void*)
{
    return new TextCodecMg(encoding);
}

void TextCodecMg::registerEncodingNames(EncodingNameRegistrar registrar)
{
#ifdef _MGCHARSET_HEBREW
    registrar("ISO8859-8", "ISO8859-8");
    registrar("visual", "ISO8859-8");
#endif /* ISO8859-8 */ 

#ifdef _MGCHARSET_GB
    registrar("EUC-CN", "EUC-CN");
    registrar("xeuccn", "EUC-CN");
    registrar("xgbk", "EUC-CN");
    registrar("csgb231280", "EUC-CN");
    registrar("cngb", "EUC-CN");

    registrar("GB1988-0", "GB1988-0");
#endif /* GB2312 */

/* Treat GB2312 encoding as GBK (its more modern superset), to match other browsers.
 * for encoding GB_2312-80 and several others. So, we need to override this behavior, too.
 */
#ifdef _MGCHARSET_GBK
    registrar("GBK", "GBK");
    registrar("GB2312-0", "GBK");
    registrar("GB2312", "GBK");
    registrar("GB_2312-80", "GBK");
#elif defined(_MGCHARSET_GB) && !defined(_MGCHARSET_GBK)
    registrar("GB2312-0", "GB2312-0");
    registrar("GB2312", "GB2312-0");
    registrar("GB_2312-80", "GB2312-0");
#endif

#ifdef _MGCHARSET_GB18030
    registrar("GB18030-0", "GB18030-0");
#endif /* GB18030 */

#ifdef _MGCHARSET_BIG5
    registrar("Big5", "Big5");
    registrar("cnbig5", "Big5");
    registrar("xxbig5", "Big5");
    registrar("EUC-TW", "EUC-TW");
#endif /* BIG5 */

#ifdef _MGCHARSET_EUCKR
    registrar("EUC-KR", "EUC-KR");
    registrar("KSC5636-0", "KSC5636-0");
    registrar("KSC5601-0", "KSC5601-0");
#endif /* EUC-KR */

#ifdef _MGCHARSET_EUCJP
    registrar("EUC-JP", "EUC-JP");
    registrar("xeuc", "EUC-JP");
    registrar("JISX0201-0", "JISX0201-0");
    registrar("JISX0208-0", "JISX0208-0");
#endif /* EUC-JP */

#ifdef _MGCHARSET_SHIFTJIS
    registrar("SHIFT-JIS", "SHIFT-JIS");
    registrar("JISX0201-1", "JISX0201-1");
    registrar("JISX0208-1", "JISX0208-1");
#endif /* SHIFT-JIS */

#ifdef _MGCHARSET_UNICODE
    registrar("ISO-10646-1", "ISO-10646-1");
    registrar("UTF-8", "UTF-8");
    registrar("utf-8", "UTF-8");
    registrar("unicode11utf8", "UTF-8");
    registrar("unicode20utf8", "UTF-8");
    registrar("xunicode20utf8", "UTF-8");
#endif /* UNICODE*/
}

void TextCodecMg::registerCodecs(TextCodecRegistrar registrar)
{
#ifdef _MGCHARSET_HEBREW
    registrar("ISO8859-8", newTextCodecMg, 0);
#endif /* ISO8859-8 */ 
#ifdef _MGCHARSET_GB
    registrar("GB1988-0", newTextCodecMg, 0);
    registrar("EUC-CN", newTextCodecMg, 0);
    registrar("GB2312-0", newTextCodecMg, 0);
#endif /* GB2312 */
#ifdef _MGCHARSET_GBK
    registrar("GBK", newTextCodecMg, 0);
#endif /* GBK */
#ifdef _MGCHARSET_GB18030
    registrar("GB18030-0", newTextCodecMg, 0);
#endif /* GB18030 */
#ifdef _MGCHARSET_BIG5
    registrar("Big5", newTextCodecMg, 0);
    registrar("EUC-TW", newTextCodecMg, 0);
#endif /* BIG5 */
#ifdef _MGCHARSET_EUCKR
    registrar("EUC-KR", newTextCodecMg, 0);
    registrar("KSC5636-0", newTextCodecMg, 0);
    registrar("KSC5601-0", newTextCodecMg, 0);
#endif /* EUC-KR */
#ifdef _MGCHARSET_EUCJP
    registrar("EUC-JP", newTextCodecMg, 0);
    registrar("JISX0201-0", newTextCodecMg, 0);
    registrar("JISX0208-0", newTextCodecMg, 0);
#endif /* EUC-JP */
#ifdef _MGCHARSET_SHIFTJIS
    registrar("SHIFT-JIS", newTextCodecMg, 0);
    registrar("JISX0201-1", newTextCodecMg, 0);
    registrar("JISX0208-1", newTextCodecMg, 0);
#endif /* SHIFT-JIS */
#ifdef _MGCHARSET_UNICODE
    registrar("ISO-10646-1", newTextCodecMg, 0);
    registrar("UTF-8", newTextCodecMg, 0);
#endif /* UNICODE*/
}

// We strip BOM characters because they can show up both at the start of content
// and inside content, and we never want them to end up in the decoded text.
void TextCodecMg::appendOmittingBOM(Vector<UChar>& v, const UChar* characters, size_t length)
{
    size_t start = 0;
    const UChar BOM = 0xFEFF;
    for (size_t i = 0; i != length; ++i) {
        if (BOM == characters[i]) {
            if (start != i)
                v.append(&characters[start], i - start);
            start = i + 1;
        }
    }
    if (start != length)
        v.append(&characters[start], length - start);
}

/*
 * Transform the codepage(specified by encoding) string in bytes to UTF16 string
 * Parameters:
 *    bytes     character buffer for transformation
 *    length    the number of characters in the buffer bytes
 *    wc32     uc16 or uc32,TRUE is uc32, FALSE is uc16
 * Returns:
 *    a String object that contains the UTF16 string transformed from bytes
 */
String TextCodecMg::decode(const char* bytes, size_t length, bool flush, bool stopOnError, bool& sawError)
{
    Vector<UChar> result;
    const char* conversionBytes;
    size_t conversionLength;
    char* prefixedBytes = 0;

    //for null string, should return directly.
    if (!bytes || (length == 0 && m_numBufferedBytes == 0)) {
        return String();
    }

    if (m_numBufferedBytes) {
        conversionLength = length + m_numBufferedBytes;
        prefixedBytes = static_cast<char*>(fastMalloc(conversionLength));
        memcpy(prefixedBytes, m_bufferedBytes, m_numBufferedBytes);
        memcpy(prefixedBytes + m_numBufferedBytes, bytes, length);
        conversionBytes = prefixedBytes;
        // all buffered bytes are consumed now
        m_numBufferedBytes = 0;
    }
    else {
        // no previously buffered partial data, 
        // just convert the data that was passed in
        conversionBytes = bytes;
        conversionLength = length;
    }

    UChar buffer[gMaxBufferSize];
    int convertedLength, count, leftConvLength = conversionLength;
    size_t countRead = 0;

    do {
        count = MBS2WCSEx (m_codec->hfont(), buffer, FALSE, 
                (const unsigned char*)conversionBytes + countRead, leftConvLength, 
                gMaxBufferSize, &convertedLength);

        if (count <= 0) {
            if (leftConvLength > 3) {
                //should skip one garbled character or '\0'.
                const unsigned char* curBytes = (const unsigned char*)conversionBytes + countRead;
                if (*curBytes == '\0')
                    convertedLength = 1;
                else 
                    count = MBS2WCSEx (mgSbcLogFont(), buffer, FALSE, curBytes, 1,
                            gMaxBufferSize, &convertedLength);
            }
            else {
                //m_numBufferedBytes = 0; // reset state for subsequent calls to decode
                fastFree(prefixedBytes);
                sawError = true;
                if (leftConvLength > 0) {
                    m_numBufferedBytes = leftConvLength;
                    memcpy(m_bufferedBytes, conversionBytes + countRead, m_numBufferedBytes);
                }
                else {
                    m_numBufferedBytes = 0;
                }
                return String();
            }

        }

        countRead += convertedLength;
        leftConvLength -= convertedLength;

        appendOmittingBOM(result, reinterpret_cast<const UChar*>(buffer), count);
    } while (leftConvLength > 3); //avoid to process half word.

    // Partial input at the end of the string may not result in an error being raised. 
    // From the gnome library documentation on g_convert_with_iconv:
    // "Even if the conversion was successful, this may be less than len if there were partial characters at the end of the input."
    // That's why we need to compare conversionLength against countRead 

    m_numBufferedBytes = leftConvLength;
    if (m_numBufferedBytes > 0) {
        if (flush) {
            m_numBufferedBytes = 0; // reset state for subsequent calls to decode
            fastFree(prefixedBytes);
            sawError = true;
            return String();
        }
        memcpy(m_bufferedBytes, conversionBytes + countRead, m_numBufferedBytes);
    }

    fastFree(prefixedBytes);
    return String::adopt(result);
}

/*
 * Transform the UTF16 string in buffer characters to codepage string
 * Parameters:
 *    characters     string buffer of UTF16
 *    length         number of characters in the string buffer characters
 *    wc32           uc16 or uc32,TRUE is uc32, FALSE is uc16
 * Returns:
 *    a String object that contains the codepage(specified by encoding) string transformed
 *    from bytes
 */
CString TextCodecMg::encode(const UChar* characters, size_t length, UnencodableHandling handling)
{
    Vector<char> result;
    char buffer[gMaxBufferSize];

    size_t offset = 0;
    size_t dstoffset = 0;
    const UChar* source;
    int leftWCSLength = length;
    while (leftWCSLength > 0) {
        int mbs_len;
        int convertedLength, sourceLength = (gMaxBufferSize) / 4;
        if (sourceLength > leftWCSLength) {
            sourceLength = leftWCSLength;
        }

        source = characters + offset;
        mbs_len = WCS2MBSEx (m_codec->hfont(), (unsigned char*)buffer, (const void*)source, 
                    sourceLength, FALSE, gMaxBufferSize, &convertedLength);
        if (mbs_len <= 0)
             break;

        result.grow(dstoffset + mbs_len);
        memcpy (result.data() + dstoffset, buffer, mbs_len);
        dstoffset += mbs_len;

        offset += convertedLength;
        leftWCSLength -= convertedLength;
    }

    return CString(result.data(), result.size());
}

} // namespace WebCore
