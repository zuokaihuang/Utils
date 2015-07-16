#include <memory/MemBuffer.h>
#include <assert.h>
#include <memory.h>
#include <sys/stat.h>
#include <stdio.h>

// 不要写在头文件 避免编译出来的库大小不匹配
#define MEMBUFFER_DEFAULT_SIZE 512

MemBuffer::MemBuffer(size_t nSize) :
    m_pBuffer(NULL),
    m_dataSize(0),
    m_capacity(0)
{
    if (nSize == 0 || nSize == (size_t) -1){
        m_capacity = MEMBUFFER_DEFAULT_SIZE;
    }

    require (m_capacity);
}

MemBuffer::MemBuffer(const MemBuffer &rhs){
    *this = rhs;// call operator=()
}

MemBuffer::~MemBuffer(){
    empty ();
}

const MemBuffer& MemBuffer::operator= (const MemBuffer& rhs){
    if (&rhs != this){
        this->appendData (rhs.data (), rhs.size ());
    }
    return *this;
}

void* MemBuffer::require (size_t size){
    if ( m_pBuffer != NULL
        &&  (size == 0 || (m_capacity - m_dataSize) > size)){
        // 当前buffer足够用
        return (m_pBuffer+m_dataSize);
    }
    size_t new_size;
    new_size = ( m_capacity == 0? MEMBUFFER_DEFAULT_SIZE : m_capacity );
    const size_t alignment = 8;
    if (new_size % alignment != 0){
        new_size += alignment - new_size % alignment;
    }
    while( (new_size - m_dataSize) < size ){
        new_size <<= 1;
    }
    // 第一次申请
    if ( m_pBuffer == NULL){
         m_pBuffer = (unsigned char*)calloc(1, new_size);
    }else{
        if (new_size != m_capacity){
            // realloc
            void* new_buffer = realloc (m_pBuffer, new_size);
            if (new_buffer){
                m_pBuffer = (unsigned char*)new_buffer;
                memset(m_pBuffer + m_dataSize, 0, new_size - m_dataSize);
            }else{
                return 0;
            }
        }
    }
    m_capacity = new_size;
    return m_pBuffer + m_dataSize;
}

size_t MemBuffer::appendData(const void* pData, size_t nsize){
    if ( pData == NULL) return m_dataSize;
    void* ptr = this->require (nsize);
    memcpy (ptr, pData, nsize);
    m_dataSize += nsize;
    return nsize;
}

size_t MemBuffer::insertData (size_t offset, const void *pData, size_t size){
    if ( offset > m_dataSize ){ // assert
        return 0;
    }
    if ( pData == NULL || size == 0){
        return 0;
    }
    // ensure buffer enough
    this->require (size);
    memmove (m_pBuffer + offset + size,
             m_pBuffer + offset,
             m_dataSize - offset);

    memcpy (m_pBuffer + offset, pData, size);
    m_dataSize += size;

    return size;
}

size_t MemBuffer::deleteData(size_t offset, size_t size){
    if ( offset > m_dataSize || size == 0)
        return 0;

    memmove ( m_pBuffer + offset,
              m_pBuffer + offset + size,
              size);
    memset (m_pBuffer + offset + size, 0, size);

    m_dataSize -= size;

    return size;
}

size_t MemBuffer::loadFromFile(const char* filepath){
    if (!filepath) return 0;

    struct stat64 st;
    stat64(filepath, &st);
    off_t file_size = st.st_size;

    FILE* fin;
    if ((fin = fopen (filepath, "rb")) == NULL)
        return 0;

    void* ptr = this->require (file_size);

    size_t nRead = fread (ptr, 1, file_size, fin);
    fclose(fin);

    m_dataSize += nRead;

    return nRead;
}

size_t MemBuffer::saveToFile(const char* filepath){
    if (!filepath) return 0;

    FILE* fout;
    if ((fout = fopen (filepath, "wb+")) == NULL)
        return 0;
    size_t nWrite = fwrite (this->data (), 1, this->size (), fout);
    fclose(fout);
    return nWrite;
}


void MemBuffer::shrink (){
    if (m_pBuffer == NULL || m_dataSize == m_capacity)
        return;
    size_t new_size = (m_dataSize > 0) ? m_dataSize : MEMBUFFER_DEFAULT_SIZE;
    void* new_buffer = realloc (m_pBuffer, new_size);
    if (new_buffer){
        m_pBuffer = (unsigned char*)new_buffer;
        m_capacity = new_size;
    }
}

void MemBuffer::empty (){
    if (m_pBuffer)
        free(m_pBuffer);
    m_pBuffer = NULL;

    m_dataSize = 0;
    m_capacity = 0;
}


// =====================
size_t MemBuffer::pushInt(const int* v){
    return appendData (v, sizeof(*v));
}

size_t MemBuffer::pushLong(const long* v){
    return appendData (v, sizeof(*v));
}

// [s][t][r][i][n][g][6]
size_t MemBuffer::pushString(const char* v, const size_t size){
     appendData (v, size );
     appendData (&size, sizeof(size_t));
}

size_t MemBuffer::pushBool(const bool* v){
    return appendData (v, sizeof(*v));
}

size_t MemBuffer::pushFloat(const float* v){
    return appendData (v, sizeof(*v));
}

size_t MemBuffer::pushDouble(const  double* v){
    return appendData (v, sizeof(*v));
}

int     MemBuffer::popInt(){
#define TYPE int
    size_t offset = sizeof(TYPE);
    TYPE* v = (TYPE*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
    return *v;
}

long    MemBuffer::popLong(){
#define TYPE long
    size_t offset = sizeof(TYPE);
    TYPE* v = (TYPE*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
    return *v;
}
#include <iostream>
using namespace std;
char*   MemBuffer::popString(){
    size_t offset = sizeof(size_t);
    size_t* size = (size_t*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
     char* string = (char*)(m_pBuffer+m_dataSize-*size);
    m_dataSize-=*size;
    return string;
}

bool    MemBuffer::popBool(){
#define TYPE bool
    size_t offset = sizeof(TYPE);
    TYPE* v = (TYPE*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
    return *v;
}

float   MemBuffer::popFloat(){
#define TYPE float
    size_t offset = sizeof(TYPE);
    TYPE* v = (TYPE*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
    return *v;
}

double  MemBuffer::popDouble(){
#define TYPE double
    size_t offset = sizeof(TYPE);
    TYPE* v = (TYPE*)(m_pBuffer+m_dataSize-offset);
    m_dataSize-=offset;
    return *v;
}

