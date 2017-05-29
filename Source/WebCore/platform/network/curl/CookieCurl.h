/*
** $Id: CookieCurl.h 546 2007-10-30 09:26:33Z whzeng $
**
** CookieCurl.h: Cookie header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-10-30
*/

#ifndef CookieCurl_h
#define CookieCurl_h

#include "config.h"
#include "PlatformString.h"

#if ENABLE(COOKIE)

namespace WebCore {

class CookieManager;

class CookieCurl {
    friend class DomainCookie;
    friend class CookieManager;
    friend class CookieArchiver;

public:
    CookieCurl();
    ~CookieCurl();
    void parse(String &cookie, String &domain);
    bool isExpired();
    int accessCount() {return m_accessCount; };

protected:
    int incAccessCount(){return m_accessCount++;}
    bool parse(String &cookie, String &key, String &value);
    bool expireDate(String &cookie);
    
private:    
    int m_accessCount;
    CookieCurl *m_next;
    CookieCurl *m_prev;
    String m_name;
    String m_path;
    String m_host;
    String m_value; 
    time_t m_expiry;
    bool m_isSecure;
    bool m_isHttpOnly;
    bool m_isDomain;
};

} /* namespace WebCore */

#endif /* ENABLE(COOKIE) */
#endif /* Cookie_h */
