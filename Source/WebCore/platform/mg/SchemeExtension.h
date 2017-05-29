/*
** $Id: SchemeExtension.h 658 2007-11-28 05:06:17Z whzeng $
**
** SchemeExtension.h: Scheme extension header file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-11-28
*/

#ifndef SchemeExtension_h
#define SchemeExtension_h

#include "config.h"
#include "minigui.h"
#include "mdolphin.h"
#include "KURL.h"
#include "PlatformString.h" 
#include <wtf/Vector.h>

namespace WebCore {

#if ENABLE(SCHEMEEXTENSION)
struct SchemeHandler {
    String scheme;
    MDCB_SCHEME_HANDLER cb;
    void *param;
};

bool RegisterSchemeHandler(const char *scheme, MDCB_SCHEME_HANDLER cb, void *param);
bool UnregisterSchemeHandler(const char *scheme);
bool scheduleScheme(const KURL &url);
#endif /* ENABLE(SCHEMEEXTENSION) */

bool isSupportedInternalScheme(const char *scheme);

} /* namespace WebCore */

#endif /* SchemeExtension_h */
