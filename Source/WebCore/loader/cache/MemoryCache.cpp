/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
    Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "MemoryCache.h"

#include "CachedCSSStyleSheet.h"
#include "CachedFont.h"
#include "CachedImage.h"
#include "CachedScript.h"
#include "CachedXSLStyleSheet.h"
#include "CachedResourceLoader.h"
#include "Document.h"
#include "FrameLoader.h"
#include "FrameLoaderTypes.h"
#include "FrameView.h"
#include "Image.h"
#include "Logging.h"
#include "ResourceHandle.h"
#include "SecurityOrigin.h"
#include "SecurityOriginHash.h"
#include <stdio.h>
#include <wtf/CurrentTime.h>
#include <wtf/text/CString.h>
#include "CachedResourceLoader.h"
#include "CachedResource.h"


using namespace std;

#ifndef NDEBUG
#define MEMMACHE_DEBUG_OUTPUT(fmt,...) fprintf(stderr, "File:%s, Line:%d, Function:%s, "#fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define MEMMACHE_DEBUG_OUTPUT(fmt,...)
#endif

namespace WebCore {

static const int cDefaultCacheCapacity = 8192 * 1024;
static const double cMinDelayBeforeLiveDecodedPrune = 1; // Seconds.
static const float cTargetPrunePercentage = .95f; // Percentage of capacity toward which we prune, to avoid immediately pruning again.
static const double cDefaultDecodedDataDeletionInterval = 0;

MemoryCache* memoryCache()
{
    static MemoryCache staticCache;
    return &staticCache;
}

MemoryCache::MemoryCache()
    : m_disabled(false)
    , m_pruneEnabled(true)
    , m_inPruneDeadResources(false)
    , m_capacity(cDefaultCacheCapacity)
    , m_minDeadCapacity(0)
    , m_maxDeadCapacity(cDefaultCacheCapacity)
    , m_deadDecodedDataDeletionInterval(cDefaultDecodedDataDeletionInterval)
    , m_liveSize(0)
    , m_deadSize(0)
    , m_cacheType(MemCache)
    , m_inEvict(false)
#if ENABLE(DISK_CACHE)
    , m_httpCache(0)
    , m_cacheDir(".cache")
    , m_diskCapacity(0)
#endif
{
}

MemoryCache::~MemoryCache()
{
/*
    for (;;) {
        CachedResourceMap::iterator i = m_resources.begin();
        if (i == m_resources.end())
            break;
        remove(i->second);
    }
    */
    pruneDeadResources();
    pruneLiveResources();
#if ENABLE(DISK_CACHE)
    if (httpCache())
        delete httpCache();
#endif
}

KURL MemoryCache::removeFragmentIdentifierIfNeeded(const KURL& originalURL)
{
    if (!originalURL.hasFragmentIdentifier())
        return originalURL;
    // Strip away fragment identifier from HTTP and file urls.
    // Data urls must be unmodified and it is also safer to keep them for custom protocols.
    if (!(originalURL.protocolInHTTPFamily() || originalURL.isLocalFile()))
        return originalURL;

    KURL url = originalURL;
    url.removeFragmentIdentifier();
    return url;
}

bool MemoryCache::add(CachedResource* resource)
{
    if (disabled())
        return false;
    
    m_resources.set(resource->url(), resource);
    resource->setInCache(true);
    
    resourceAccessed(resource);
    
    LOG(ResourceLoading, "MemoryCache::add Added '%s', resource %p\n", resource->url().latin1().data(), resource);
    return true;
}

void MemoryCache::revalidationSucceeded(CachedResource* revalidatingResource, const ResourceResponse& response)
{
    CachedResource* resource = revalidatingResource->resourceToRevalidate();
    ASSERT(resource);
    ASSERT(!resource->inCache());
    ASSERT(resource->isLoaded());
    ASSERT(revalidatingResource->inCache());
    evict(revalidatingResource);

    ASSERT(!m_resources.get(resource->url()));
    m_resources.set(resource->url(), resource);
    resource->setInCache(true);
    resource->updateResponseAfterRevalidation(response);
    insertInLRUList(resource);
    int delta = resource->size();
    if (resource->decodedSize() && resource->hasClients())
        insertInLiveDecodedResourcesList(resource);
    if (delta)
        adjustSize(resource->hasClients(), delta);

#if ENABLE(DISK_CACHE)
    if (memoryCache()->isDiskCache())
    {
        if (!memoryCache()->httpCache()->updateCachedResource(resource))
        {
            //setInCache(false);
        }
    }
#endif
    
    revalidatingResource->switchClientsToRevalidatedResource();
    // this deletes the revalidating resource
    revalidatingResource->clearResourceToRevalidate();
}

void MemoryCache::revalidationFailed(CachedResource* revalidatingResource)
{
    LOG(ResourceLoading, "Revalidation failed for %p", revalidatingResource);
    ASSERT(revalidatingResource->resourceToRevalidate());
    revalidatingResource->clearResourceToRevalidate();
}

CachedResource* MemoryCache::resourceForURL(const KURL& resourceURL,CachedResource::Type type,const String& charset)
{
    if (disabled())
        return 0;
        
    KURL url = removeFragmentIdentifierIfNeeded(resourceURL);

    CachedResource* resource = m_resources.get(url);
    bool wasPurgeable = MemoryCache::shouldMakeResourcePurgeableOnEviction() && resource && resource->isPurgeable();
    if (resource && !resource->makePurgeable(false)) {
        ASSERT(!resource->hasClients());
        evict(resource);
        return 0;
    }
    // Add the size back since we had subtracted it when we marked the memory as purgeable.
    if (wasPurgeable)
        adjustSize(resource->hasClients(), resource->size());
    
    if(resource) {
        return resource;
    }
 
#if ENABLE(DISK_CACHE)
    if (isDiskCache())
    {
        resource = createResource(type,  url, charset);

        if (!resource)
            return NULL;
        
        resource->setLoading(true);
        if (!httpCache()->cachedResource(url, resource)) 
        {
            resource->setInCache(false);
            resource->setLoading(false);
            delete resource;
            resource = NULL;
        } 
        else if (resource->errorOccurred()){
            // if error occurred, cache()->remove(,true) will be called.
            resource->setLoading(false);
            return 0;
        } 
        else 
        {
            resource->setInCache(true);

            m_resources.set(url, resource);  
            //resourceAccessed(resource);
            // we need do sth to fullfil a CachedResource
            resource->setStatus(CachedResource::Cached);
        }
    }
#endif
    return resource;
}

unsigned MemoryCache::deadCapacity() const 
{
    // Dead resource capacity is whatever space is not occupied by live resources, bounded by an independent minimum and maximum.
    unsigned capacity = m_capacity - min(m_liveSize, m_capacity); // Start with available capacity.
    capacity = max(capacity, m_minDeadCapacity); // Make sure it's above the minimum.
    capacity = min(capacity, m_maxDeadCapacity); // Make sure it's below the maximum.
    return capacity;
}

unsigned MemoryCache::liveCapacity() const 
{ 
    // Live resource capacity is whatever is left over after calculating dead resource capacity.
    return m_capacity - deadCapacity();
}

void MemoryCache::pruneLiveResources()
{
    if (!m_pruneEnabled)
        return;

    unsigned capacity = liveCapacity();
    if (capacity && m_liveSize <= capacity)
        return;

    unsigned targetSize = static_cast<unsigned>(capacity * cTargetPrunePercentage); // Cut by a percentage to avoid immediately pruning again.
    double currentTime = FrameView::currentPaintTimeStamp();
    if (!currentTime) // In case prune is called directly, outside of a Frame paint.
        currentTime = WTF::currentTime();
    
    // Destroy any decoded data in live objects that we can.
    // Start from the tail, since this is the least recently accessed of the objects.

    // The list might not be sorted by the m_lastDecodedAccessTime. The impact
    // of this weaker invariant is minor as the below if statement to check the
    // elapsedTime will evaluate to false as the currentTime will be a lot
    // greater than the current->m_lastDecodedAccessTime.
    // For more details see: https://bugs.webkit.org/show_bug.cgi?id=30209
    CachedResource* current = m_liveDecodedResources.m_tail;
    while (current) {
        CachedResource* prev = current->m_prevInLiveResourcesList;
        ASSERT(current->hasClients());
        if(current != prev) {
            if (current->isLoaded() && current->decodedSize()) {
                // Check to see if the remaining resources are too new to prune.
                double elapsedTime = currentTime - current->m_lastDecodedAccessTime;
                if (elapsedTime < cMinDelayBeforeLiveDecodedPrune)
                    return;

                // Destroy our decoded data. This will remove us from 
                // m_liveDecodedResources, and possibly move us to a different LRU 
                // list in m_allResources.
                current->destroyDecodedData();

                if (targetSize && m_liveSize <= targetSize)
                    return;
            }
        } else {
            break;
        }
        current = prev;
    }
}

/*
 * @brief 清除一类资源
 * @param int index, 索引
 * @param unsigned int targetSize
 */
bool MemoryCache::pruneOneDeadResource(int index, unsigned int targetSize)
{

    CachedResource* prev = NULL;
    if(index >= 0) {
        CachedResource* current = m_allResources[index].m_tail;

        // First flush all the decoded data in this queue.
        while (current) {
            prev = current->m_prevInAllResourcesList;
            MEMMACHE_DEBUG_OUTPUT("index:%d, ##cur:%p,##pre:%p\n", index, current, prev );

            if(current != prev) { 

                if (current->wasPurged()) {
                    evict(current);
                }

                if (!current->hasClients() && !current->isPreloaded() && current->isLoaded()) {
                    // Destroy our decoded data. This will remove us from 
                    // m_liveDecodedResources, and possibly move us to a different 
                    // LRU list in m_allResources.

                    MEMMACHE_DEBUG_OUTPUT("##index:%d, cur:%p##\n", index, current );
                    current->destroyDecodedData();

                    if (targetSize && m_deadSize <= targetSize) {
                        MEMMACHE_DEBUG_OUTPUT("index:%d, m_deadSize <= targetSize\n", index );
                        return false;
                    }
                }

#if 1
                if (!current->hasClients() 
                        && !current->isPreloaded() 
                        && !current->isCacheValidator()) {

                    MEMMACHE_DEBUG_OUTPUT("current:%p\n", current);
                    if (!makeResourcePurgeable(current))
                        evict(current);

                    // If evict() caused pruneDeadResources() to be re-entered, bail out. This can happen when removing an
                    // SVG CachedImage that has subresources.
                    //if (!m_inPruneDeadResources)
                    //    return false;

                    if (targetSize && m_deadSize <= targetSize) {
                        return false;
                    }
                }
#endif
            } else {
                break;
            }

            current = prev;
        }
        
        return true;
    }
    return false;
}

void MemoryCache::pruneDeadResources()
{
    if (!m_pruneEnabled)
        return;

    unsigned capacity = deadCapacity();
    if (capacity && m_deadSize <= capacity) {
        MEMMACHE_DEBUG_OUTPUT("m_deadSize <= capacity");
        return;
    }

    // Cut by a percentage to avoid immediately pruning again.
    unsigned targetSize = static_cast<unsigned>(capacity * cTargetPrunePercentage); 
    int size = m_allResources.size();
    if(!m_inPruneDeadResources) {
        m_inPruneDeadResources = true;

        // Remove from the tail, since this is the least frequently accessed of the objects.
        for (int i = size - 1; i >= 0; i--) {
            if(!pruneOneDeadResource(i, targetSize))
                break;

#if 0      //this can cause core down so don't use it

            // Shrink the vector back down so we don't waste time inspecting
            // empty LRU lists on future prunes.
            if (m_allResources[i].m_head)
                canShrinkLRULists = false;
            else if (canShrinkLRULists)
                m_allResources.resize(i);
#endif 
        }
    }

    m_inPruneDeadResources = false;
}

void MemoryCache::setCapacities(unsigned minDeadBytes, unsigned maxDeadBytes, unsigned totalBytes)
{
#if ENABLE(DISK_CACHE)
    if (isDiskCache())
    {
        m_diskCapacity = totalBytes;
        //remove? no. it should be done in disk cache internel.
        //httpCache()->setCacheCapacity(m_diskCapacity);
    } 
    else 
#endif
    {
        ASSERT(minDeadBytes <= maxDeadBytes);
        ASSERT(maxDeadBytes <= totalBytes);
        m_minDeadCapacity = minDeadBytes;
        m_maxDeadCapacity = maxDeadBytes;
        m_capacity = totalBytes;
        prune();
    }
}

bool MemoryCache::makeResourcePurgeable(CachedResource* resource)
{
    if( NULL == resource)
        return false;

    if (!MemoryCache::shouldMakeResourcePurgeableOnEviction())
        return false;

    if (!resource->inCache())
        return false;

    if (resource->isPurgeable())
        return true;

    if (!resource->isSafeToMakePurgeable())
        return false;

    if (!resource->makePurgeable(true))
        return false;

    adjustSize(resource->hasClients(), -static_cast<int>(resource->size()));

    return true;
}

#if PLATFORM(MG)
void MemoryCache::getCapacities(unsigned* minDeadBytes, unsigned* maxDeadBytes, unsigned* totalBytes)
{
#if ENABLE(DISK_CACHE)
    if (isDiskCache())
    {
        if (totalBytes)
           *totalBytes = m_diskCapacity; 
        //httpCache()->cacheCapacity(totalBytes);
    } else 
#endif
    {
        if (minDeadBytes)
            *minDeadBytes = m_minDeadCapacity;
        if (maxDeadBytes)
            *maxDeadBytes = m_maxDeadCapacity;
        if (totalBytes)
            *totalBytes = m_maxDeadCapacity;
    }
}
bool MemoryCache::setCacheType(CacheType type, const char* path, unsigned diskCapacity)
{
    m_cacheType = type;

#if ENABLE(DISK_CACHE)
    if (m_cacheType == DiskCache)
    {
        m_cacheDir = path;
        if (diskCapacity == 0)
            m_diskCapacity = 50 * m_capacity;
        else 
        {
            m_diskCapacity = diskCapacity;
        }

        //set m_httpCache
        if (!m_httpCache)
        {
            m_httpCache =new disk_cache::HttpCache(m_diskCapacity, m_cacheDir); 
        }
    }
#endif

    return true;
}
#endif

void MemoryCache::evict(CachedResource* resource)
{

    if(resource && !m_inEvict) {
        m_inEvict = true;
        LOG(ResourceLoading, "Evicting resource %p for '%s' from cache", resource, resource->url().latin1().data());
        // The resource may have already been removed by someone other than our caller,
        // who needed a fresh copy for a reload. See <http://bugs.webkit.org/show_bug.cgi?id=12479#c6>.
        if (resource->inCache()) {
            // Remove from the resource map.
            m_resources.remove(resource->url());
            resource->setInCache(false);

            // Remove from the appropriate LRU list.
            removeFromLRUList(resource);
            removeFromLiveDecodedResourcesList(resource);

            // If the resource was purged, it means we had already decremented the size when we made the
            // resource purgeable in makeResourcePurgeable(). So adjust the size if we are evicting a
            // resource that was not marked as purgeable.
            if (!MemoryCache::shouldMakeResourcePurgeableOnEviction() || !resource->isPurgeable())
                adjustSize(resource->hasClients(), -static_cast<int>(resource->size()));

#if 0 //ENABLE(DISK_CACHE) 
            //this codes put here can cause diskcache can not find entry 
            //and generate another disk file in filesystem, so move it next code scope

            if (isDiskCache()) 
            {
                MEMMACHE_DEBUG_OUTPUT("before removeCachedResource");
                httpCache()->removeCachedResource(resource);
                MEMMACHE_DEBUG_OUTPUT("after removeCachedResource");
            }
#endif

        } else
            ASSERT(m_resources.get(resource->url()) != resource);

        if (resource->canDelete()) {

#if ENABLE(DISK_CACHE)
            if (isDiskCache())
            {
                MEMMACHE_DEBUG_OUTPUT("before removeCachedResource");
                httpCache()->removeCachedResource(resource);
                MEMMACHE_DEBUG_OUTPUT("after removeCachedResource");
            }
#endif

            MEMMACHE_DEBUG_OUTPUT("[%s] canDelete\n", __FUNCTION__);
            delete resource;
            resource = NULL;
        }
        
        m_inEvict = false;
    }
}

static inline unsigned fastLog2(unsigned i)
{
    unsigned log2 = 0;
    if (i & (i - 1))
        log2 += 1;
    if (i >> 16)
        log2 += 16, i >>= 16;
    if (i >> 8)
        log2 += 8, i >>= 8;
    if (i >> 4)
        log2 += 4, i >>= 4;
    if (i >> 2)
        log2 += 2, i >>= 2;
    if (i >> 1)
        log2 += 1;
    return log2;
}

MemoryCache::LRUList* MemoryCache::lruListFor(CachedResource* resource)
{
    unsigned accessCount = max(resource->accessCount(), 1U);
    unsigned queueIndex = fastLog2(resource->size() / accessCount);
//#ifndef NDEBUG
    resource->m_lruIndex = queueIndex;
//#endif
    if (m_allResources.size() <= queueIndex)
        m_allResources.grow(queueIndex + 1);
    return &m_allResources[queueIndex];
}

void MemoryCache::removeFromLRUList(CachedResource* resource)
{
    if(resource) {
        LRUList* oldList = NULL;
        // If we've never been accessed, then we're brand new and not in any list.
        if (resource->accessCount() == 0)
            return;

//#if !ASSERT_DISABLED
        unsigned oldListIndex = resource->m_lruIndex;
//#endif

        LRUList* list = lruListFor(resource);

#if !ASSERT_DISABLED
        // Verify that the list we got is the list we want.
        //ASSERT(resource->m_lruIndex == oldListIndex);

        // Verify that we are in fact in this list.
        bool found = false;
        for (CachedResource* current = list->m_head; current; current = current->m_nextInAllResourcesList) {
            if (current == resource) {
                found = true;
                break;
            }
        }
        //ASSERT(found);
#endif

        CachedResource* next = resource->m_nextInAllResourcesList;
        CachedResource* prev = resource->m_prevInAllResourcesList;

        if ( oldListIndex != resource->m_lruIndex && oldListIndex < m_allResources.size()) {
            oldList = &m_allResources[oldListIndex];
            if (oldList) {

                if (next == 0 && prev == 0 && oldList->m_head != resource)
                    return;

                MEMMACHE_DEBUG_OUTPUT("##oldListIndex != resource->m_lruIndex##");

                if (oldList->m_tail == resource)
                    oldList->m_tail = prev;

                if (oldList->m_head == resource)
                    oldList->m_head = next;
            }

        }

        if (next == 0 && prev == 0 && list->m_head != resource)
            return;


        if (next)
            next->m_prevInAllResourcesList = prev;
        else if (list->m_tail == resource)
            list->m_tail = prev;

        if (prev)
            prev->m_nextInAllResourcesList = next;
        else if (list->m_head == resource)
            list->m_head = next;

        resource->m_nextInAllResourcesList = 0;
        resource->m_prevInAllResourcesList = 0;

    }
}

void MemoryCache::insertInLRUList(CachedResource* resource)
{
    if(resource) {

        // Make sure we aren't in some list already.
        ASSERT(!resource->m_nextInAllResourcesList && !resource->m_prevInAllResourcesList);
        //ASSERT(resource->inCache());
        //ASSERT(resource->accessCount() > 0);

        LRUList* list = lruListFor(resource);

        resource->m_prevInAllResourcesList = NULL;
        resource->m_nextInAllResourcesList = NULL;

        resource->m_nextInAllResourcesList = list->m_head;
        if (list->m_head)
            list->m_head->m_prevInAllResourcesList = resource;
        list->m_head = resource;

        if (!resource->m_nextInAllResourcesList)
            list->m_tail = resource;

#ifndef NDEBUG
        // Verify that we are in now in the list like we should be.
        list = lruListFor(resource);
        bool found = false;
        for (CachedResource* current = list->m_head; current; current = current->m_nextInAllResourcesList) {
            if (current == resource) {
                found = true;
                break;
            }
        }
        ASSERT(found);
#endif
    }
}

void MemoryCache::resourceAccessed(CachedResource* resource)
{
    //ASSERT(resource->inCache());
    
    // Need to make sure to remove before we increase the access count, since
    // the queue will possibly change.
    removeFromLRUList(resource);
    
    // If this is the first time the resource has been accessed, adjust the size of the cache to account for its initial size.
    if (!resource->accessCount())
        adjustSize(resource->hasClients(), resource->size());
    
    // Add to our access count.
    resource->increaseAccessCount();
    
    // Now insert into the new queue.
    insertInLRUList(resource);
}

void MemoryCache::removeResourcesWithOrigin(SecurityOrigin* origin)
{
    Vector<CachedResource*> resourcesWithOrigin;

    CachedResourceMap::iterator e = m_resources.end();
    for (CachedResourceMap::iterator it = m_resources.begin(); it != e; ++it) {
        CachedResource* resource = it->second;
        RefPtr<SecurityOrigin> resourceOrigin = SecurityOrigin::createFromString(resource->url());
        if (!resourceOrigin)
            continue;
        if (resourceOrigin->equal(origin))
            resourcesWithOrigin.append(resource);
    }

    for (size_t i = 0; i < resourcesWithOrigin.size(); ++i) {
        //MEMMACHE_DEBUG_OUTPUT("Delete resourcesWithOrigin\n");
        remove(resourcesWithOrigin[i]);

    }
}

void MemoryCache::getOriginsWithCache(SecurityOriginSet& origins)
{
    CachedResourceMap::iterator e = m_resources.end();
    for (CachedResourceMap::iterator it = m_resources.begin(); it != e; ++it)
        origins.add(SecurityOrigin::create(KURL(KURL(), it->second->url())));
}

void MemoryCache::removeFromLiveDecodedResourcesList(CachedResource* resource)
{
    // If we've never been accessed, then we're brand new and not in any list.
    if (!resource->m_inLiveDecodedResourcesList)
        return;
    resource->m_inLiveDecodedResourcesList = false;

#ifndef NDEBUG
    // Verify that we are in fact in this list.
    bool found = false;
    for (CachedResource* current = m_liveDecodedResources.m_head; current; current = current->m_nextInLiveResourcesList) {
        if (current == resource) {
            found = true;
            break;
        }
    }
    ASSERT(found);
#endif

    CachedResource* next = resource->m_nextInLiveResourcesList;
    CachedResource* prev = resource->m_prevInLiveResourcesList;
    
    if (next == 0 && prev == 0 && m_liveDecodedResources.m_head != resource)
        return;
    
    resource->m_nextInLiveResourcesList = 0;
    resource->m_prevInLiveResourcesList = 0;
    
    if (next)
        next->m_prevInLiveResourcesList = prev;
    else if (m_liveDecodedResources.m_tail == resource)
        m_liveDecodedResources.m_tail = prev;

    if (prev)
        prev->m_nextInLiveResourcesList = next;
    else if (m_liveDecodedResources.m_head == resource)
        m_liveDecodedResources.m_head = next;
}

void MemoryCache::insertInLiveDecodedResourcesList(CachedResource* resource)
{
    // Make sure we aren't in the list already.
    ASSERT(!resource->m_nextInLiveResourcesList && !resource->m_prevInLiveResourcesList && !resource->m_inLiveDecodedResourcesList);
    resource->m_inLiveDecodedResourcesList = true;

    resource->m_nextInLiveResourcesList = m_liveDecodedResources.m_head;
    if (m_liveDecodedResources.m_head)
        m_liveDecodedResources.m_head->m_prevInLiveResourcesList = resource;
    m_liveDecodedResources.m_head = resource;
    
    if (!resource->m_nextInLiveResourcesList)
        m_liveDecodedResources.m_tail = resource;
        
#ifndef NDEBUG
    // Verify that we are in now in the list like we should be.
    bool found = false;
    for (CachedResource* current = m_liveDecodedResources.m_head; current; current = current->m_nextInLiveResourcesList) {
        if (current == resource) {
            found = true;
            break;
        }
    }
    ASSERT(found);
#endif

}

void MemoryCache::addToLiveResourcesSize(CachedResource* resource)
{
    m_liveSize += resource->size();
    m_deadSize -= resource->size();
}

void MemoryCache::removeFromLiveResourcesSize(CachedResource* resource)
{
    m_liveSize -= resource->size();
    m_deadSize += resource->size();
}

void MemoryCache::adjustSize(bool live, int delta)
{
    if (live) {
        ASSERT(delta >= 0 || ((int)m_liveSize + delta >= 0));
        m_liveSize += delta;
    } else {
        ASSERT(delta >= 0 || ((int)m_deadSize + delta >= 0));
        m_deadSize += delta;
    }
}

void MemoryCache::TypeStatistic::addResource(CachedResource* o)
{
    bool purged = o->wasPurged();
    bool purgeable = o->isPurgeable() && !purged; 
    int pageSize = (o->encodedSize() + o->overheadSize() + 4095) & ~4095;
    count++;
    size += purged ? 0 : o->size(); 
    liveSize += o->hasClients() ? o->size() : 0;
    decodedSize += o->decodedSize();
    purgeableSize += purgeable ? pageSize : 0;
    purgedSize += purged ? pageSize : 0;
}

MemoryCache::Statistics MemoryCache::getStatistics()
{
    Statistics stats;
    CachedResourceMap::iterator e = m_resources.end();
    for (CachedResourceMap::iterator i = m_resources.begin(); i != e; ++i) {
        CachedResource* resource = i->second;
        switch (resource->type()) {
        case CachedResource::ImageResource:
            stats.images.addResource(resource);
            break;
        case CachedResource::CSSStyleSheet:
            stats.cssStyleSheets.addResource(resource);
            break;
        case CachedResource::Script:
            stats.scripts.addResource(resource);
            break;
#if ENABLE(XSLT)
        case CachedResource::XSLStyleSheet:
            stats.xslStyleSheets.addResource(resource);
            break;
#endif
        case CachedResource::FontResource:
            stats.fonts.addResource(resource);
            break;
        default:
            break;
        }
    }
    return stats;
}

void MemoryCache::setDisabled(bool disabled)
{
    m_disabled = disabled;
    if (!m_disabled)
        return;

#if ENABLE(DISK_CACHE)
    if (isDiskCache())
    {
        // clear mem cache
        for (;;) {
            CachedResourceMap::iterator i = m_resources.begin();
            if (i == m_resources.end())
                break;
            remove(i->second);
        }
        // clear disk cache
        httpCache()->clearCache();
    } else 
#endif
    {
        for (;;) {
            CachedResourceMap::iterator i = m_resources.begin();
            if (i == m_resources.end())
                break;
            evict(i->second);
        }
    }
}

void MemoryCache::evictResources()
{
    if (disabled())
        return;

    setDisabled(true);
    setDisabled(false);
}

#ifndef NDEBUG
void MemoryCache::dumpStats()
{
    Statistics s = getStatistics();
    printf("%-13s %-13s %-13s %-13s %-13s %-13s %-13s\n", "", "Count", "Size", "LiveSize", "DecodedSize", "PurgeableSize", "PurgedSize");
    printf("%-13s %-13s %-13s %-13s %-13s %-13s %-13s\n", "-------------", "-------------", "-------------", "-------------", "-------------", "-------------", "-------------");
    printf("%-13s %13d %13d %13d %13d %13d %13d\n", "Images", s.images.count, s.images.size, s.images.liveSize, s.images.decodedSize, s.images.purgeableSize, s.images.purgedSize);
    printf("%-13s %13d %13d %13d %13d %13d %13d\n", "CSS", s.cssStyleSheets.count, s.cssStyleSheets.size, s.cssStyleSheets.liveSize, s.cssStyleSheets.decodedSize, s.cssStyleSheets.purgeableSize, s.cssStyleSheets.purgedSize);
#if ENABLE(XSLT)
    printf("%-13s %13d %13d %13d %13d %13d %13d\n", "XSL", s.xslStyleSheets.count, s.xslStyleSheets.size, s.xslStyleSheets.liveSize, s.xslStyleSheets.decodedSize, s.xslStyleSheets.purgeableSize, s.xslStyleSheets.purgedSize);
#endif
    printf("%-13s %13d %13d %13d %13d %13d %13d\n", "JavaScript", s.scripts.count, s.scripts.size, s.scripts.liveSize, s.scripts.decodedSize, s.scripts.purgeableSize, s.scripts.purgedSize);
    printf("%-13s %13d %13d %13d %13d %13d %13d\n", "Fonts", s.fonts.count, s.fonts.size, s.fonts.liveSize, s.fonts.decodedSize, s.fonts.purgeableSize, s.fonts.purgedSize);
    printf("%-13s %-13s %-13s %-13s %-13s %-13s %-13s\n\n", "-------------", "-------------", "-------------", "-------------", "-------------", "-------------", "-------------");
}

void MemoryCache::dumpLRULists(bool includeLive) const
{
    printf("LRU-SP lists in eviction order (Kilobytes decoded, Kilobytes encoded, Access count, Referenced, isPurgeable, wasPurged):\n");

    int size = m_allResources.size();
    for (int i = size - 1; i >= 0; i--) {
        printf("\n\nList %d: ", i);
        CachedResource* current = m_allResources[i].m_tail;
        while (current) {
            CachedResource* prev = current->m_prevInAllResourcesList;
            if (includeLive || !current->hasClients())
                printf("(%.1fK, %.1fK, %uA, %dR, %d, %d); ", current->decodedSize() / 1024.0f, (current->encodedSize() + current->overheadSize()) / 1024.0f, current->accessCount(), current->hasClients(), current->isPurgeable(), current->wasPurged());

            current = prev;
        }
    }
}
#endif

} // namespace WebCore
