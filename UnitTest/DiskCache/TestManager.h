#ifndef TestManager_h
#define TestManager_h
#include "TestBase.h"

#define TMLIST_MAX_SIZE  10

namespace UnitTest {

typedef TestBase* TMList[TMLIST_MAX_SIZE];

class TestManager : public TestBase {
public:
    TestManager(void);
    virtual ~TestManager(void);

    virtual bool testInit(void);
    virtual void testExit(void); 

    virtual bool testExec(void);

    virtual bool addTest(TestBase* testBase);
    virtual bool delTest(TestBase* testBase);

private:
    TMList m_tmList;
};

}
#endif
