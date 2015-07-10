#include "message/Message.h"
#include <message/MessageLooper.h>
#include <time/time.h>

Message::Message(MessageLooper *Looper)
{
    this->m_MessageLooper = Looper;
}

Message::~Message()
{

}

void Message::post (u_int32_t ms){
    DAssert( m_MessageLooper == NULL, "MessageLooper is nullptr");
    DAssert ( handleFunction == NULL && handler == NULL ,"Message handler and handle function both nullptr" );
    this->whenUs = ms*1000 + GetNowUs();
    m_MessageLooper->post (this);
}
