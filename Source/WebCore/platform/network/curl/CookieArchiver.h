/*
** $Id: CookieArchiver.h 546 2007-12-24 09:26:33Z whzeng $
**
** CookieArchiver.h: load and save cookies.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-12-24
*/

#include "config.h"

#ifndef CookieArchiver_h
#define CookieArchiver_h

#if ENABLE(FILECOOKIE)
#include "FileSystemMg.h"

namespace WebCore {

class CookieCurl;
class CookieManager;
class DomainCookie;

class CookieArchiver {
public:
    CookieArchiver(char* path = 0);
    ~CookieArchiver();

    int load(CookieManager* mgr);
    int save(DomainCookie* domain);
    bool save(CookieManager* mgr);
    void deleteDomain(DomainCookie* domain);

    bool setCookieDir(const char* path);
    const char* cookieDir();

protected:    
    bool serialize(CookieCurl* cookie, HFile fp);
    int loadDomain(DomainCookie* domain);

private:
    char *m_cookieDir;
    bool m_isDefaultDir;
};

} /* namespace WebCore */

#endif /* ENABLE(FILECOOKIE) */
#endif /* CookieArchiver_h */
