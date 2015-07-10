#include <message/MessageLooper.h>
#include <debug/Debug.h>
#include <chrono>
#include <time/time.h>

MessageLooper::MessageLooper(){
    m_MinimalWhenToWakeUp = LONG_MAX;

    m_bIsRunning = true;
    m_pWorkThread =
            new std::thread(
                [ this ]() -> void{
                    DLog("MessageLooper WorkThread init");
                    while( m_bIsRunning ){
                        {// m_AccessLock lock
                            std::lock_guard<std::mutex> _l(m_AccessLock);
                            auto it = m_DeferMessageList.begin ();
                            while( it != m_DeferMessageList.end () ){
                                Message* msg = *it;
                                if (msg != nullptr){
                                    if ( msg->whenUs < GetNowUs ()){
                                        m_ExecutableMessageList.push_back (msg);
                                        m_DeferMessageList.erase (it);
                                        it = m_DeferMessageList.begin ();
                                    }else{
                                        m_MinimalWhenToWakeUp = msg->whenUs;
                                        break; // sorted, so meet fisrt whenUs > now()
                                    }
                                }else{
                                    if (m_bIsRunning){
                                        break;
                                    }
                                }
                                it++;
                            }
                        }// m_AccessLock unlock
                        int64_t time = m_MinimalWhenToWakeUp - GetNowUs ();
                        time = time > 0 ? time : 0;
                        std::unique_lock<std::mutex> uni_l(m_cond_mutex);

                        using namespace std::chrono;
                        std::chrono::duration<int64_t, std::micro> dura(time);

                        m_cond.wait_for( uni_l, dura);
                    }
                });
}

MessageLooper::~MessageLooper(){
    m_bIsRunning = false;
    if (m_pWorkThread){
        post (nullptr);
        m_pWorkThread->join ();
        DLog("MessageLooper has", (m_DeferMessageList.size ()), "defer message and", m_ExecutableMessageList.size (), "processable message not process");
        delete m_pWorkThread;
    }
    for (Message* message : m_ExecutableMessageList) {
        delete message;
    }
    for (Message* message : m_DeferMessageList) {
        delete message;
    }
}

Message* MessageLooper::getMessage (){
    auto messsage = new Message(this);
    return messsage;
}

void MessageLooper::post (Message *message){
    std::lock_guard<std::mutex> _l(m_AccessLock);
    if ( !message ){
        m_cond.notify_one ();
        return;
    }
    uint64_t now = GetNowUs ();
    if (message->whenUs > now){
        std::lock_guard<std::mutex> __guard(m_cond_mutex);
        if ( message->whenUs < m_MinimalWhenToWakeUp){
            m_MinimalWhenToWakeUp = message->whenUs;
            DLog("update minimal time", m_MinimalWhenToWakeUp );
        }
        // m_MessageQueue.push (message);
        m_DeferMessageList.push_back (message);
        m_DeferMessageList.sort ( [](const Message*a, const Message*b){ return a->whenUs < b->whenUs;} );
        m_cond.notify_one ();
    } else {
        m_ExecutableMessageList.push_back (message);
    }
}

void MessageLooper::drain (){
    auto start = GetNowUs ();
//    DLog("drain start at:" , start);
    for (Message* message : m_ExecutableMessageList) {
        if (message->handleFunction){
            message->handleFunction(message);
        }
        if (message->handler){
            message->handler->onMessage (message);
        }
        delete message;
    }
    m_ExecutableMessageList.clear ();
    DLog("drain done, take time:" , (GetNowUs () - start) , "us");
}


