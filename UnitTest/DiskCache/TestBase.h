#ifndef TestBase_h
#define TestBase_h
#include "TestDataType.h"
//#define CHECK_EQ(X, Y) assert(X == Y)
//#define EXPECT_EQ(a, b)    CHECK_EQ(a, b)
//#define EXPECT_FALSE(cond) assert(!(cond)) 
//#define EXPECT_TRUE(cond) assert(cond) 
//#define ASSERT_TRUE(X)   assert(X)
//#define ASSERT_EQ(e1, e2) assert((e1) == (e2))


#ifdef __cplusplus
extern "C"
{
#endif

namespace UnitTest {

class TestBase{
public:
    TestBase()
    {

    }

    virtual ~TestBase(void) 
    {
        DEBUG_OUTPUT("Destroy");
    }

    //初始化测试
    virtual bool testInit(void) 
    {
        DEBUG_OUTPUT("Init");
        return false;
    }
    
    //退出测试
    virtual void testExit(void) 
    {
        DEBUG_OUTPUT("Exit");
    }

    //执行测试
    virtual bool testExec(void) 
    {
        DEBUG_OUTPUT("Execute");
        return false;
    }

    virtual bool addTest(TestBase* testBase) 
    {
        return false;
    }

    virtual bool delTest(TestBase* testBase)
    {
        return false;
    }
};

}

#ifdef __cplusplus
}
#endif
#endif  // TestBase_h
