/*
** $Id: CookieManagerCurl.cpp 1116 2008-03-28 03:39:00Z whzeng $
**
** CookieManagerCurl.cpp: Cookie management implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-4-24
*/

#include "config.h"

#if ENABLE(COOKIE)

#include "CString.h"
#include "CookieManagerCurl.h"
#if USE(MULTIPLE_THREADS)
#include "Threading.h"    
#endif
#include <stdio.h>

using namespace std;

namespace WebCore{ 

#define MAX_DOMAINS      (300)
#define MAX_DOMAINCOOKIES  (64)
#define MIN_DOMAINCOOKIES  (10)
#define MAX_COOKIES      (512)
#define MIN_COOKIES      (64)
#define MAX_COOKIESIZE   (8*1024)
#define MIN_COOKIESIZE   (512)

DomainCookie::DomainCookie(String domain)
    : m_accessCount(1)
    , m_dirty(true)
    , m_cookies(0)
    , m_head(NULL)
    , m_domain(domain)
{
}

int DomainCookie::add(CookieCurl *newCookie)
{
    if (! newCookie)
        return 0;

    if (! m_head) {
        ASSERT(m_cookies == 0);
        m_head = newCookie;
    }
    else {
        m_head->m_prev = newCookie;
        newCookie->m_next = m_head;
        m_head = newCookie;
    }

    m_cookies++;
    return 1;
}

void DomainCookie::remove(CookieCurl *cookie)
{
    ASSERT(cookie);

    if (cookie->m_next)
        cookie->m_next->m_prev = cookie->m_prev;
    if (cookie->m_prev)
        cookie->m_prev->m_next = cookie->m_next;
    else {
        m_head = m_head->m_next;
        if (m_head)
            m_head->m_prev = NULL;
    }
    delete cookie;
    m_cookies--;
}

inline bool operator<(DomainCookie &a, DomainCookie &b)
{
    return a.accessCount() < b.accessCount();
}

CookieManager::CookieManager()
{
#if LIMIT_COOKIE
    m_maxDomains = DEFAULT_DOMAINS;
    m_maxDomainCookies = DEFAULT_DOMAINCOOKIES;
    m_maxCookies = DEFAULT_COOKIES;
    m_maxCookieSize = DEFAULT_COOKIESIZE;
#endif
    m_enableCookie = true;
    m_cookies = 0;

#if ENABLE(FILECOOKIE)
    //printf("enable file cookie\n");
    m_archiver = new CookieArchiver(); 
    m_archiver->load(this);
#endif

#if USE(MULTIPLE_THREADS)
    m_lock = new Mutex;
#endif
}

CookieManager::~CookieManager()
{
    for (unsigned int i=0; i<m_domains.size(); i++) {
        removeCookies(m_domains[i]);
        delete m_domains[i]; 
    }
    m_domains.erase(m_domains.begin(), m_domains.end());

#if USE(MULTIPLE_THREADS)
    delete m_lock;
#endif
}

CookieManager *CookieManager::sharedInstance()
{
    static CookieManager *sharedCookieManager = NULL;
    if (! sharedCookieManager)
       sharedCookieManager = new CookieManager();
    return sharedCookieManager;
}

int CookieManager::removeCookie(const DomainCookie *domain, CookieCurl *cookie)
{
    if (! cookie)
        return 0;
    const_cast<DomainCookie*>(domain)->remove(cookie);
    m_cookies--;
    return 1;
}

int CookieManager::addCookie(DomainCookie *domain, CookieCurl* newCookie)
{
    ASSERT(newCookie);

    if (domain->m_cookies > m_maxDomainCookies)
        removeLRUCookies(domain);

    if (m_cookies >= m_maxCookies)
        removeLRUDomains();

    domain->add(newCookie);
    m_cookies++;
    return 1;
}

bool CookieManager::set(const KURL &url, const KURL &policyURL, const String& value)
{
    if (url.isEmpty() || value.isEmpty() || !cookiesEnabled())
        return false;

    lock();

    String cookiestr= value;

    /* Split URL to domain,path,host */
    String strDomain;
    String urlHost, urlDomain, urlPath;
    splitURI(url, urlHost, urlDomain, urlPath);

    /* Parse cookie which from HTTP header */
    CookieCurl *newCookie = new CookieCurl();
    newCookie->parse(cookiestr, strDomain);

	if (strDomain.isEmpty()) {
        if (urlDomain.startsWith("www.", false))
            strDomain = urlDomain;
        else
            strDomain = urlHost;
        newCookie->m_host = urlHost;
    }
    else
        newCookie->m_host = strDomain;

    if (newCookie->m_path.isEmpty())
        newCookie->m_path = urlPath;

    if (strDomain.startsWith("."))
        strDomain = strDomain.right(strDomain.length()-1);

    bool isNewDomain = false;
    DomainCookie *domain = lookup(strDomain);
    if (!domain) {
        domain = new DomainCookie(strDomain);
        isNewDomain = true;
        addDomain(domain);
    }

    /* Look up the domain for the new cookie */
    CookieCurl *pCookie = lookup(domain, newCookie->m_host, newCookie->m_path, newCookie->m_name);
    removeCookie(domain, pCookie);

    if (newCookie->isExpired())
        delete newCookie;
    else 
        addCookie(domain, newCookie);

#if ENABLE(FILECOOKIE)
    if (isNewDomain)
        m_archiver->save(this);
    m_archiver->save(domain);
#endif

    unlock();
    return true;
}

bool CookieManager::addDomain(DomainCookie *domain)
{
    ASSERT(domain);

    /* check the number of domains */
    int size = m_domains.size();
    if (size >= m_maxDomains)
        removeLRUDomains();
    m_domains.insert(m_domains.begin(), domain);
    return true;
}

DomainCookie *CookieManager::lookup(const String &domain)
{
    int size = m_domains.size();
    for (int i=0; i<size; i++) {
        if (equalIgnoringCase(m_domains[i]->domain(), domain))
            return m_domains[i];
    }
    return NULL;
}

CookieCurl *CookieManager::lookup(const DomainCookie *domain,
        const String &host, const String &path, const String &name)
{
    CookieCurl *pCookie = domain->m_head;
    while (pCookie) {
        if (equalIgnoringCase(pCookie->m_path, path) && (pCookie->m_name == name))
            return pCookie;
        pCookie = pCookie->m_next;
    }
    return NULL;
}

bool CookieManager::matchDomain(const String &domain, const String &host)
{
    // if we have a non-domain cookie, require an exact match between domain and host.
    // RFC2109 specifies this behavior; it allows a site to prevent its subdomains
    // from accessing a cookie, for whatever reason.

    // we have a domain cookie; test the following two cases:
    /*
     * normal case for hostName = x<domainName>
     *    e.g., hostName = home.netscape.com
     *          domainName = .netscape.com
     *
     * special case for domainName = .hostName
     *    e.g., hostName = netscape.com
     *          domainName = .netscape.com
     */
    // the lengthDiff tests are for efficiency, so we do only one equalIgnoringCase()
    int domainLength = domain.length();
    int lengthDiff = host.length() - domainLength; 

    // case for host & domain equal
    // (e.g. .netscape.com & .netscape.com)
    // this gives us slightly more efficiency, since we don't have
    // to call up Substring().
    if (!lengthDiff)
        return equalIgnoringCase(domain, host);

    // normal case
    if (lengthDiff > 0)
        return equalIgnoringCase(domain, host.substring(lengthDiff, domainLength));

    // special case
    if (-1 == lengthDiff)
        return equalIgnoringCase(domain.substring(1, domainLength-1), host);
  
    // no match
    return false; 
}

String CookieManager::matchedCookies(const KURL& kuri, String &path)
{
    String strCookie;
    String host = kuri.host();
    bool isSecure = (kuri.protocol() == "https");

    for (size_t i=0; i<m_domains.size(); i++) {
        DomainCookie *domain = m_domains[i];
        if (! matchDomain(domain->m_domain, host))
            continue;

        CookieCurl *pCookie = domain->m_head;
        while (pCookie) {
            CookieCurl *pNext = pCookie->m_next;

            if (pCookie->m_isSecure && (!isSecure))
                goto NextCookie;

            if (! pCookie->isExpired()) {
                if (path.startsWith(pCookie->m_path) && ((pCookie->m_isDomain) 
                            || (!pCookie->m_isDomain && matchDomain(pCookie->m_host, host)))) {
                    if (!strCookie.length()) {
                        if (pCookie->m_name.isEmpty())
                            strCookie = pCookie->m_value;
                        else
                            strCookie = pCookie->m_name + "=" + pCookie->m_value;
                    }else {
                        if (pCookie->m_name.isEmpty())
                            strCookie += "; " + pCookie->m_value;
                        else
                            strCookie += "; " + pCookie->m_name + "=" + pCookie->m_value;
                    }
                    domain->incAccessCount();
                    pCookie->incAccessCount();
                }
            }
            else
                removeCookie(domain, pCookie);

NextCookie:            
            pCookie = pNext;
        }
    }
    return strCookie;
}

String CookieManager::get(const KURL &url)
{
    if (url.isEmpty() || !cookiesEnabled())
        return String("");

    lock();

    String strHost, strPath, strDomain ;
    splitURI(url, strHost, strDomain, strPath);

    String cookie = matchedCookies(url, strPath);
    
    unlock();
    return cookie;
}

bool CookieManager::removeAll(void)
{
    lock();

    unlock();
    return true;
}

void CookieManager::clearCookies(void)
{
#define MAX_FILENAME 256
    lock();

    for (unsigned int i=0; i<m_domains.size(); i++) {
        removeCookies(m_domains[i]);
#if ENABLE(FILECOOKIE)    
        m_archiver->deleteDomain(m_domains[i]);
#endif
    }

#if ENABLE(FILECOOKIE)
    char fileName[MAX_FILENAME];
    snprintf(fileName, sizeof(fileName)-1, "%s/index.dat", cookieDir());
    deleteFile(fileName);
#endif
    unlock();

    return;
}

int CookieManager::cookieCount()
{
    return m_cookies;
}

int CookieManager::setCookieCount(int num)
{
    ASSERT (num <= m_maxCookies);
    m_cookies = num;
    return num;
}

bool CookieManager::enableCookies(bool enabled)
{
    m_enableCookie = enabled;
    return m_enableCookie;
}

bool CookieManager::cookiesEnabled()
{
   return m_enableCookie;
}

int CookieManager::removeLRUDomains()
{
    /* check the number of domains */
    int reserved = m_domains.size() /3;
    int removed = m_domains.size() / 3;

    partial_sort(m_domains.begin() + reserved, m_domains.end(), m_domains.end());

    for (int i=0; i<removed; i++) {
        removeCookies(m_domains[i]);
#if ENABLE(FILECOOKIE)    
        m_archiver->deleteDomain(m_domains[i]);
#endif
    }

    m_domains.erase(m_domains.begin() + reserved, m_domains.end() - removed);
    return removed;
}

int DomainCookie::removeLRUCookies()
{
    if ( !(m_head && m_cookies))
        return 0;

    /* insert last recently used cookies into LRU list */
    vector<CookieCurl*> cookies;
    CookieCurl* cur = m_head;
    while (cur) {
        cookies.insert(cookies.begin(), cur); 
        cur = cur->m_next;
    }

    int reserved = cookies.size() / 3;
    int removed = cookies.size() / 3;

    /* partial sort cookie by accessed count */
    partial_sort(cookies.begin() + reserved, cookies.end(), cookies.end());

    /* remove LRU cookies */
    for (int i=0; i<removed; i++) {
        cur = cookies[i];
        if (cookies[i]->m_prev)
            cookies[i]->m_prev->m_next = cookies[i]->m_next;
        if (cookies[i]->m_next)
            cookies[i]->m_next->m_prev = cookies[i]->m_prev;
        delete cookies[i];
    }
    m_cookies -= removed;
    return removed;
}

int CookieManager::removeLRUCookies(DomainCookie *domain)
{
    ASSERT(domain);
    int old = domain->m_cookies;
    m_cookies -= old;

    domain->removeLRUCookies();
    
    m_cookies = domain->m_cookies;
    return old - domain->m_cookies;
}

int  CookieManager::removeCookies(DomainCookie *domain)
{
    CookieCurl *pCookie = domain->m_head;
    CookieCurl *pNext;
    while (pCookie) {
        pNext = pCookie->m_next;
        removeCookie(domain, pCookie);
        pCookie = pNext;
    }
    return 0;
}

void CookieManager::splitURI(const KURL &kurl, String &host, String &domain, String &path)
{
	int pos;

    if (kurl.hasPath()) {
        path = kurl.path();
        pos = path.reverseFind('/');
        path = path.left(pos+1);
    }

	host = kurl.host(); 
	domain = kurl.host();
	pos = domain.find('.');
    if (pos != -1)
	    domain = domain.substring(pos, domain.length() - pos);
}

#if LIMIT_COOKIE
void CookieManager::setLimits(int maxCookies, int maxDomainCookies, int maxCookieSize)
{
    if ((maxCookies <= MIN_COOKIES) || (maxCookies> MAX_COOKIES))
        m_maxCookies = DEFAULT_COOKIES;
    else
        m_maxCookies = maxCookies;

    if ((maxDomainCookies <= MIN_DOMAINCOOKIES) || (maxDomainCookies> MAX_DOMAINCOOKIES))
        m_maxDomainCookies = DEFAULT_DOMAINCOOKIES;
    else 
        m_maxDomainCookies = maxDomainCookies;

    if ((maxCookieSize <= MIN_COOKIESIZE) || (maxCookieSize > MAX_COOKIESIZE))     
        m_maxCookieSize = DEFAULT_COOKIESIZE;
    else 
        m_maxCookieSize = maxCookieSize;
}

int CookieManager::getLimits(int *maxCookies, int *maxDomainCookies, int *maxCookieSize)
{
    if (! (maxCookies && maxDomainCookies && maxCookieSize))
       return -1;
    
    *maxCookies = m_maxCookies;
    *maxDomainCookies = m_maxDomainCookies;
    *maxCookieSize = m_maxCookieSize;
    return 0;
}
#endif /* LIMIT_COOKIE */

#if ENABLE(FILECOOKIE)
bool CookieManager::setCookieDir(const char* path)
{
    ASSERT(m_archiver);
    return m_archiver->setCookieDir(path);
}

const char *CookieManager::cookieDir()
{
    ASSERT(m_archiver);
    return m_archiver->cookieDir();
}

#endif

#if DEBUG_COOKIE
void CookieManager::dump(void)
{
    CookieCurl* pCookie = NULL, *loop;
    int cookies = 0;
    
    fprintf(stderr, "\n*****************Begin Dump Cookies************");
    for (unsigned int i=0; i<m_domains.size(); i++) {
        int domaincookies = 0;
        pCookie = m_domains[i]->m_head;
        fprintf(stderr, "\ndomain=[%s]:\n", m_domains[i]->m_domain.latin1().data());
        for (loop=pCookie; loop; loop=loop->m_next) {
            fprintf(stderr, "[Host=%s]:", loop->m_host.latin1().data());
            fprintf(stderr, "[path=%s];",loop->m_path.latin1().data());
            fprintf(stderr, "[%s=",loop->m_name.latin1().data());
            fprintf(stderr, "%s];",loop->m_value.latin1().data());
            fprintf(stderr, "[expire=%d]\n", (int)loop->m_expiry);
            cookies++;
            domaincookies++;
        }
        
        fprintf(stderr, "\nHost Cookies=%d", domaincookies);
        if (m_domains[i]->m_cookies != domaincookies)
            fprintf(stderr, "\n%d != %d", m_domains[i]->m_cookies, domaincookies);
        fprintf(stderr, "\n-----------------------------------");
    }
    
    if (m_cookies != cookies)
        fprintf(stderr, "\n%d != %d", m_cookies, cookies);
    fprintf(stderr, "\nReal Cookies=%d, Domains=%d, Record:Cookies=%d", 
        cookies, m_domains.size(), m_cookies);
    fprintf(stderr, "\n*****************End Dump Cookies**************\n");
}
#endif

}/*namespace WebCore*/

#endif /* ENABLE(COOKIE) */
