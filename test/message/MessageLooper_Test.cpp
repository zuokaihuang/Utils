#include <message/MessageLooper.h>
#include <debug/Debug.h>
#include <chrono>


#ifdef ENABLE_TESTCASE

#include <debug/testing.h>
#include <iostream>
using namespace std;

class Handler : public MessageHandler{
public:
    Handler() : MessageHandler(){}
    void onMessage (const Message* message){
        cout << "class hander:\n" << message->what << endl;
    }
};

static int test_MessageLooper(){
    cout << "Go Testing" << __FILE__ << endl;

    auto handle_func = [](const Message*msg){
        cout << "func hander:\n" << msg->what << endl;
    };
    auto handler = new Handler;

    MessageLooper* looper = new MessageLooper;

    Message* msg1 = looper->getMessage ();
    msg1->what = 1;
    msg1->handleFunction = handle_func;
    msg1->post (100);

    Message* msg2 = looper->getMessage ();
    msg2->what = 2;
    msg2->handler = handler;
    msg2->post (0);

    Message* msg3 = looper->getMessage ();
    msg3->what = 3;
    msg3->handleFunction = handle_func;
    msg3->post (5000);

//    std::this_thread::sleep_for (std::chrono::milliseconds(200));

    looper->drain ();


    delete looper;
    return 0;
}

TC_Entry( MessageLooper )


#endif // ENABLE_TESTCASE
