#include "time/time.h"
#include <chrono>


uint64_t GetNowUs (){
    uint64_t now = std::chrono::system_clock::now ().time_since_epoch ().count ();
    return now;
}

uint64_t GetNowMs (){
    uint64_t now = std::chrono::system_clock::now ().time_since_epoch ().count ();
    return now/1000;
}

