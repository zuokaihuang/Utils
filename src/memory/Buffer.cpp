#include "memory/Buffer.h"

Buffer::Buffer(MemoryPool *pool, size_t capacity):
    m_pMemoryPool( pool ),
    m_Data(pool->alloc ( capacity ) ),
    m_Capacity( capacity ),
    m_Size(0)
{
}

Buffer::Buffer(const Buffer &rhs):
    m_pMemoryPool(rhs.m_pMemoryPool),
    m_Capacity( rhs.m_Capacity ),
    m_Size(0)
{
    m_Data = rhs.m_pMemoryPool->alloc (m_Capacity);
    DAssert_True( m_Data == nullptr, "Buffer alloc memory failed!");
    memcpy (m_Data, rhs.data (), rhs.capacity () );
}

size_t Buffer::copy (const Buffer &rhs){
    if (capacity () - size() >= rhs.size ()){
        memcpy (m_Data, rhs.data (), rhs.size () );
        return rhs.size ();
    }
    return 0;
}

size_t Buffer::copy (const char *data, size_t size){
    if ( this->capacity () - this->size() >= size){
        memcpy ( (char*)m_Data + m_Size, data, size );
        m_Size += size;
        return size;
    }
    return 0;
}

size_t Buffer::capacity () const{
    return m_Capacity;
}

void* Buffer::data () const{
    return m_Data;
}

size_t Buffer::size () const{
    return m_Size;
}

// RingBuffer
RingBuffer::RingBuffer(MemoryPool *pool, size_t capacity):
    Buffer(pool, capacity)
{

}

RingBuffer::RingBuffer(const RingBuffer &rhs):
    Buffer(rhs.m_pMemoryPool, capacity ())
{
    memcpy (m_Data, rhs.data (), rhs.capacity () );
}

size_t RingBuffer::copy(const char *data, size_t sz){
    size_t left_space = this->capacity () - this->size();
    size_t copied_cnt = 0;

    if (left_space > sz){
        memcpy ( (char*)m_Data + m_Size, data, sz );
        m_Size += sz;
        copied_cnt += sz;
    }else{
        if (left_space == 0){
            m_Size = 0;
            left_space = capacity ();
        }
        memcpy ( (char*)m_Data + m_Size, data, left_space );
        copied_cnt += left_space;
        m_Size += left_space;

        size_t npos = left_space ;
        size_t next_to_copy = sz - copied_cnt;

        if (next_to_copy >= capacity () ){
            copied_cnt += this->copy ( data+copied_cnt, next_to_copy );
        }else{
            memcpy ( (char*)m_Data, data + npos, next_to_copy );
            copied_cnt += next_to_copy;
        }
    }
    return copied_cnt;
}

size_t RingBuffer::copy(const Buffer& rhs){
    return 0;
}


