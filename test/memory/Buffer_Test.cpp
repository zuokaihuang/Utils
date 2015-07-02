#ifdef ENABLE_TESTCASE
#include "memory/Buffer.h"
#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_Buffer (){
    cout << "Go Testing" << __FILE__ << endl;

    MemoryPool pool;

    Buffer buffer( &pool, 10);

    buffer.copy ( "hellohelloX", 11); // too big, will not copy
    cout << (char*)buffer.data () << "," << buffer.size () << "," << buffer.capacity () << endl;

    buffer.copy ( "world", 5);
    cout << (char*)buffer.data () << "," << buffer.size () << "," << buffer.capacity () << endl;

    Buffer otherBuffer (buffer);
    cout << (char*)otherBuffer.data () << "," << otherBuffer.size () << "," << otherBuffer.capacity () << endl;
    pool.clear ();

    cout << "test RingBuffer" << endl;

    RingBuffer ringBuffer( &pool, 3);
    int rs = 0;

    rs = ringBuffer.copy ("hel", 3);
    DLog("copied:", rs, (char*)ringBuffer.data () );

    rs = ringBuffer.copy ("worldp", 6);
    DLog("copied:", rs, (char*)ringBuffer.data ());

    rs = ringBuffer.copy ("worldpq", 7);
    DLog("copied:", rs, (char*)ringBuffer.data ());

    cout << "capacity:" << ringBuffer.capacity () << "=>" << (char*)ringBuffer.data ()  << endl;



    return 0;
}

TC_Entry( Buffer )

#endif // ENABLE_TESTCASE

