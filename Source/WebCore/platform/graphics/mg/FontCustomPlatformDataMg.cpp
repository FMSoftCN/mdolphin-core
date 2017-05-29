/* 
** $Id: FontCustomPlatformDataMg.cpp 590 2011-04-22 05:29:20Z lijiangwei $
** 
** FontCustomPlatformDataMg.cpp: The platform implementation for FontCustomPlatformData.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: XiaoweiYan
**
** Create date: 2010-06-01
*/
#include "minigui.h"

#include "config.h"
#include "FontCustomPlatformData.h"

#include "FontPlatformData.h"
#include "SharedBuffer.h"

#include "NotImplemented.h"

namespace WebCore {

FontCustomPlatformData::~FontCustomPlatformData()
{
}

FontPlatformData FontCustomPlatformData::fontPlatformData(int size, bool bold, bool italic, FontRenderingMode)
{
    return FontPlatformData();
}

FontCustomPlatformData* createFontCustomPlatformData(SharedBuffer* buffer)
{
    notImplemented();
    return 0;
}

bool FontCustomPlatformData::supportsFormat(const String& format)
{
    return equalIgnoringCase(format, "truetype") || equalIgnoringCase(format, "opentype") || equalIgnoringCase(format, "woff");
}

}

