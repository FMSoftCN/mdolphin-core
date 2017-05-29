#include "config.h"
#include "DiskCacheBackendTest.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "TestAssert.h"

using namespace disk_cache;
using namespace net;
using namespace base;
using namespace WTF;

#define ENABLE_TEST 1
#define TEST_MAX_DATA_SIZE      (80 * 1024)
#define TEST_MAX_STREAM_COUNT   3
#define TEST_MAX_KEY_SIZE       (2 * 1024)

namespace UnitTest {

DiskCacheBackendTest::DiskCacheBackendTest()
    : TestBase()
    , m_pBackEnd(NULL)
    , m_nCapability(DISKCACHE_CAPABILITY)
    , m_eCacheType(net::DISK_CACHE)
{
    strcpy(m_szPath, DISKCACHE_BACKEND_PATH);
}

DiskCacheBackendTest::~DiskCacheBackendTest()
{
    memset(m_szPath, 0, sizeof(m_szPath));
    m_nCapability = NULL;
    m_eCacheType  = DISK_CACHE;
    m_pBackEnd = NULL;

}
//测试初始化
bool DiskCacheBackendTest::testInit(void)
{
    DEBUG_OUTPUT("Enter");
    this->initDiskCache();
    DEBUG_OUTPUT("Leave");
    return true;
}

//测试退出
void DiskCacheBackendTest::testExit(void)
{
    DEBUG_OUTPUT("Enter");
    this->exitDiskCache();
    DEBUG_OUTPUT("Leave");
}

//测试执行
bool DiskCacheBackendTest::testExec(void)
{
    DEBUG_OUTPUT("Enter");
    this->execDiskCache(); 
    DEBUG_OUTPUT("Leave");
    return true;
}

bool DiskCacheBackendTest::initDiskCache(void)
{
    DEBUG_OUTPUT("m_szPath:%s", m_szPath);
    if (m_szPath[0] == '\0')
        return false;

    if (access(m_szPath, F_OK))
        mkdir(m_szPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH);

    m_pBackEnd = disk_cache::CreateCacheBackend(m_szPath, true, m_nCapability, net::DISK_CACHE);
    if(!m_pBackEnd)
        return false;
    DEBUG_OUTPUT("m_pBackEnd:%p", m_pBackEnd);
    return true;
}

void DiskCacheBackendTest::exitDiskCache(void)
{

    if (m_pBackEnd) {
        delete m_pBackEnd;
        m_pBackEnd = NULL;
    }
}

bool DiskCacheBackendTest::execDiskCache(void)
{
    bool ok = false;
    ok = this->testCreateEntry();
    ok = this->testOpenEntry();
    ok = this->testReadAndWriteEntry();
    ok = this->testGetKey();
    ok = this->testDoomEntry();

    return ok;
}

///////////////////////////////////////////////
bool DiskCacheBackendTest::testCreateEntry(void)
{
    disk_cache::Entry *entry_first = NULL, *entry_second = NULL;

    bool isok = m_pBackEnd->CreateEntry("first", &entry_first);
    if (!isok) {
        DEBUG_OUTPUT("Backend::CreateEntry() failed!");
        return false;
    }
    TEST_ASSERT_OK(isok, "BackEnd", "CreateEntry", "First CreateEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "CreateEntry", "First CreateEntry Fail");
    
    isok = m_pBackEnd->CreateEntry("second", &entry_second);
    TEST_ASSERT_OK(isok, "BackEnd", "CreateEntry", "Second CreateEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "CreateEntry", "Second CreateEntry Fail");

    if (!isok) {
        DEBUG_OUTPUT("Backend::CreateEntry() failed!");
        return false;
    }

    return true;    
}

bool DiskCacheBackendTest::testOpenEntry(void)
{
    disk_cache::Entry *entry_first = NULL, *entry_second = NULL;

    bool isok = m_pBackEnd->OpenEntry("first", &entry_first);
    TEST_ASSERT_OK(isok, "BackEnd", "OpenEntry", "First OpenEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "OpenEntry", "First OpenEntry Fail");
    if (!isok) {
        DEBUG_OUTPUT("Entry::OpenEntry() failed!\n");
        return false;
    }

    isok = m_pBackEnd->OpenEntry("second", &entry_second);
    TEST_ASSERT_OK(isok, "BackEnd", "OpenEntry", "Second OpenEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "OpenEntry", "Second OpenEntry Fail");
    if (!isok) {
        DEBUG_OUTPUT("Entry::OpenEntry() failed!\n");
        return false;
    }

    entry_first->Close();
    entry_second->Close();
    return true;
}

bool DiskCacheBackendTest::testDoomEntry(void)
{
    bool isok = m_pBackEnd->DoomEntry("first");
    TEST_ASSERT_OK(isok, "BackEnd", "DoomEntry", "First  DoomEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "DoomEntry", "First DoomEntry Fail");
    if (!isok) {
        DEBUG_OUTPUT("Entry::DoomEntry() first failed!\n");
        return false;
    }

    isok = m_pBackEnd->DoomEntry("second");
    TEST_ASSERT_OK(isok, "BackEnd", "DoomEntry", "Second  DoomEntry OK");
    TEST_ASSERT_FAIL(isok, "BackEnd", "DoomEntry", "Second DoomEntry Fail");
    if (!isok) {
        DEBUG_OUTPUT("Entry::DoomEntry() second failed!\n");
        return false;
    }

    return true;
}

void DiskCacheBackendTest::printData(const char* buf, int size, const char* header, const char* tail)
{
    if(!buf || size <= 0)
        return;

    if(header)
        printf("%s\n", header);
    else
        printf("start\n");

    for(int j = 0; j < size; j++) {
        if( j != 0 && j % 20 == 0 )
            printf("\n");
        printf(" %02x", (unsigned char)buf[j] );
    }

    if(tail)
        printf("\n%s\n", tail);
    else 
        printf("\nend\n");
}


bool DiskCacheBackendTest::generateData(char *buffer, int maxSize)
{
    if(!buffer || maxSize <= 0)
        return false;

    static bool flag = false;
    if(!flag) {
        flag = true;
        srand(time(NULL));
    }

    for (int j = 0; j < maxSize; j++) {
        buffer[j] = (unsigned char)(rand() % 255);
    }

    if (!buffer[0])
        buffer[0] = 'g';

    return true;
}

bool DiskCacheBackendTest::testReadAndWriteEntry()
{
    int i;
    char *data_read, *data_write = NULL;
    disk_cache::Entry *entry_first = NULL;
    data_read = new char[TEST_MAX_DATA_SIZE];
    data_write = new char[TEST_MAX_DATA_SIZE];
    if( !data_read || !data_write )
        return false;

    if(!m_pBackEnd->OpenEntry("first", &entry_first))
        return false;
    
    for (i = 0; i < TEST_MAX_STREAM_COUNT; i++) {
        memset(data_read, 0,TEST_MAX_DATA_SIZE);
        memset(data_write, 0,TEST_MAX_DATA_SIZE);

        if(!generateData(data_write, TEST_MAX_DATA_SIZE))
            break;

        printData(data_write, 100, "data_write", "data_write()");        

        if (entry_first) {
            int eq = -1;
            int writesize = entry_first->WriteData(i, 0, data_write, TEST_MAX_DATA_SIZE, NULL, false);    
            TEST_ASSERT_OK(writesize == TEST_MAX_DATA_SIZE, "Entry", "WriteData", "Write First Entry OK" );
            TEST_ASSERT_FAIL(writesize == TEST_MAX_DATA_SIZE, "Entry", "WriteData", "Write First Entry Faied" );
            if(writesize != TEST_MAX_DATA_SIZE)
                break;

            int readsize = entry_first->ReadData(i, 0, data_read, TEST_MAX_DATA_SIZE, NULL);        
            TEST_ASSERT_OK(readsize == TEST_MAX_DATA_SIZE, "Entry", "ReadData", "Read First Entry OK" );
            TEST_ASSERT_FAIL(readsize == TEST_MAX_DATA_SIZE, "Entry", "ReadData", "Read First Entry Faied" );
            if (readsize != TEST_MAX_DATA_SIZE)
                break;

            printData(data_read, 100, "read_data", "read_data()");

            eq = memcmp(data_write, data_read, TEST_MAX_DATA_SIZE);
            TEST_ASSERT_OK(!eq, "Entry", "ReadData", "Read And Write Is Same" );
            TEST_ASSERT_FAIL(!eq, "Entry", "ReadData", "Read And Write Is Difference" );
            if(eq) {
                DEBUG_OUTPUT("i:%dERRRORRRRRRRRRRRRRRRRRRRRRRRRRRr\n", i);
                break;
            }

        }
    }
    entry_first->Close();

    if(data_read)
        delete [] data_read;
    if(data_write)
        delete [] data_write;
}

bool DiskCacheBackendTest::testGetKey()
{
    String key_in("first");
    String key_out;
    bool result = false;
    char* write_data = NULL;
    char* read_data = NULL;
    disk_cache::Entry *entry_first = NULL, *entry_second = NULL;

    if(!m_pBackEnd->OpenEntry(key_in, &entry_first))
        return false;
    key_out = entry_first->GetKey();
    TEST_ASSERT_OK(key_out == key_in, "Entry", "GetKey", "GetKey first OK" );
    TEST_ASSERT_FAIL(key_out == key_in, "Entry", "GetKey", "GetKey first Failed");
    entry_first->Close();

    write_data = new char[TEST_MAX_KEY_SIZE + 1];
    if(!write_data)
        return false;
    memset(write_data, 0, TEST_MAX_KEY_SIZE + 1);

    if(!generateData(write_data, TEST_MAX_KEY_SIZE))
        return false;

    printData(write_data, 100, "longkeydata", "longkeydata()");

    String key_in2(write_data);
    DEBUG_OUTPUT("key in len:%u", key_in2.length()); 
    bool ok = m_pBackEnd->CreateEntry(key_in2, &entry_second);
    TEST_ASSERT_OK(ok, "Entry", "CreateEntry", "Create Long Key Entry OK" );
    TEST_ASSERT_FAIL(ok, "Entry", "CreateEntry", "Create Long Key Entry Failed" );

    TEST_ASSERT_OK(entry_second, "Entry", "CreateEntry", "Create Long Key Entry is not NULL" );
    TEST_ASSERT_FAIL(entry_second, "Entry", "CreateEntry", "Create Long Key Entry is NULL" );
    
    String key_out2 = entry_second->GetKey();
    DEBUG_OUTPUT("key out len:%u", key_out2.length()); 
    TEST_ASSERT_OK(key_out2 == key_in2, "Entry", "GetKey", "GetKey Long Key OK" );
    TEST_ASSERT_FAIL(key_out2 == key_in2, "Entry", "GetKey", "GetKey Long Key Failed");

    if(entry_second)
        entry_second->Close();

    if(write_data) {
        delete [] write_data;
        write_data = NULL;
    }
    
    return true;
}
    
}
