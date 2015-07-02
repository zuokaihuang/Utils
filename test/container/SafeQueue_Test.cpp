#ifdef ENABLE_TESTCASE
#include "container/SafeQueue.h"
#include "debug/testing.h"
#include <iostream>
using namespace std;
#include <thread>

static int test_SafeQueue(){
    cout << "Go Testing" << __FILE__ << endl;

    SafeQueue<int> sq;

    auto th1 = new std::thread([](SafeQueue<int>* sq){
            int i=0;
            while(i++ < 10){
                sq->push (i);
            }
        return 0;
    }, &sq);
    auto th2 = new std::thread([](SafeQueue<int>* sq){
            while(true){
                if ( !sq->is_empty ())
                {
                    int val = sq->pop ();
                    cout << "val:" << val << endl;
                    if (val == 10) break;
                    continue;
                }
                std::this_thread::sleep_for (std::chrono::seconds(1));
            }
        return 0;
    }, &sq);

    th1->join ();
    th2->join ();

    return 0;
}

TC_Entry( SafeQueue )

#endif // ENABLE_TESTCASE


