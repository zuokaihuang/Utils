#include "memory/MemoryPool.h"
#include <iostream>

#include <iostream>
using namespace std;
MemoryPool::MemoryPool() :
    m_TotalSize(0),
    m_TotalBlock(0)
{
    cout << "init" << __func__ << endl;
}

MemoryPool::~MemoryPool(){
    std::lock_guard<std::mutex> _l(m_Mutex);

    for(auto item: m_PtrMap){
        void* ptr = (void*)item.first;
        ::free( ptr );
    }
}

void* MemoryPool::alloc (size_t size){
    std::lock_guard<std::mutex> _l(m_Mutex);

    void* ptr = malloc(size);
    auto mb = new MemoryBlock;
    unsigned long id = (uint64_t)ptr;

    if (ptr){
        m_PtrMap.insert ( std::make_pair(ptr, size) );
        m_TotalSize += size;
        m_TotalBlock++;
    }
    return ptr;
}

void MemoryPool::free (void* &ptr){
    std::lock_guard<std::mutex> _l(m_Mutex);
    auto it = m_PtrMap.find (ptr);

    DAssert_True( it == m_PtrMap.end () || it->first != ptr , "MemoryPool Item NotFound, May Free Twice");

    ::free( (void*)it->first );
    ptr = nullptr;
    m_TotalBlock--;
    m_TotalSize -= it->second;

    m_PtrMap.erase (it);

}

void MemoryPool::clear (){
    auto it = m_PtrMap.begin ();

    while( it != m_PtrMap.end () ){
        void* ptr = (*it).first;
        this->free ( ptr );
        it = m_PtrMap.begin ();// this->free will change the container m_PtrMap
    }
}

void MemoryPool::dump (){
    std::lock_guard<std::mutex> _l(m_Mutex);

    std::cout << "MemoryPool(0x" << this << ")\n"
              << "  Total size : " << m_TotalSize << "\n"
              << "  Total count: " << m_TotalBlock << "\n"
              << std::endl;
}

