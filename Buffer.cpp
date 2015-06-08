#include "Buffer.h"

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

size_t Buffer::copy (const void *data, size_t size){
    if ( this->capacity () - this->size() >= size){
        memcpy ( (char*)m_Data + m_Size, data, size );
        m_Size += size;
        return size;
    }
    return 0;
}

// RingBuffer
RingBuffer::RingBuffer(MemoryPool *pool, size_t capacity):
    Buffer(pool, capacity),
    m_pMemoryPool( pool ),
    m_Data(pool->alloc ( capacity ) ),
    m_Capacity( capacity ),
    m_Size(0)
{
}

RingBuffer::RingBuffer(const RingBuffer &rhs):
    Buffer(rhs.m_pMemoryPool, capacity ()),
    m_pMemoryPool(rhs.m_pMemoryPool),
    m_Capacity( rhs.m_Capacity ),
    m_Data(rhs.m_pMemoryPool->alloc ( rhs.m_Capacity )),

    m_Size(0)
{
    memcpy (m_Data, rhs.data (), rhs.capacity () );
}



#ifdef ENABLE_TESTCASE

#include <testing.h>
#include <iostream>
using namespace std;

#include <testing.h>


TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("Buffer", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        MemoryPool pool;

        Buffer buffer( &pool, 10);

        buffer.copy ( "hellohelloX", 11); // too big, will not copy
        cout << (char*)buffer.data () << "," << buffer.size () << "," << buffer.capacity () << endl;

        buffer.copy ( "world", 5);
        cout << (char*)buffer.data () << "," << buffer.size () << "," << buffer.capacity () << endl;

        Buffer otherBuffer (buffer);
        cout << (char*)otherBuffer.data () << "," << otherBuffer.size () << "," << otherBuffer.capacity () << endl;

        cout << "test RingBuffer" << endl;

        RingBuffer ringBuffer( &pool, 10);

        return 0;
    }, arg );

}
TESTCASE_END

#endif // ENABLE_TESTCASE
