#include <iostream>
using namespace std;

#include <testing.h>
#include <MemoryPool.h>

#ifdef REVERSION
#else
 #define REVERSION "0.0"
#endif

int main(int argc, char *argv[])
{
    cout << "Hello Utils..." << REVERSION << endl;
//    fflush(stdout);

    TestCase& testcase = Sigleton<TestCase>();
    testcase.dump ();

//    testcase.runAllTestCase ();
//    testcase.runTestCase ("testing");
//    testcase.runTestCase ("Tree");
//    testcase.runTestCase ("MemoryPool");
//    testcase.runTestCase ("Sigleton");
    testcase.runTestCase ("Buffer");
//    testcase.runTestCase ("Debug");
//    testcase.runTestCase ("SafeQueue");

    cout << "All Done." << endl;

    return 0;
}
