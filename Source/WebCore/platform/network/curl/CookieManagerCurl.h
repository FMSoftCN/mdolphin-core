/*
** $Id: CookieManagerCurl.h 1028 2008-03-10 09:06:14Z whzeng $
**
** CookieManagerCurl.h: Cookie management header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-10-30
*/

#ifndef CookieManagerCurl_h 
#define CookieManagerCurl_h 

#include "config.h"

#if ENABLE(COOKIE)

#include <pthread.h>
#include "vector"
#include "KURL.h"
#include "PlatformString.h"
#include "algorithm"
#include "CookieJar.h"
#include "CookieCurl.h"
#include "CookieArchiver.h"
#include "Threading.h"

#define LIMIT_COOKIE 1
#define DEBUG_COOKIE 0

namespace WebCore {

using namespace std;

#define DEFAULT_DOMAINS  (60)
#define DEFAULT_DOMAINCOOKIES  (32)
#define DEFAULT_COOKIES      (240)
#define DEFAULT_COOKIESIZE   (4*1024)


class DomainCookie {
    friend class CookieManager;
    friend class CookieArchiver;
public:
    DomainCookie(String domain);
    String &domain(){ return m_domain;}
    CookieCurl *head();
    int add(CookieCurl *newCookie);
    int accessCount() {return m_accessCount;}
    void remove(CookieCurl *cookie);
    int removeLRUCookies();
    int cookieCount() {return m_cookies;}

private:
    int incAccessCount() {return m_accessCount++;}
    int m_accessCount; 
    bool m_dirty;    /* the host cookie data is dirty */
    int m_cookies;  /* the number of Cookies */
    CookieCurl *m_head;
    String m_domain;   /* domain string */
};

/* Note: This class CookieManager only support single instance. */
class CookieManager {
public:
    static CookieManager *sharedInstance();
    bool removeAll(void);
    void clearCookies(void);

    vector<DomainCookie*> domains() {return m_domains;}
    int cookieCount();
    int setCookieCount(int num);

#ifdef LIMIT_COOKIE
    bool addDomain(DomainCookie *domain);
#endif

    bool set(const KURL &url, const KURL &policyURL, const String &m_value);
    String get(const KURL &url);

    bool enableCookies(bool enable);
    bool cookiesEnabled();
    CookieManager();
    ~CookieManager();
#if DEBUG_COOKIE
    void dump(void);
#endif

#ifdef LIMIT_COOKIE
    void setLimits(int maxCookies, int maxDomainCookies, int maxCookieSize);
    int  getLimits(int *maxCookies, int *maxDomainCookies, int *maxCookieSize);
#endif

protected:
    bool equalCookie(CookieCurl *oldOne, CookieCurl* newOne);
    void splitURI(const KURL &kurl, String &host, String &domain, String &path);
    bool matchDomain(const String &domain, const String &host);
    DomainCookie *lookup(const String &domain);
    CookieCurl *lookup(const DomainCookie *domain, const String &host, const String &path, const String &name)
;
    String matchedCookies(const KURL& kurl , String &path);
    int addCookie(DomainCookie *domain, CookieCurl *cookie);
    int removeCookie(const DomainCookie *domain, CookieCurl *cookie);
#if ENABLE(FILECOOKIE)
    bool setCookieDir(const char *path);
    const char* cookieDir();
#endif

#ifdef LIMIT_COOKIE    
    int removeCookies(DomainCookie *domain);
    int removeLRUDomains(void);
    int removeLRUCookies(DomainCookie *domain);
#endif


private:

#if USE(MULTIPLE_THREADS)
    void lock() { m_lock->lock();}
    void unlock() { m_lock->unlock();}
#else
    void lock() {}
    void unlock() {}
#endif

    vector<DomainCookie*> m_domains;
    int  m_cookies;
    bool m_enableCookie;   /* default true */

#if ENABLE(FILECOOKIE)
    CookieArchiver *m_archiver;
#endif

#if USE(MULTIPLE_THREADS)
    Mutex *m_lock;
#endif

#ifdef LIMIT_COOKIE
    int m_maxDomains;
    int m_maxCookies;       /* the maximum number of the cookies */
    int m_maxDomainCookies; /* the maximum number of cookies belong the same host */
    int m_maxCookieSize;    /* the maximum size of a single cookie */
#endif
};

} /* namespace WebCore */

#endif /* ENABLE(COOKIE) */

#endif /* CookieManagerCurl_h  */
