#ifdef ENABLE_TESTCASE

#include "cache/LRU.h"

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_LRU (){
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
}

TC_Entry( LRU )

#endif // ENABLE_TESTCASE

