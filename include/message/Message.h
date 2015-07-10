#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <debug/Debug.h>
#include <functional>

class MessageLooper;
//class MessageHandler;
class Message;

class MessageHandler{
public:
    virtual void onMessage(const Message* message)=0;
};

typedef std::function<void(const Message*)> MessageHandlerFunction;

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
    uint64_t whenUs;
    MessageHandlerFunction handleFunction;
    MessageHandler *handler;
};

#endif // MESSAGE_H
