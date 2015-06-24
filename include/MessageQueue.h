#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <SafeQueue.h>
#include <iterator>
#include <iostream>

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

    T& pop_with_block(){
        std::lock_guard<std::mutex> guard_access(m_Mutex);

        std::unique_lock<std::mutex> _lock(m_Mutex_Cond);
        m_CondV.wait (_lock, [this](){ return !m_Queue.is_empty ();});

        T& front(m_Queue.pop ());

        return front;
    }

    ///
    /// \brief pop_non_block
    /// \return
    ///
    T& pop_non_block(T& default_val){
        std::lock_guard<std::mutex> guard_access(m_Mutex);
        std::unique_lock<std::mutex> _lock(m_Mutex_Cond);
        if (m_Queue.is_empty ()){
            std::cout << "aa" << std::endl;
            return default_val;
        }
        T& front(m_Queue.pop ());
        return front;
    }

    T& pop_with_timeout(T& default_val, int ms){
        std::unique_lock<std::mutex> _lock(m_Mutex_Cond);
        m_CondV.wait_for (_lock,  std::chrono::milliseconds(ms));
        if (m_Queue.is_empty ()){
            return default_val;
        }
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
    SafeQueue<T> m_Queue;

    std::mutex m_Mutex_Cond;
    std::condition_variable m_CondV;
};

#endif // MESSAGEQUEUE_H
