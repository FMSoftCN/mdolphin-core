#ifndef DiskCacheClient_h
#define DiskCacheClient_h
#include "TestManager.h"
#include "DiskCacheBackendTest.h"
#include "TestAssert.h"

namespace UnitTest {

class TestClient {
    public:
        TestClient(PFN_CLIENT_CALLBACK_T pfnClientCallback) 
        : m_pTestManager(new TestManager)
        , m_pTestBackend(new DiskCacheBackendTest)
        {
            testNotifyClient()->setNotifyCallback(pfnClientCallback);

            if (m_pTestManager) {
                m_pTestManager->addTest(m_pTestBackend);
                m_pTestManager->testInit();
            }
        }

        ~TestClient()
        {
            if (m_pTestManager)
                m_pTestManager->testExit();

            if (m_pTestBackend) {
                delete m_pTestBackend;
                m_pTestBackend = NULL;
            }

            if (m_pTestManager) {
                delete m_pTestManager;
                m_pTestManager = NULL;
            }
        }

        bool doTest()
        {
            if(!m_pTestManager)
                return false;

            DEBUG_OUTPUT("m_pTestManager:%p, m_pTestBackend:%p", 
                                            m_pTestManager, m_pTestBackend);
            return m_pTestManager->testExec();
        }


    private:
        TestBase* m_pTestManager;
        TestBase* m_pTestBackend;
};

}

#endif
