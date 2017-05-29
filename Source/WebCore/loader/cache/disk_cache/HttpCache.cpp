
#include "config.h"
#include "KURL.h"
#include "mdolphin.h"
#include "CString.h"
#include "PlatformString.h"
#include <assert.h>
#include "MemoryCache.h"

#include <wtf/RefPtr.h>
#include <wtf/Assertions.h>              

#include "HttpCache.h"
#include "CachedResource.h"
#include "SharedBuffer.h"
#include "cache_type.h"
#include <curl/curl.h>

#ifndef NDEBUG
#define DISKCACHE_DEBUG_OUTPUT(fmt,...) fprintf(stderr, "File:%s, Line:%d, Function:%s, "#fmt"\n", __FILE__, __LINE__, __FUNCTION__,## __VA_ARGS__)
#else
#define DISKCACHE_DEBUG_OUTPUT(fmt,...)
#endif

namespace WebCore 
{
// the following function is used for disk cache now
bool setResponseFromDiskCache(const String& responseURL, String& header, CachedResource* resource)
{
    ResourceResponse& response = const_cast<ResourceResponse&>(resource->response());
    
    //Vector<String> headers = header.split('\0');
    WebCore::KURL kurl(WebCore::ParsedURLString, responseURL);
    response.setURL(kurl);   //add response url for combining url error

    Vector<String> headers;
    header.split('\0',headers);
    
    Vector<String>::iterator it = headers.begin();
    for (++it; it!=headers.end(); ++it)
    {
        String str = *it;
        
        int pos = str.find("Content-Type: ");
        if (-1 != pos)
        {
            int pos2 = str.find("charset: ");
           
            if (-1 == pos2)
            {
                (response).setMimeType(
                        str.right(str.utf8().length()-pos-strlen("Content-Type: ")));
            } else {
                (response).setMimeType(
                        str.substring(pos+strlen("Content-Type: "), pos2-2-pos-strlen("Content-Type: ")));

                (response).setTextEncodingName(
                        str.right(str.utf8().length()-pos2-strlen("charset: ")));

            }

            continue;
        }

        pos = str.find("Last-Modified: ");
        if (-1 != pos)
        {
            String value = str.right(str.utf8().length()-pos-strlen("Last-Modified: "));
            time_t tm = curl_getdate(value.utf8().data(), NULL);
            (response).setHTTPHeaderField("Last-Modified", value);
            (response).setLastModifiedDate(tm);

            continue;
        }

        pos = str.find("ETag: ");
        if (-1 != pos)
        {
            String value = str.right(str.utf8().length()-pos-strlen("ETag: "));
            (response).setHTTPHeaderField("ETag", value);
            continue;
        }

        pos = str.find("Expires: ");
        if (-1 != pos)
        {
            String value = str.right(str.utf8().length()-pos-strlen("Expires: "));
            time_t tm = curl_getdate(value.utf8().data(), NULL);
            (response).setExpirationDate(tm);

            continue;
        }

        pos = str.find("Cache-Control: ");
        if (-1 != pos)
        {
            (response).setHTTPHeaderField("cache-control", str);

            if (str.contains("no-cache", false) || str.contains("no-store", false)) {
                (response).setExpirationDate(time(0));
                continue;
            }

            int pos = str.find("max-age=", false);
            if (-1 != pos) {
                int sec = atoi(str.substring(pos+8, str.utf8().length()-pos-8).utf8().data()); 
                response.setExpirationDate(sec);
            }

            continue;
        }

        pos = str.find("Content-Length: ");
        if (-1!=pos)
        {
            String value = str.right(str.utf8().length()-pos-strlen("Content-Length: "));
            (response).setExpectedContentLength(atoll(value.utf8().data()));

            continue;
        }
    }

    return true;
    //set resource m_response according to header
}
}

namespace disk_cache{

// disk cache entry data indices..
enum {
  mResponseInfoIndex,
  mResponseContentIndex
};

//HttpCache::ActiveEntriesMap HttpCache::m_activeEntries;

HttpCache::HttpCache(unsigned capacity, const char* cacheDir) : 
    m_path(cacheDir),m_maxBytes(capacity),m_cacheType(net::DISK_CACHE)
{
    // init backend, so to load map file
    m_backend = CreateCacheBackend(m_path, true, m_maxBytes, m_cacheType);
}

HttpCache::~HttpCache()
{
    static int k, l;

    DISKCACHE_DEBUG_OUTPUT("~HttpCache");
    // If we have any active entries remaining, then we need to deactivate them.
    while (!m_activeEntries.isEmpty()) {
        ActiveEntry* entry = m_activeEntries.begin()->second;
        deactivateEntry(entry);
        ++k;
    }    

    DISKCACHE_DEBUG_OUTPUT("~activeEntries [%d]", k);
    //Delete m_doomedEntries
    ActiveEntriesSet::iterator it = m_doomedEntries.begin();
    for (; it != m_doomedEntries.end(); ++it)
    {
        ++l;
        delete *it;
    }

    DISKCACHE_DEBUG_OUTPUT("~doomedEntries [%d]", l);
    DISKCACHE_DEBUG_OUTPUT("*********** DISK CACHE used size [%d]*****************", m_backend->GetUsedSize());
    if (backend())
        delete backend();
}

void HttpCache::deactivateEntry(ActiveEntry* entry) {
  assert(!entry->doomed);

  WTF::String key = entry->disk_entry->GetKey(); 
  if (key.isEmpty())
    return slowDeactivateEntry(entry);

  ActiveEntriesMap::iterator it = m_activeEntries.find(key);
  assert(it != m_activeEntries.end());
  assert(it->second == entry);

  m_activeEntries.remove(it);
  delete entry;
}

// We don't know this entry's key so we have to find it without it.
void HttpCache::slowDeactivateEntry(ActiveEntry* entry) {
  for (ActiveEntriesMap::iterator it = m_activeEntries.begin();
       it != m_activeEntries.end(); ++it) { 
    if (it->second == entry) {
      m_activeEntries.remove(it);
      delete entry;
      break;
    }
  }
}


//-----------------------------------------------------------------------------

HttpCache::ActiveEntry::ActiveEntry(Entry* e)
    : disk_entry(e),
      doomed(false) {
}

HttpCache::ActiveEntry::~ActiveEntry() {
  if (disk_entry)
    disk_entry->Close();
}

//-----------------------------------------------------------------------------

//find entry from m_activeEntries accoding to key
HttpCache::ActiveEntry* HttpCache::findActiveEntry(const WTF::String& key)
{
    //printf_debug();
    ActiveEntriesMap::iterator it = m_activeEntries.find(key);
    if (it != m_activeEntries.end())
        return it->second;
    
    return NULL;
}

// We should add ActiveEntry to m_activeEntries for a new disk entry.
HttpCache::ActiveEntry* HttpCache::ActivateEntry(const WTF::String& key, Entry* entry)
{
    onActivateEntry();
    assert(entry);
    ActiveEntry* activeEntry = new ActiveEntry(entry);
    m_activeEntries.set(key, activeEntry);

    return activeEntry; 
}

void HttpCache::onActivateEntry()
{
    // md_debug("onActivateEntry\n");
    if (m_activeEntries.size() < 5)
        return;
    int targetSize = 2 * m_activeEntries.size() / 3;

    int size = 0;
    while (!m_activeEntries.isEmpty()) {
        ActiveEntry* entry = m_activeEntries.begin()->second;
        deactivateEntry(entry);
        ++size;
        if (size >= targetSize)
            break;
    }
}

// Generate a key that can be used inside the cache.
const WTF::String HttpCache::generateCacheKey(const WTF::String& url)
{
    // It seems the key is the url only in chromium. 
    // We may expand it future.
    return url;
}

// Open an entry.
HttpCache::ActiveEntry* HttpCache::findDiskEntry(const WTF::String& key)
{
    Entry* disk_entry;
    if (!backend()->OpenEntry(key, &disk_entry))
        return NULL;

    return ActivateEntry(key, disk_entry);
}

bool HttpCache::writeEntry(Entry* entry, WebCore::CachedResource* resource)
{
    if(!entry)
        return false;

    //get HTTP header
    resource->setHttpHeader();
    const WTF::String header = resource->httpHeader();
    
    char* data = (char*)malloc(header.length());
    for (unsigned int i = 0; i < header.length(); ++i )
        data[i] = header[i];

    //const char* buf = header.utf8().data();
    //entry->WriteData(mResponseInfoIndex, 0, header.utf8().data(), header.length(), NULL, true);   
    int rv;
    if ((rv = entry->WriteData(mResponseInfoIndex, 0, data, header.length(), NULL, true)) < 0)
    {
        DISKCACHE_DEBUG_OUTPUT("WriteData header error");
        free(data);
        return false;
    }

    free(data);
   
    //get HTTP body
    WebCore::SharedBuffer* body = resource->data();
    if (!body) {
        DISKCACHE_DEBUG_OUTPUT("body is null");
        return false;
    }

    if ( (rv = entry->WriteData(mResponseContentIndex, 0, body->data(), body->size(), NULL, true) ) < 0 )
    {
        DISKCACHE_DEBUG_OUTPUT("WriteData body error");
        return false;
    }

    //entry->Close();
    DISKCACHE_DEBUG_OUTPUT("write entry end for %s", resource->url().utf8().data());
    return true;

}

#ifdef EXPOSE_COMPILE_WARNING
static void dumpString(const WTF::String& str)
{
#ifdef EXPOSE_COMPILE_WARNING
    for (int i = 0; i < str.utf8().length(); ++i )
#else
    for (unsigned int i = 0; i < str.utf8().length(); ++i )
#endif
        DISKCACHE_DEBUG_OUTPUT("%c", str[i]);
    DISKCACHE_DEBUG_OUTPUT("\n");
}
#endif

// Parse Entry to CachedResource
// We need 
bool HttpCache::parseEntryToCachedResource(const String& responseURL,Entry* disk_entry, WebCore::CachedResource* resource)
{
    int size = disk_entry->GetDataSize(mResponseInfoIndex);

    char* buffer = (char*)malloc(size);
    int rv = disk_entry->ReadData(mResponseInfoIndex, 0, buffer, size, NULL);

    if (rv != size) {
        DISKCACHE_DEBUG_OUTPUT("readData of response header failed\n");
        free(buffer);
        return false;
    }

    //set m_response in resource
    WTF::String header(buffer, size);

    free(buffer);

    WebCore::setResponseFromDiskCache(responseURL, header, resource);

    size = disk_entry->GetDataSize(mResponseContentIndex);

    buffer = (char*)malloc(size);

    rv = disk_entry->ReadData(mResponseContentIndex, 0, buffer, size, NULL);
    if (rv!=size)
    {
        DISKCACHE_DEBUG_OUTPUT("readData of response content failed");
        free(buffer);
        return false;
    }
    //设置resource的data
    RefPtr<WebCore::SharedBuffer> body = WebCore::SharedBuffer::create(buffer, size);

    WebCore::memoryCache()->resourceAccessed(resource);
    //resource->increaseAccessCount();

    resource->data(body, true);

    free(buffer);

    DISKCACHE_DEBUG_OUTPUT("parseEntryToCachedResource end");
    return true;
}


//
// Search the cached resource according to url.
// Return false if not found
//
bool HttpCache::cachedResource(const WTF::String& url, WebCore::CachedResource* resource)
{
    //generate key
    //key is the url for normal
    const WTF::String key = generateCacheKey(url);

    //search entry
    //Firstly, search from m_activeEntries
    ActiveEntry* entry = findActiveEntry(key);
    if (!entry)
    {
        DISKCACHE_DEBUG_OUTPUT("key:%s\n",key.utf8().data());
        entry = findDiskEntry(key);
        if (!entry)
        {
            DISKCACHE_DEBUG_OUTPUT("##not in disk##");
            return false;
        }
    }

    if (!resource)
        return false;

    return parseEntryToCachedResource(key, entry->disk_entry, resource);
}

//
// Add cached resource to disk.
//
bool HttpCache::addCachedResource(WebCore::CachedResource* resource)
{
    const WTF::String key = generateCacheKey(resource->url());

    DISKCACHE_DEBUG_OUTPUT("##url:%s##\n", key.utf8().data());

    //call backend to create an entry
    Entry* disk_entry = NULL;
    if (!backend()->CreateEntry(key, &disk_entry))
        return false;

    if (!writeEntry(disk_entry, resource))
    {
        DISKCACHE_DEBUG_OUTPUT("WriteEntry failed\n");
        backend()->DoomEntry(key);
        return false;
    }

    //add ActiveEntry to m_activeEntries
    if (!ActivateEntry(key, disk_entry))
    {
        DISKCACHE_DEBUG_OUTPUT("Activate Entry error");
    }

    return true;
}

bool HttpCache::updateCachedResource(WebCore::CachedResource* resource) 
{
    if(!resource)
        return false;

    Entry* disk_entry = NULL;
    const WTF::String key = generateCacheKey(resource->url());

    ActiveEntry* active_entry = findActiveEntry(key);
    if (!active_entry)
    {
        DISKCACHE_DEBUG_OUTPUT("key:%s\n", key.utf8().data());
        active_entry = findDiskEntry(key);
        if (!active_entry)
        {
            DISKCACHE_DEBUG_OUTPUT("not find in disk");
            return false;

        } else {
            disk_entry = active_entry->disk_entry;
        }

    } else {
        disk_entry = active_entry->disk_entry;
    }

    if (!writeEntry(disk_entry, resource))
    {
        DISKCACHE_DEBUG_OUTPUT("WriteEntry failed");
        backend()->DoomEntry(key);
        return false;
    }

    DISKCACHE_DEBUG_OUTPUT("OK");
    return true;
}

bool HttpCache::removeCachedResource(WebCore::CachedResource* resource)
{
    if(!resource) {
        DISKCACHE_DEBUG_OUTPUT("null resouce\n");
        return false;
    }

    //generate key
    const WTF::String key = generateCacheKey(resource->url());
    ActiveEntriesMap::iterator it = m_activeEntries.find(key);
    if (it == m_activeEntries.end()) {
        if(backend())
            backend()->DoomEntry(key);
    } else {

        ASSERT(it->second->disk_entry);

        DISKCACHE_DEBUG_OUTPUT("In active");
        if(it->second && it->second->disk_entry) {
            it->second->disk_entry->Doom();
            it->second->doomed = true;
        }

        // We keep track of doomed entries so that we can ensure that they are
        // cleaned up properly when the cache is destroyed.
        m_doomedEntries.add(it->second);

        m_activeEntries.remove(it);
    }

    return true;
}

bool HttpCache::setCacheCapacity(unsigned size)
{
    return backend()->SetMaxSize(size);
}

bool HttpCache::cacheCapacity(unsigned* size)
{
    return backend()->GetMaxSize((int *)size);
}

bool HttpCache::clearCache()
{
    while (!m_activeEntries.isEmpty()) {
        ActiveEntry* entry = m_activeEntries.begin()->second;
        deactivateEntry(entry);
    }    
    return backend()->DoomAllEntries();
}

void HttpCache::getUsedCapacity(unsigned int* size)
{
    if (size)
        *size = backend()->GetUsedSize();
}

}
