#include <message/MessageLooper.h>
#include <debug/Debug.h>
#include <chrono>

MessageLooper::MessageLooper(){
    m_MinimalWhenToWakeUp = LONG_MAX;

    m_bIsRunning = true;
    m_pWorkThread = new std::thread([ this ]() -> void{
        DLog("MessageLooper WorkThread init");
        while( m_bIsRunning ){
            std::lock_guard<std::mutex> _l(m_AccessLock);
            DLog("timeout");
            int64_t wait =  GetNowUs () - m_MinimalWhenToWakeUp;
//            DLog("wait:", m_MinimalWhenToWakeUp, GetNowUs () , wait);
//            DLog("size:", m_MessageQueue.size () );
            Message* msg = m_MessageQueue.pop_with_timeout ( wait );
            if ( !msg ){ continue; }
            // Got a msg timeouted
            m_ExecutableMessageList.push_back (msg);
                                            DLog(msg);
        }
    });
}

MessageLooper::~MessageLooper(){
    m_bIsRunning = false;
    if (m_pWorkThread){
        auto msg = this->getMessage ();
        m_MessageQueue.push ( msg ); // wake up the queue
        m_pWorkThread->join ();
        delete m_pWorkThread;
    }
}

Message* MessageLooper::getMessage (){
    auto messsage = new Message(this);
    return messsage;
}

void MessageLooper::post (Message *message){
    std::lock_guard<std::mutex> _l(m_AccessLock);
    uint64_t now = GetNowUs ();
    if (message->when > now){
        if ( message->when < m_MinimalWhenToWakeUp){
            DLog("aaa");
            m_MinimalWhenToWakeUp = message->when;
        }
        m_MessageQueue.push (message);
    } else {
        DLog("bbb");
        m_ExecutableMessageList.push_back (message);
    }
}

void MessageLooper::poll (){
//    DLog("size:", m_MessageQueue.size () );
//    while( !m_MessageQueue.is_empty () ){
//        Message* message = m_MessageQueue.pop_with_block ();

//        DLog("Message what:", message->what);

//        delete message;
//    }
    for (Message* message : m_ExecutableMessageList) {
        DLog("Message what:", message->what);
        delete message;
    }
}


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
