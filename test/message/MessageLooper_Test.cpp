#include <message/MessageLooper.h>
#include <debug/Debug.h>
#include <chrono>


#ifdef ENABLE_TESTCASE

#include <debug/testing.h>
#include <iostream>
using namespace std;
static int test_message(){
    cout << "Go Testing" << __FILE__ << endl;

    MessageLooper* looper = new MessageLooper;

    Message* msg1 = looper->getMessage ();
    msg1->what = 1;
    msg1->post (100);


    Message* msg2 = looper->getMessage ();
    msg2->what = 2;
    msg2->post (0);

    looper->poll ();

    std::this_thread::sleep_for (std::chrono::seconds(3));
    delete looper;
    return 0;
}

TC_Entry( message )


#endif // ENABLE_TESTCASE
