#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_testing(){
    cout << "Go Testing" << __FILE__ << endl;
    return 0;
}

TC_Entry( testing )

#endif // ENABLE_TESTCASE
