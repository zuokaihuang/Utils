#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;
#include <thread>

static int test_Debug(){
    cout << "Go Testing" << __FILE__ << endl;

    DLog(1, 1.1, "Hello", (int*)0x0, 1.2f, 1==1, string("你好"));
    DLog("hello") << "---<<<<world\n" ;

    ELog(1, 22, "error");


    DebugLogToFileOpen("/tmp/hello");
    FLog(  111, 222);
    DebugGetOutPutLogFile() << "hello" ;

    for(int i=0;i<100;i++){
        auto th = new std::thread([](){
            FLog( "world", "ppp");
            return 0;
        });
    }

    DebugLogToFileClose ();

    return 0;
}

TC_Entry( Debug )

#endif // ENABLE_TESTCASE
