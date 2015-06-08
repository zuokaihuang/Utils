#ifndef BUFFER_H
#define BUFFER_H

#include "MemoryPool.h"

class Buffer{
public:
    explicit Buffer(MemoryPool* pool, size_t capacity);
    explicit Buffer(const Buffer& rhs);

    size_t copy(const void *data, size_t size);
    size_t copy(const Buffer& rhs);
    void* data() const { return m_Data; }
    size_t capacity() const { return m_Capacity; }
    size_t size() const { return m_Size; }

private:
    void*  m_Data;
    size_t m_Capacity;
    size_t m_Size;
    MemoryPool* m_pMemoryPool;
};

class RingBuffer : public Buffer{
public:
    explicit RingBuffer(MemoryPool* pool, size_t capacity);
    explicit RingBuffer(const RingBuffer &rhs);

    size_t copy(const void *data, size_t size);
    size_t copy(const Buffer& rhs);

private:
    void*  m_Data;
    size_t m_Capacity;
    size_t m_Size;
    MemoryPool* m_pMemoryPool;
};

#endif // BUFFER_H
