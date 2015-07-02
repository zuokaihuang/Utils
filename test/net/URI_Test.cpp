#include "net/URI.h"




#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_URI (){
    cout << "Go Testing" << __FILE__ << endl;

    URI url;
    url.parse ("http://www.baidu.com:80/q/s/query.php?hello=ppp");
    cout << "parse ok?=>" << url.valid () << endl;
    url.dump ();
    cout << "=============" << endl;

    URI url2;
    url2.parse ("file:///tmp/afile");

    cout << "parse ok?=>" << url2.valid () << endl;
    url2.dump ();
    cout << "=============" << endl;

    return 0;
}

TC_Entry( URI )

#endif // ENABLE_TESTCASE

