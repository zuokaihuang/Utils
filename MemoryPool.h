#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stdlib.h>
#include <mutex>
#include <map>

#include <assert.h>

#ifndef DAssert_True
#define DAssert_True(cond, msg) do{ if( (cond) ){fflush(stdout);fflush(stderr); assert(!(cond) && msg);} }while(0);
#endif

typedef void*(*Allocater)(size_t size);
class MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();
    void* alloc(size_t size);
    void  free(void *&ptr);
    void  dump();
    void  clear();
    template <typename T>
    T* cnew(){
        // use replacement new
        void* ptr = alloc (sizeof(T)); // lock here
        if (ptr){
            std::nothrow_t _tag;
            T* t = new (ptr)  T();
            return t;
        }
        return nullptr;
    }
    template <typename T>
    void cdelete(T*& t){
        t->~T();
        free((void*&)t);
        t = nullptr;
    }

private:
    struct MemoryBlock{
        size_t id;
        void* ptr;
        size_t size;
    };

    std::mutex      m_Mutex;
    size_t m_TotalSize;
    size_t m_TotalBlock;
    std::map<void*, size_t> m_PtrMap;
};




#endif // MEMORYPOOL_H
