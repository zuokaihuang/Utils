#include "message/Message.h"
#include <message/MessageLooper.h>
#include <chrono>

Message::Message(MessageLooper *Looper)
{
    this->m_MessageLooper = Looper;
}

Message::~Message()
{

}

void Message::post (u_int32_t ms){
    DAssert( m_MessageLooper!= NULL, "MessageLooper is nullptr");
    this->when = ms + GetNowUs();
    m_MessageLooper->post (this);
}

uint64_t GetNowUs (){
    uint64_t now = std::chrono::system_clock::now ().time_since_epoch ().count ();
    return now;
}
