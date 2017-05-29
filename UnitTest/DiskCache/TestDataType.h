#ifndef TestDataType_h
#define TestDataType_h
#include <stdio.h>
#include <math.h>

namespace UnitTest {

#define FILE_AND_LINE() __FILE__, __LINE__

#define DISKCACHE_PATH_MAX      (256)
#define DISKCACHE_CAPABILITY    (1024 * 1024L)

#define DEBUG_OUTPUT(fmt,...) printf("File:%s, Line:%d, Function:%s, "#fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

typedef struct _TestResult {
    char class_type[32];
    char fun_name[128];
    char real_result[256];
    char expect_result[256];
    char file_name[256];
    char file_line[20];
    char msg[256];
}TestResult;

typedef int (*PFN_CLIENT_CALLBACK_T)(TestResult *pTestResult);

}

#endif
