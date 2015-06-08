#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <SafeQueue.h>

template <typename T>
class MessageQueue
{
public:
    MessageQueue(){}
    ~MessageQueue(){}

    void push(T& t){
        std::lock_guard<std::mutex> guard_access(m_Mutex);

        std::unique_lock<std::mutex> _lock(m_Mutex_Cond);
        m_Queue.push (t);
        m_CondV.notify_one ();
    }

    T& pop(){
        std::lock_guard<std::mutex> guard_access(m_Mutex);

//        if ( m_Queue.size () == 0){
            std::unique_lock<std::mutex> _lock(m_Mutex_Cond);
//            m_CondV.wait (_lock);
//        }

        m_CondV.wait (_lock, [this](){ return !m_Queue.is_empty ();});

        T& front(m_Queue.pop ());

        return front;
    }

    bool is_empty(){
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Queue.size () == 0 ? true : false;
    }

    size_t size() {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Queue.size ();
    }

private:
    std::mutex m_Mutex;
//    std::deque<T> m_Queue;
    SafeQueue<T> m_Queue;

    std::mutex m_Mutex_Cond;
    std::condition_variable m_CondV;
};

#endif // MESSAGEQUEUE_H
