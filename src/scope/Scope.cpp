#include "scope/Scope.h"


ScopeGuard::~ScopeGuard (){
    if ( !_dissmissed ){
        _destructor();
    }
}

#ifdef ENABLE_TESTCASE

#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int do_open(){
    int fd = 100;
    cout << "Open Resource:" << fd << endl;
    return fd;
}

void do_close(int fd){
    cout << "Release Resource:" << fd << endl;
}


#include <debug/testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("Scope", [](){
        cout << "Go Testing" << __FILE__ << endl;

        auto fd = do_open();
        ScopeGuard fd_grard( [&fd]{ do_close(fd); } );

        return 0;
    }, arg );
}
TESTCASE_END

#endif // ENABLE_TESTCASE

