#ifndef TESTING_H
#define TESTING_H

#include <functional>
#include <map>

#include "Sigleton.h"
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

    typedef std::function<int(void* arg)> TestFunction;
    void addTestCase(std::string name, TestFunction function, void*arg);
    void runAllTestCase();
    void runTestCase(std::string name);
    void dump();
private:
    typedef
    struct _tagTestItem{
        TestFunction func;
        void* arg;
    }TestItem;
    std::map< std::string, TestItem* > mTestFunctions;
};



#define TESTCASE_START \
static TestCaseEntry _TestCaseEntry([](){ \
    do{ \

#define TESTCASE_END \
    }while(0); \
});


#endif // TESTING_H
