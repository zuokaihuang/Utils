#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <mutex>
#include <queue>
#include <cassert>

template <typename T>
class SafeQueue
{
public:
    SafeQueue(){}
    ~SafeQueue(){}

    void push(T& t){
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_Queue.push (t);
    }

    T& pop(){
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (m_Queue.size () > 0){
            T& front(m_Queue.front ());
            m_Queue.pop ();
            return front;
        }
        assert(!"The SafeQueue Is Empty");
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
    std::queue<T> m_Queue;
};

#endif // SAFEQUEUE_H
