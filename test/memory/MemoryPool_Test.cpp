#ifdef ENABLE_TESTCASE
#include "memory/MemoryPool.h"
#include "debug/testing.h"
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


static int test_MemoryPool (){
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
}

TC_Entry( MemoryPool )

#endif // ENABLE_TESTCASE



