#include "config.h"
#include <string.h>
#include "TestManager.h"
namespace UnitTest {

TestManager::TestManager(void)
{
    memset(m_tmList, 0, sizeof(m_tmList));
    DEBUG_OUTPUT("Create");
}

TestManager::~TestManager(void)
{
    memset(m_tmList, 0, sizeof(m_tmList));
    DEBUG_OUTPUT("Destroy");
}

bool TestManager::testInit(void)
{
    int i;
    for (i = 0; i < TMLIST_MAX_SIZE; i++) {
        if (m_tmList[i])
            m_tmList[i]->testInit();
    }

    DEBUG_OUTPUT("OK");
    return true;
}

void TestManager::testExit(void)
{
    int i;
    for (i = 0; i < TMLIST_MAX_SIZE; i++) {
        if (m_tmList[i])
            m_tmList[i]->testExit();
    }

    DEBUG_OUTPUT("OK");
}

bool TestManager::testExec(void)
{
    int i;
    for (i = 0; i < TMLIST_MAX_SIZE; i++) {
        if (m_tmList[i])
            m_tmList[i]->testExec();
    }

    DEBUG_OUTPUT("OK");
    return true;
}

bool TestManager::addTest(TestBase* testBase)
{
    int i;
    for(i = 0; i < TMLIST_MAX_SIZE; i++) {
        if(m_tmList[i] == NULL) {
            m_tmList[i] = testBase;

            DEBUG_OUTPUT("OK");
            return true;
        }
    }
    return false;
}

bool TestManager::delTest(TestBase* testBase)
{
    int i;
    for(i = 0; i < TMLIST_MAX_SIZE; i++) {
        if(m_tmList[i] == testBase) {
            m_tmList[i] = NULL;
            break;
        }
    }

    DEBUG_OUTPUT("OK");
    return true;
}

}
