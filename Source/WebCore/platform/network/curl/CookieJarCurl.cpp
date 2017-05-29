/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "CookieJar.h"

#include "Cookie.h"
#include "Document.h"
#include "KURL.h"
#include "PlatformString.h"
#include "StringHash.h"

#if PLATFORM(MG)
#include "CookieManagerCurl.h"
#include "NotImplemented.h"
#else
#include <wtf/HashMap.h>
#endif

namespace WebCore {
#if PLATFORM(MG)

#if ENABLE(COOKIE)
void setCookies(Document*, const KURL& url, const String& value)
{
    CookieManager::sharedInstance()->set(url, KURL() /* for compatible*/, value);
}

String cookies(const Document*, const KURL& url)
{
    return CookieManager::sharedInstance()->get(url);
}

bool cookiesEnabled(const Document* /*document*/)
{
    return CookieManager::sharedInstance()->cookiesEnabled();
}

void enableCookies(bool enabled)
{
    CookieManager::sharedInstance()->enableCookies(enabled);
    return;
}

void clearCookies()
{
    return CookieManager::sharedInstance()->clearCookies();
}
// added in 3.0
String cookieRequestHeaderFieldValue(const Document* document, const KURL& url)
{
    return cookies(document, url);
}

bool getRawCookies(const Document*, const KURL&, Vector<Cookie>& rawCookies)
{
    notImplemented();
    return false;
}

void deleteCookie(const Document*, const KURL&, const String&)
{
    notImplemented();
    return;
}

void setCookieStoragePrivateBrowsingEnabled(bool enabled)
{
    // FIXME: Not yet implemented
}
#else

void setCookies(Document* /*document*/, const KURL& url, const String& value)
{
    return;
}

String cookies(const Document* /*document*/, const KURL& url)
{
    return String();
}

bool cookiesEnabled(const Document* /*document*/)
{
    return false;
}

void enableCookies(bool enabled)
{
    return ;
}
void clearCookies()
{
    return;
}

String cookieRequestHeaderFieldValue(const Document* /*document*/, const KURL& url)
{
    return String();
}

bool getRawCookies(const Document*, const KURL&, Vector<Cookie>& rawCookies)
{
    return false;
}

void deleteCookie(const Document*, const KURL&, const String&)
{
    return;
}

void setCookieStoragePrivateBrowsingEnabled(bool enabled)
{
    // FIXME: Not yet implemented
}
#endif /* ENABLE(COOKIE) */

#else
static HashMap<String, String> cookieJar;

void setCookies(Document* /*document*/, const KURL& url, const String& value)
{
    cookieJar.set(url.string(), value);
}

String cookies(const Document* /*document*/, const KURL& url)
{
    return cookieJar.get(url.string());
}

String cookieRequestHeaderFieldValue(const Document* /*document*/, const KURL& url)
{
    // FIXME: include HttpOnly cookie.
    return cookieJar.get(url.string());
}

bool cookiesEnabled(const Document* /*document*/)
{
    return true;
}

bool getRawCookies(const Document*, const KURL&, Vector<Cookie>& rawCookies)
{
    // FIXME: Not yet implemented
    rawCookies.clear();
    return false; // return true when implemented
}

void deleteCookie(const Document*, const KURL&, const String&)
{
    // FIXME: Not yet implemented
}

#if !PLATFORM(EFL)
void setCookieStoragePrivateBrowsingEnabled(bool enabled)
{
    // FIXME: Not yet implemented
}
#endif

void getHostnamesWithCookies(HashSet<String>& hostnames)
{
    // FIXME: Not yet implemented
}

void deleteCookiesForHostname(const String& hostname)
{
    // FIXME: Not yet implemented
}

void deleteAllCookies()
{
    // FIXME: Not yet implemented
}
#endif

}
