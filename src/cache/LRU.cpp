#include "cache/LRU.h"

unsigned int hash_gethash(const char* key){
    unsigned int hash = 0;
    int i;
    for(i=0; *key; i++){
        if ( (i&1) == 0 ){
            hash ^= ((hash<<7) ^ (*key++) ^(hash>>3));
        }else{
            hash ^= (~(hash<<11) ^ (*key++) ^ (hash>>5));
        }
    }
    return hash;
}


