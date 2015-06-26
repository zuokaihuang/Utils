#ifndef SIGLETON_H
#define SIGLETON_H

#include <mutex>



template <typename T>
T& Sigleton(){
    static T instance;
    return instance;
}


static std::mutex __mutex__sigletonptr__;
template <typename T>
T* SigletonPtr(){
    static T* instance = nullptr;
    if (!instance){
        std::lock_guard<std::mutex> _(__mutex__sigletonptr__);
        if (!instance)
            instance = new T();
    }
    return instance;
}


#endif // SIGLETON_H
