#include <iostream>
using namespace std;

#include <testing.h>
#include <MemoryPool.h>

#ifdef REVERSION
#else
#define REVERSION "0.0"
#endif

typedef struct OP{
    void operator() () {
        std::cout << "functor\n";
    }
    int operator [](int idx){
        return 100;
    }
}OP;

int main(int argc, char *argv[])
{
    OP tb;
    tb();
    cout << tb[123] << endl;
    return 0;
    clock_t start = clock ();
    TestCase& testcase = Sigleton<TestCase>();
//    testcase.dump ();

    //    testcase.runAllTestCase ();
    //    testcase.runTestCase ("testing");
    //    testcase.runTestCase ("Tree");
    //    testcase.runTestCase ("MemoryPool");
    //    testcase.runTestCase ("Sigleton");
    //    testcase.runTestCase ("Buffer");
//        testcase.runTestCase ("Debug");
//        testcase.runTestCase ("SafeQueue");
//    testcase.runTestCase ("MessageQueue");
//    testcase.runTestCase ("URI");
    testcase.runTestCase ("HTTP");


    clock_t end = clock();

    cout << "\n\nTotal Time:" << end - start << " ms" << endl;
    cout << "All Done." << endl;
    return 0;
}
