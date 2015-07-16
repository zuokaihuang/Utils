#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include <stdlib.h>
#include <stddef.h>

class MemBuffer{
public:
    MemBuffer(size_t nSize=-1);
    MemBuffer(const MemBuffer& rhs);
    virtual ~MemBuffer();
    const MemBuffer& operator= (const MemBuffer& rhs);

    size_t appendData(const void* pData, size_t size);
    size_t insertData(size_t offset, const void* pData, size_t size);
    size_t deleteData(size_t offset, size_t size);

    size_t loadFromFile(const char* filepath);
    size_t saveToFile(const char* filepath);

    inline void* data() const { return ( m_dataSize == 0 ? NULL: m_pBuffer); }
    inline size_t size() const { return m_dataSize; }
    inline size_t capacity() const { return m_capacity; }

    void shrink();
    void empty();
    void *require(size_t size);

    // push and pop
    size_t pushInt(const int* v);
    size_t pushLong(const long* v);
    size_t pushString(const char* v, const size_t size);
    size_t pushBool(const bool *v);
    size_t pushFloat(const float* v);
    size_t pushDouble(const double* v);

    int     popInt();
    long    popLong();
    char*   popString();
    bool    popBool();
    float   popFloat();
    double  popDouble();

private:
    unsigned char*  m_pBuffer;
    size_t          m_dataSize;
    size_t          m_capacity;
};


#endif // MEMBUFFER_H

