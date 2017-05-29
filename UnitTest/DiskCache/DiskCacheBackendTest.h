#ifndef DiskCacheBackendTest_h
#define DiskCacheBackendTest_h
#include "TestBase.h"
#include "backend_impl.h"

#define DISKCACHE_BACKEND_PATH  "/tmp/diskcache"
//#define DISKCACHE_PATH_MAX      (256)
//#define DISKCACHE_CAPABILITY    (1024 * 1024L)


#ifdef __cplusplus
extern "C"
{
#endif

namespace UnitTest {

class DiskCacheBackendTest : public TestBase {
public:
        
        DiskCacheBackendTest();
        virtual ~DiskCacheBackendTest();

        //测试初始化
        virtual bool testInit(void);
        
        //测试退出
        virtual void testExit(void); 

        //测试执行
        virtual bool testExec(void);

private:
        bool initDiskCache(void);
        void exitDiskCache(void);
        bool execDiskCache(void);

private:
        void printData(const char* buf, int size, const char* header, const char* tail);
        bool generateData(char *buffer, int maxSize);
        bool testCreateEntry(void);
        bool testOpenEntry(void);
        bool testDoomEntry(void);
        bool testReadAndWriteEntry();
        bool testGetKey();

private:
        disk_cache::Backend*    m_pBackEnd;
        char        m_szPath[DISKCACHE_PATH_MAX];
        uint32_t    m_nCapability;
        net::CacheType   m_eCacheType;

};

}

#ifdef __cplusplus
}
#endif
#endif //DiskCacheBackendTest_h
