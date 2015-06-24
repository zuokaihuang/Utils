#include "testing.h"

#include <iostream>
using namespace std;

TestCase::TestCase()
{
//    cout << __func__ << "=>" << this << endl;
}

TestCase::~TestCase()
{

}

void TestCase::addTestCase (string name, TestFunction function, void *arg){
    auto item = new TestItem{function, arg};

    mTestFunctions.insert ( std::make_pair(name, item));
}

void TestCase::runAllTestCase(){
    auto item = mTestFunctions.begin ();

    while( item != mTestFunctions.end () ){
        auto name = item->first;
        TestItem* i = item->second;
        i->func(i->arg);

        item++;
    }
}

void TestCase::runTestCase (string name){
    auto item = mTestFunctions.find (name);
    if (item != mTestFunctions.end ()){
        TestItem* i = item->second;
        i->func(i->arg);
    }
}

void TestCase::dump (){
    cout << "TestCase Dump:\n"
         << "Total Case:" << mTestFunctions.size () << "\n"
         << "\n"
         << endl;

    auto item = mTestFunctions.begin ();
    while( item != mTestFunctions.end () ){
        auto name = item->first;
        cout << "=>" << name << endl;

        item++;
    }

}

/*
#include <testing.h>
#include <iostream>
using namespace std;
static TestCaseEntry _TestCaseEntry([](){
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("testing", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;
        return 0;
    }, arg );

});
*/

#ifdef ENABLE_TESTCASE

#include <testing.h>
#include <iostream>
using namespace std;
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("testing", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;
        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
