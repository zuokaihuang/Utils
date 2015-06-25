#ifndef LRU_H
#define LRU_H

#include <array>
#include <iostream>
#include <deque>
#include <assert.h>

unsigned int hash_gethash(const char *key);

template <class T>
class LRU
{
    class Data{
    public:
        Data(){}
        unsigned int hash;
        const T* v;
    };

    typedef typename std::deque<Data*> DataType;
    typedef typename DataType::iterator iterator;
    typedef typename DataType::size_type size_type;
public:
    LRU(size_t size){
        static_assert(std::is_pointer<T>::value, "LRU only support pointer");
        m_size = size;
    }
    ~LRU(){}
    void add(const char* key, const T& t){
        if (m_data.size () == m_size){
            m_data.erase (--m_data.end ());
        }
        Data* data = new Data();
        data->hash = hash_gethash( key );
        data->v = &t;
        m_data.push_back (data);
    }

    T get(const char* key){
        using namespace std;
        unsigned int hash = hash_gethash (key);
        for(Data* it : m_data){
            cout << "hash:" << hash << "," << it->hash << endl;
            if (it->hash == hash){
                m_data.push_front ( it );
                m_data.pop_back ();
                return (T)it->v;
            }
        }
        return (T)nullptr;
    }

    iterator begin(){
        return m_data.begin ();
    }

    iterator end() {
        return m_data.end ();
    }

    size_type size(){
        return m_data.size ();
    }

private:
    size_t m_size;
    DataType m_data;
};


#endif // LRU_H
