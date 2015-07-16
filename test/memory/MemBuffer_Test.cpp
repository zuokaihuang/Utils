#ifdef ENABLE_TESTCASE
#include "memory/MemBuffer.h"
#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_MemBuffer (){
    cout << "Go Testing" << __FILE__ << endl;

    MemBuffer mem;

    cout << "default capacity:" << mem.capacity () << endl;

    mem.require (512);
    cout << "now capacity:" << mem.capacity () << endl;

    mem.require (513);
    cout << "now capacity:" << mem.capacity () << endl;

    cout << "not use=>" << mem.data () << endl;
    mem.appendData ("hello", 5);
    cout << "=>" << ( mem.data ()!=NULL?(char*) mem.data (): NULL) << endl;

    mem.appendData (" world", 6);
    cout << "=>" << ( mem.data ()!=NULL?(char*) mem.data (): NULL) << endl;

    cout << "now size:" << mem.size () << " capacity:" << mem.capacity () << endl;
    mem.shrink ();
    cout << "now size:" << mem.size () << " capacity:" << mem.capacity () << endl;

    mem.insertData (6, "code ", 5);
    cout << "inserted size:" << mem.size () << " capacity:" << mem.capacity () << endl;
    cout << "=>" << ( mem.data ()!=NULL?(char*) mem.data (): NULL) << endl;

    mem.deleteData (6, 5);
    cout << "deleted size:" << mem.size () << " capacity:" << mem.capacity () << endl;
    cout << "=>" << ( mem.data ()!=NULL?(char*) mem.data (): NULL) << endl;

    MemBuffer other;
    other = mem;
    cout << "other size:" << other.size () << " capacity:" << other.capacity () << endl;
    cout << "other=>" << ( other.data ()!=NULL?(char*) other.data (): NULL) << endl;

    size_t rs = other.saveToFile ("/tmp/temp_test");
    cout << "write :" << rs << endl;

    mem.empty ();
    rs = mem.loadFromFile ("/tmp/temp_test");
    cout << "read :" << rs << endl;
    cout << "read size:" << mem.size () << " capacity:" << mem.capacity () << endl;
    cout << "=>" << ( mem.data ()!=NULL?(char*) mem.data (): NULL) << endl;
    unlink ("/tmp/temp_test");

    cout << "=============================\n\n" << endl;
    int i = 1;
    long l = 1000;
    char string[] = "hello world";
    bool b = true;
    float f = 2.0f;
    double d = 100.1;


    mem.empty ();


    mem.pushInt ( &i );
    mem.pushLong ( &l );
    mem.pushString ( string, strlen(string) );
    mem.pushBool ( &b );
    mem.pushFloat ( &f );
    mem.pushDouble ( &d );
cout << "push size:" << mem.size () << " capacity:" << mem.capacity () << endl;

    other.empty ();
    other = mem;

    cout << "double:" << other.popDouble () << endl;
    cout << "float:" << other.popFloat () << endl;
    cout << "bool:" << other.popBool () << endl;
    cout << "stirng:" << other.popString () << endl;
    cout << "loing:" << other.popLong () << endl;
    cout << "int:" << other.popInt () << endl;

    return 0;
}

TC_Entry( MemBuffer )

#endif // ENABLE_TESTCASE

