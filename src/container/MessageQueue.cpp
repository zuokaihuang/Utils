#include "container/MessageQueue.h"

#ifdef ENABLE_TESTCASE
#include <iostream>
using namespace std;
#include <thread>
#include <chrono>
#include <debug/testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("MessageQueue", [](){
        cout << "Go Testing" << __FILE__ << endl;

        MessageQueue<int*> mq;

        auto th1 = new std::thread([ &mq ](){
                int i = 0;
                cout << "pusher ===>" << endl;
                while(i++ < 10){
                    auto val = new int;
                    *val = i;
                    mq.push ( val );
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }
            return 0;
        });
#if 0
        auto th2 = new std::thread([ &mq ](){
                cout << "poper ===>" << endl;
                while(true){
                    if ( !mq.is_empty ())
                    {
                        int* val = mq.pop_with_block ();
                        if (!val) continue;
                        cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                        cout << "val:" << *val << endl;
                        if (*val == 10) {delete val; break;}

                        delete val;
                    }
                    std::this_thread::sleep_for (std::chrono::microseconds(2));
                }
            return 0;
        });
#elif 0
        auto th2 = new std::thread([ &mq ](){
                cout << "poper ===>" << endl;
                while(true){
                    int* val = mq.pop_non_block ();
                    if ( !val){
                        cout << "nothing" << endl;
                        std::this_thread::sleep_for (std::chrono::seconds(1));
                        continue;
                    }
                    cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    cout << "val:" << *val << endl;
                    if (*val == 10) {delete val; break;}
                    delete val;
                }
            return 0;
        });
#else 1
        auto th2 = new std::thread([ &mq ](){
                cout << "poper ===>" << endl;
                while(true){
                    int* val = mq.pop_with_timeout ( 500);
                    if ( !val ){
                        cout << "nothing" << endl;
                        std::this_thread::sleep_for (std::chrono::seconds(1));
                        continue;
                    }
                    cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    cout << "val:" << *val << endl;
                    if (*val == 10) {delete val; break;}
                    delete val;
                }
            return 0;
        });
#endif
        th1->join ();
        th2->join ();

        return 0;
    }, arg );
}
TESTCASE_END

#endif //ENABLE_TESTCASE
