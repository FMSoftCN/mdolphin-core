#ifndef _http_cache_h_
#define _http_cache_h_

#include "cache_type.h"
#include "file_path.h"
#include "disk_cache.h"
#include "PlatformString.h"
#include "CachedResource.h"

#include <wtf/HashMap.h>
#include <wtf/HashSet.h>

namespace disk_cache{

class Backend;
class Entry;


class HttpCache{

public:
    HttpCache(unsigned capacity, const char* cacheDir);
    ~HttpCache();

    // search WebCore::CachedResource
    bool cachedResource(const WTF::String& url, WebCore::CachedResource* resource);

    // add WebCore::CachedResource
    bool addCachedResource(WebCore::CachedResource* resource);

    // remove WebCore::CachedResource
    bool removeCachedResource(WebCore::CachedResource* resource);

    bool updateCachedResource(WebCore::CachedResource* resource);

    bool setCacheCapacity(unsigned);

    bool cacheCapacity(unsigned* size);

    bool clearCache(void);

    void getUsedCapacity(unsigned int* size);

private:
    struct ActiveEntry {
        Entry* disk_entry;
        bool doomed;

        explicit ActiveEntry(Entry*);
        ~ActiveEntry();
    }; 

    typedef HashMap<WTF::String, ActiveEntry*> ActiveEntriesMap;
    typedef HashSet<ActiveEntry*> ActiveEntriesSet;
    //m_activeEntries is used to store the entry we have already opened.
    //ActiveEntry is the pointer to disk cache entry.
    ActiveEntriesMap m_activeEntries;

    //m_doomedEntries is used to store the doomed entries.
    //The entry in the doomedEntries will be deleted from disk in the Destructor of HttpCache
     ActiveEntriesSet m_doomedEntries;
    void onActivateEntry();
    void deactivateEntry(ActiveEntry* entry);
    void slowDeactivateEntry(ActiveEntry* entry);

    ActiveEntry* findActiveEntry(const WTF::String& key);
    ActiveEntry* ActivateEntry(const WTF::String& key, Entry* entry);

    const WTF::String generateCacheKey(const WTF::String& url);

    ActiveEntry* findDiskEntry(const WTF::String& key);
    bool writeEntry(Entry* entry, WebCore::CachedResource* resource);


    //to avoid diskcache url combination incorrect, so we add responseURL to make sure url is right.
    bool parseEntryToCachedResource(const String& responseURL, Entry* entry, WebCore::CachedResource* resource);

    Backend* backend(){return m_backend;}

    const char* m_path;
    int m_maxBytes;
    net::CacheType m_cacheType;
    Backend* m_backend;
};
//#if CACHE_DEBUG
#ifdef EXPOSE_COMPILE_WARNING
    static void dumpString(const WTF::String& str);
#endif
//#endif

}


#endif
