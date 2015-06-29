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

#ifdef ENABLE_TESTCASE

#include <debug/testing.h>
#include <iostream>
using namespace std;
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("LRU", [](){
        cout << "Go Testing" << __FILE__ << endl;

        LRU<const char*> lru(3);
        lru.add ("1", "1");
        lru.add ("2", "2");
        lru.add ("3", "3");
        lru.add ("4", "4");

        const char* val = lru.get ("4");
        cout << "val:" << val << endl;
        cout << "size:" << lru.size() << endl;
        for( auto i : lru){
            cout << *i->v << endl;
            i++;
        }


        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
