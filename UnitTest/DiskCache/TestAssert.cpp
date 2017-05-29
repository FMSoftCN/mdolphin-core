#include "TestAssert.h"

namespace UnitTest {

static TestNotifyClient m_testNotifyClient;
TestNotifyClient* TestAssert::m_pNotifyClient = &m_testNotifyClient;

TestNotifyClient* testNotifyClient()
{
    return &m_testNotifyClient;
}

void TestAssert::testAssertFailIf(bool shouldFail, 
                                  const char* testClass, 
                                  const char* testFunc, 
                                  const char* message, 
                                  const char* fileName, 
                                  int fileLine )
{
    if (shouldFail && m_pNotifyClient) {

        TestResult tr;
        strcpy(tr.class_type, testClass);
        strcpy(tr.fun_name, testFunc);
        strcpy(tr.real_result,"");
        strcpy(tr.expect_result,"");
        strcpy(tr.msg, message);
        strcpy(tr.file_name, fileName);
        sprintf(tr.file_line, "%d", fileLine);
        m_pNotifyClient->updateData(&tr);

    }

}

void TestAssert::testAssertFailNotEqual(const char* expect, 
                                        const char* real, 
                                        const char* testClass, 
                                        const char* testFunc, 
                                        const char* message, 
                                        const char* fileName, 
                                        int fileLine)
{
    if (m_pNotifyClient) {
        DEBUG_OUTPUT("##expect:%s, ##real:%s", expect, real );

        TestResult tr;
        strcpy(tr.class_type, testClass);
        strcpy(tr.fun_name, testFunc);
        strcpy(tr.real_result,real);
        strcpy(tr.expect_result,expect);
        strcpy(tr.msg, message);
        strcpy(tr.file_name, fileName);
        sprintf(tr.file_line, "%d", fileLine);
        m_pNotifyClient->updateData(&tr);
    }
}

void TestAssert::testAssertFailNotEqualIf(bool shouldFail, 
                                          const char* expect, 
                                          const char* real, 
                                          const char* testClass, 
                                          const char* testFunc, 
                                          const char* message, 
                                          const char* fileName, 
                                          int fileLine)
{
    if (shouldFail && m_pNotifyClient)
        testAssertFailNotEqual(expect, real, testClass, testFunc, message, fileName, fileLine);
}

}
