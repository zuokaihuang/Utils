#include <net/HttpC.h>


#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_HttpC(){
    cout << "Go Testing" << __FILE__ << endl;

    HttpC http;
    std::string url = "http://m.baidu.com/?from=1002296a";

    http.setWriteToFile (true, true);
    http.get (url);

    cout << "http code:" << http.getStateCode () << endl;
    cout << "http content length:" << http.getContentLength () << endl;
    if (http.getContent ())
        cout << "content:" << (char*)http.getContent () << endl;

    return 0;
}

TC_Entry( HttpC )

#endif // ENABLE_TESTCASE
