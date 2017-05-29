#ifndef TestAssert_h
#define TestAssert_h
#include "TestDataType.h"
#include "Converter.h"
#include "TestNotifyClient.h"

namespace UnitTest {

#define TEST_ASSERT(condition) TEST_ASSERT_FAIL((condition), "", "", "")

#define TEST_ASSERT_FAIL(condition, testClass, testFunc, message)       \
    (UnitTest::TestAssert::testAssertFailIf( !(condition),          \
                                             #testClass,            \
                                             #testFunc,             \
                                             #message,              \
                                             FILE_AND_LINE()))

#define TEST_ASSERT_OK(condition, testClass, testFunc, message)         \
    (UnitTest::TestAssert::testAssertFailIf( (condition),       \
                                             #testClass,        \
                                             #testFunc,         \
                                             #message,          \
                                             FILE_AND_LINE()))
//compare two object
#define TEST_ASSERT_EQUAL(expect, real, testClass, testFunc, message)   \
    (UnitTest::assertEqual( (expect),       \
                            (real),         \
                            #testClass,     \
                            #testFunc,      \
                            #message,       \
                            FILE_AND_LINE()))


class TestAssert {
    public:

        static void testAssertFailIf(bool shouldFail, 
                                     const char* testClass, 
                                     const char* testFunc, 
                                     const char* message, 
                                     const char* fileName, 
                                     int fileLine );

        static void testAssertFailNotEqual(const char* expect, 
                                           const char* real, 
                                           const char* testClass, 
                                           const char* testFunc, 
                                           const char* message, 
                                           const char* fileName, 
                                           int fileLine);

        static void testAssertFailNotEqualIf(bool shouldFail, 
                                             const char* expect, 
                                             const char* real, 
                                             const char* testClass, 
                                             const char* testFunc, 
                                             const char* message, 
                                             const char* fileName, 
                                             int fileLine);

    public:
        static TestNotifyClient* m_pNotifyClient;
};

template<class T>
struct Compare {
    static bool equal(const T& x, const T& y)
    {
        printf( "%d, %d\n", x, y);
        return x == y;
    }

    static Str toStr(const T& x)
    {
        Converter cv;
        cv << x;
        return cv.buffer();
    }

};

template <>
struct Compare<double> {
    static bool equal(double x, double y)
    {
        return fabs(x - y) < 10e-6;
    }

    static Str toStr(double x)
    {
        Converter cv;
        cv << x;
        return cv.buffer();
    }
};

template<class T>
void assertEqual(const T& expect, 
                 const T& real, 
                 const char* testClass, 
                 const char* testFuc, 
                 const char* message, 
                 const char* fileName, 
                 int fileLine)
{
    if(!Compare<T>::equal(expect, real)) {
        TestAssert::testAssertFailNotEqual(Compare<T>::toStr(expect).c_str(),
                                           Compare<T>::toStr(real).c_str(),
                                           testClass,
                                           testFuc,
                                           message,
                                           fileName,
                                           fileLine);
    }
}

TestNotifyClient* testNotifyClient();
}

#endif
