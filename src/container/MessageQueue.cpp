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

        MessageQueue<int> mq;

        auto th1 = new std::thread([](MessageQueue<int>* mq){
                int i=0;
                cout << "pusher ===>" << endl;
                while(i++ < 10){
                    // cout << "push ===>" << i << "," << mq->size () << endl;
                    // cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    mq->push (i);
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }
            return 0;
        }, &mq);
#if 0
        auto th2 = new std::thread([](MessageQueue<int>* mq){
                cout << "poper ===>" << endl;
                while(true){
                    if ( !mq->is_empty ())
                    {
                        int val = mq->pop_with_block ();
                        cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                        cout << "val:" << val << endl;
                        if (val == 10) break;
                        continue;
                    }
                    std::this_thread::sleep_for (std::chrono::microseconds(2));
                }
            return 0;
        }, &mq);
#elif 0
        auto th2 = new std::thread([](MessageQueue<int>* mq){
                cout << "poper ===>" << endl;
                while(true){
                    int def = 0;
                    int val = mq->pop_non_block (def);
                    if (val == def){
                        cout << "nothing" << endl;
                        std::this_thread::sleep_for (std::chrono::seconds(1));
                        continue;
                    }
                    cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    cout << "val:" << val << "def:" << def << endl;
                    if (val == 10) break;
                }
            return 0;
        }, &mq);
#else 1
        auto th2 = new std::thread([](MessageQueue<int>* mq){
                cout << "poper ===>" << endl;
                while(true){
                    int def = 0;
                    int val = mq->pop_with_timeout (def, 500);
                    if (val == def){
                        cout << "nothing" << endl;
                        std::this_thread::sleep_for (std::chrono::seconds(1));
                        continue;
                    }
                    cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    cout << "val:" << val << "def:" << def << endl;
                    if (val == 10) break;
                }
            return 0;
        }, &mq);
#endif
        th1->join ();
        th2->join ();

        return 0;
    }, arg );
}
TESTCASE_END

#endif //ENABLE_TESTCASE
