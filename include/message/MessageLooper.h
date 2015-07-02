#ifndef MESSAGELOOPER_H
#define MESSAGELOOPER_H

#include <container/MessageQueue.h>
#include <message/Message.h>
#include <list>
#include <thread>

class MessageLooper
{
public:
    MessageLooper();
    ~MessageLooper();
    Message* getMessage();

    void post(Message* message);
    void poll();

private:
    uint64_t                m_MinimalWhenToWakeUp;
    std::thread*            m_pWorkThread;
    bool                    m_bIsRunning;
    std::mutex              m_AccessLock;
    MessageQueue<Message*> m_MessageQueue;
    std::list<Message*> m_ExecutableMessageList;
};

#endif // MESSAGELOOPER_H
