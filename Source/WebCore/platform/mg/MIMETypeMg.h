/*
** $Id: MIMEType.h 546 2008-01-03 09:26:33Z whzeng $
**
** MIMEType.h: identify mimeType.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2008-01-03
*/

#ifndef MIMEType_h
#define MIMEType_h

#include "KURL.h"
#include "PlatformString.h"

namespace WebCore {


class MIMEType {
public:
    MIMEType(const String &mimeType);

    bool isText() { return m_category == text; }
    bool isImage() { return m_category == image; }
    bool isApplication() { return m_category == application; }
    bool isUnknown() { return m_category == unknown; }
    bool isVideo() { return m_category == video; }
    bool isAudio() { return m_category == audio; }

    int category() { return m_category; }
    String mimeType() { return m_mimeType; }

    enum { unknown, text, image, application, video, audio } m_category;

    static MIMEType guestFromResource(const KURL &url, const void *ptr, size_t size);
private:
    
    String m_mimeType;
};

} /* namespace WebCore */

#endif /* MIMEType_h */
