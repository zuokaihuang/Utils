#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <debug/Debug.h>

class MessageLooper;

class Message
{
public:
    Message(MessageLooper* Looper);
    ~Message();
    void post(u_int32_t ms);
private:
    MessageLooper* m_MessageLooper;
public:
    int what;
    uint64_t when;
};

uint64_t GetNowUs ();

#endif // MESSAGE_H
