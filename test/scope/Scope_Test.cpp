#include "scope/Scope.h"

#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

int do_open(){
    int fd = 100;
    cout << "Open Resource:" << fd << endl;
    return fd;
}

void do_close(int fd){
    cout << "Release Resource:" << fd << endl;
}

static int test_Scope(){
    cout << "Go Testing" << __FILE__ << endl;

    auto fd = do_open();
    ScopeGuard fd_grard( [&fd]{ do_close(fd); } );

    return 0;
}

TC_Entry( Scope )

#endif // ENABLE_TESTCASE
