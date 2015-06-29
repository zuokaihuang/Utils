#ifndef TESTING_H
#define TESTING_H

#include <functional>
#include <map>

#include <debug/Debug.h>
#include "scope/Sigleton.h"
typedef std::function<void()> TestCaseEntryFunction;
struct TestCaseEntry{
    TestCaseEntry(TestCaseEntryFunction func){
        func();
    }
};

class TestCase
{
public:
    TestCase();
    ~TestCase();
public:

    typedef std::function<int()> TestFunction;
    void addTestCase(std::string name, TestFunction function, void*arg);
    void runAllTestCase();
    void runTestCase(std::string name);
    void dump();
private:
    typedef
    struct _tagTestItem{
        TestFunction func;
    }TestItem;
    std::map< std::string, TestItem* > mTestFunctions;
};

/*
 * Old Version
 * Deprecated
*/
#define TESTCASE_START \
static TestCaseEntry _TestCaseEntry([](){ \
    do{ \

#define TESTCASE_END \
    }while(0); \
});


#define TC_Entry( __NAME__ ) \
    extern "C" { void tc##__NAME__(){ test_##__NAME__(); } } \
    TESTCASE_START \
    { \
        void* arg = NULL; \
        TestCase& testcase = Sigleton<TestCase>(); \
        testcase.addTestCase ( #__NAME__ ,test_testing, NULL ); \
    } \
    TESTCASE_END



#endif // TESTING_H
