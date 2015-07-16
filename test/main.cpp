#include <iostream>
using namespace std;

#include <debug/testing.h>
#include <memory/MemoryPool.h>

#ifdef REVERSION
#else
#define REVERSION "0.0"
#endif

int main(int argc, char *argv[])
{
    auto start = GetNowMs ();
    TestCase& testcase = Sigleton<TestCase>();
//   testcase.dump ();

    //    testcase.runAllTestCase ();
//        testcase.runTestCase ("testing");
    //    testcase.runTestCase ("Tree");
    //    testcase.runTestCase ("MemoryPool");
//        testcase.runTestCase ("Sigleton");
    //    testcase.runTestCase ("Buffer");
//        testcase.runTestCase ("Debug");
//        testcase.runTestCase ("SafeQueue");
//    testcase.runTestCase ("MessageQueue");
//    testcase.runTestCase ("URI");
//    testcase.runTestCase ("HttpC");
//    testcase.runTestCase ("ScriptEngine");
//    testcase.runTestCase ("AssetManager");
//    testcase.runTestCase ("GameScriptEngine");
//    testcase.runTestCase ("LRU");
//    testcase.runTestCase ("Net");
//    testcase.runTestCase ("message");
//    testcase.runTestCase ("md5");
//    testcase.runTestCase ("MessageLooper");
    testcase.runTestCase ("MemBuffer");

    cout << "\n\nTotal Time:" << GetNowMs() - start << " ms" << endl;
    cout << "All Done." << endl;
    return 0;
}
