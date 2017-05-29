/*
** $Id: SchemeExtension.cpp 658 2007-11-28 05:06:17Z whzeng $
**
** SchemeExtension.cpp: Scheme extension header file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-11-28
*/

#include "SchemeExtension.h"
#include "PlatformString.h"
#include "CString.h"
#include <wtf/Forward.h>

namespace WebCore {

#ifndef TABLE_SIZE
#define TABLE_SIZE(tab) (sizeof(tab)/sizeof(tab[0]))
#endif

bool isSupportedInternalScheme(const char *scheme)
{
    if (!scheme)
        return false;

    static const char *builtinSchemes[] = {"http", "ftp", "https", "ftps", "file", "data"};
    
    for (size_t i=0; i<TABLE_SIZE(builtinSchemes); i++) {
        if (!strcasecmp(builtinSchemes[i], scheme))
            return true;
    }
    return false;
}

#if ENABLE(SCHEMEEXTENSION)

static Vector<SchemeHandler> *schemeHandler(void)
{
    static Vector<SchemeHandler> *handler = new Vector<SchemeHandler>;
    return handler;
}

static bool handleExtensionScheme(const KURL &url)
{
    Vector<SchemeHandler> *schemes= schemeHandler();
    if (! schemes)
        return false;

    size_t size = schemes->size();
    for (size_t i=0; i<size; i++) {
        SchemeHandler *handler = &schemes->at(i); 
        if (equalIgnoringCase(handler->scheme, url.protocol())) {
            (void)(*handler->cb)(url.string().utf8().data(), handler->param);
            return true;
        }
    }
    return false;
}

bool scheduleScheme(const KURL &url)
{
    if (isSupportedInternalScheme(url.protocol().utf8().data()))
        return false;

    if (handleExtensionScheme(url))
        return true;

    return false;
}

bool RegisterSchemeHandler(const char *scheme, MDCB_SCHEME_HANDLER cb, void *param)
{
    if (! scheme || !cb)
        return false;

    if (isSupportedInternalScheme(scheme))
        return false;

    Vector<SchemeHandler> *handler = schemeHandler();
    if (! handler)
        return false;

    int csize = strlen(scheme);

    if (! validScheme(scheme, csize))
        return false;

    String strScheme(scheme, csize);
    size_t size = handler->size();
    for (size_t i=0; i<size; i++) {
        if (equalIgnoringCase(handler->at(i).scheme, strScheme)) {
            handler->at(i).cb = cb;
            handler->at(i).param = param;
            return true;
        }
    }

    handler->resize(size+1);
    handler->at(size).scheme = strScheme;
    handler->at(size).cb = cb;
    handler->at(size).param = param;
    return true;
}

bool UnregisterSchemeHandler(const char *scheme)
{
    if (! scheme)
        return false;

    Vector<SchemeHandler> *handler = schemeHandler();
    if (! handler)
        return false;

    int csize = strlen(scheme);
    
    if (! validScheme(scheme, csize))
        return false;

    String strScheme(scheme, csize);
    size_t size = handler->size();
    for (size_t i=0; i<size; i++) {
        if (equalIgnoringCase(handler->at(i).scheme, strScheme)) {
            handler->remove(i);
            return true;
        }
    }
    return false;
}

#endif /* ENABLE(SCHEMEEXTENSION) */

} /* namespace WebCore */

