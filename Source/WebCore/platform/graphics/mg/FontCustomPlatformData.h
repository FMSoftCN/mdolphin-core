/* 
** $Id: FontCustomPlatformData.h 590 2011-04-22 05:29:20Z lijiangwei $
** 
** FontCustomPlatformData.h: The definition for FontCustomPlatformData class.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: XiaoweiYan
**
** Create date: 2010-06-01
*/

#ifndef FontCustomPlatformData_h_
#define FontCustomPlatformData_h_

#include "FontRenderingMode.h"
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>

namespace WebCore {

class SharedBuffer;
class FontPlatformData;

struct FontCustomPlatformData {
    WTF_MAKE_NONCOPYABLE(FontCustomPlatformData);
public:
    FontCustomPlatformData() { }
    ~FontCustomPlatformData();

    FontPlatformData fontPlatformData(int size, bool bold, bool italic, FontRenderingMode = NormalRenderingMode);
    static bool supportsFormat(const String&);
};

FontCustomPlatformData* createFontCustomPlatformData(SharedBuffer* buffer);

} // namespace WebCore

#endif // FontCustomPlatformData_h_
