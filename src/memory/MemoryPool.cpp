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


#ifdef ENABLE_TESTCASE

#include <debug/testing.h>
#include <iostream>
using namespace std;


class A{
public:
    A(){
        cout << "A ctor" << endl;
    }
    ~A(){
        cout << "A dctor" << endl;
    }
};

TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("MemoryPool", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        MemoryPool pool;

        auto ptr1 = pool.alloc (5);
        auto ptr2 = pool.alloc (10);
        auto ptr3 = pool.alloc (11);

        pool.dump ();

        pool.free (ptr1);
        pool.free (ptr2);
        pool.dump ();

        int* ptr4 = (int*)ptr3;
//        ptr4++; // error !!
        pool.free ( (void*&)ptr4 );

        // clear
        cout << "clear" << endl;
        pool.dump ();
        pool.clear ();

        pool.dump ();
        cout << "clear done." << endl;

        // for c++
        A* a = pool.cnew<A>();
        pool.dump ();

        pool.cdelete<A>(a);
        pool.dump ();
        cout << "a=>" << a << endl;
        return 0;
    }, arg );

}
TESTCASE_END

#endif // ENABLE_TESTCASE
