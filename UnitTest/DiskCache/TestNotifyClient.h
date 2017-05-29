#ifndef NotifyClient_h
#define NotifyClient_h
#include "TestDataType.h"

namespace UnitTest {

class TestNotifyClient {
    public:
        TestNotifyClient()
        {
            m_pfnClientCallback = NULL;
        }

        virtual ~TestNotifyClient()
        {
            m_pfnClientCallback = NULL;
        }
        
        void setNotifyCallback(PFN_CLIENT_CALLBACK_T pfnClientCallback)
        {
            m_pfnClientCallback = pfnClientCallback;
        }

        void updateData(TestResult* testResult)
        {
            DEBUG_OUTPUT("testResult:%p", testResult);
            if(m_pfnClientCallback)
                m_pfnClientCallback(testResult);
        }

    private:
        PFN_CLIENT_CALLBACK_T m_pfnClientCallback;
};

}

#endif //NotifyClient_h
