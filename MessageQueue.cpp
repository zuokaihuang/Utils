#include "MessageQueue.h"

#ifdef ENABLE_TESTCASE
#include <iostream>
using namespace std;
#include <thread>
#include <chrono>
#include <testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("MessageQueue", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        MessageQueue<int> mq;

        auto th1 = new std::thread([](MessageQueue<int>* mq){
                int i=0;
                cout << "pusher ===>" << endl;
                while(i++ < 10){
                    cout << "push ===>" << i << "," << mq->size () << endl;
                    cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                    mq->push (i);
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }
            return 0;
        }, &mq);

        auto th2 = new std::thread([](MessageQueue<int>* mq){
                cout << "poper ===>" << endl;
                while(true){
                    if ( !mq->is_empty ())
                    {
                        int val = mq->pop ();
                        cout << "time===>" << std::chrono::system_clock::now ().time_since_epoch ().count () << endl;
                        cout << "val:" << val << endl;
                        if (val == 10) break;
                        continue;
                    }
                    std::this_thread::sleep_for (std::chrono::microseconds(1));
                }
            return 0;
        }, &mq);

        th1->join ();
        th2->join ();

        return 0;
    }, arg );
}
TESTCASE_END

#endif //ENABLE_TESTCASE
