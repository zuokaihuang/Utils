#include "net/URI.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

URI::URI(std::string uri)
{
    origin = uri;
    parse();
}

URI::~URI()
{

}

void URI::parse (){
    //    char* sche = checkScheme(origin.c_str (), origin.size () );
    //    cout << "scheme:" << sche << endl;
    char* pat = checkPath(origin.c_str (), origin.size () );
//    cout << "path:" << pat << endl;
}

char *URI::checkScheme(const char* uri, const size_t size){
    char *delim = "://";
    char *p, *mark , *str;
    mark = str = strdup(uri);
    p = strtok(str, delim);
    if (p){
        p = strdup(p);
    }
    free(mark);
    return p;
}

void split( std::vector<std::string>& stringVector,
            const std::string & str,
            const std::vector<std::string>& delims){
    size_t  start = 0, end = 0;

    int delims_idx = 0;
    while ( end != string::npos)
    {
        int idx = (++delims_idx) > delims.size () ? delims.size () -1 : delims_idx;
        string delim = delims[ idx ];
        end = str.find( delim, start);

        // If at end, use length=maxLength.  Else use length=end-start.
        stringVector.push_back( str.substr( start,
                       (end == string::npos) ? string::npos : end - start));

        // If at end, use start=maxSize.  Else use start=end+delimiter.
        start = (   ( end > (string::npos - delim.size()) )
                  ?  string::npos  :  end + delim.size());
    }
}

char* URI::checkPath (const char *uri, const size_t size){
    using namespace std;
    string str(uri);
    cout << str << endl;

    string::size_type cur_pos ,next;
    cur_pos = 0;
    do{
        next = str.find_first_of ("://", cur_pos);
        if  (next == string::npos) break;
        if (next < str.size ()){
            cout << "scheme:" << str.substr (0, next)<< endl;
            cur_pos += next+3;
        }

        next = str.find_first_of ("/", cur_pos);
        if  (next == string::npos) break;
        if (next < str.size ()){
            string::size_type tmp_pos = cur_pos, tmp_next = next;

            bool flag = false;
            tmp_next = str.find_first_of (":", tmp_pos);
            if (tmp_next < str.size ()){
                cout << "port:" << str.substr (tmp_next+1, next - tmp_next-1) << endl;
                flag = true;
            }
            if (flag){
                // with port
                cout << "path case 1:" << str.substr (cur_pos, next-cur_pos-3)<< endl;
                cur_pos = next;
            }else{
                // without port
                cout << "path case 2:" << str.substr (cur_pos, next-cur_pos)<< endl;
                cur_pos = next;
            }
        }

        next = str.find_first_of ("/", cur_pos);
        if  (next == string::npos) break;
        if (next < str.size ()){
            cout << "query:" << str.substr (next+1, string::npos)<< endl;
            cur_pos= next;
        }

        return "";
    }while(0);

    cout << "parsing error" << endl;
    return "";
}


#ifdef ENABLE_TESTCASE

#include <testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("URI", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        URI url("http://www.baidu.com:80/query?hello=ppp");

        cout << "=============" << endl;

        URI url2("file:/-/www.qq.com");

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE

