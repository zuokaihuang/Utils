#include "Debug.h"
using namespace std;

std::ostream* fp_logout = &cout;
static std::ofstream fout;

void DebugLogToFileOpen(const std::string filename){

    fout.open (filename, std::ofstream::binary|std::ofstream::app);
    if(fout.is_open ())
        fp_logout = &fout;
    else
        fp_logout = &cout;
}

std::ostream& DebugGetOutPutLogFile(){
    return *fp_logout;
}

void DebugLogToFileClose(){
    if (fp_logout != &cout){
        fout.flush ();
        fout.close ();
    }
}

mutex &get_log_file_mutex(){
    static std::mutex mtx;
    return mtx;
}

#ifdef ENABLE_TESTCASE

#include <thread>
#include <testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("Debug", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        DLog(1, 1.1, "Hello", (int*)0x0, 1.2f, 1==1, string("你好"));
        ELog(1, 22, "error");

        DebugLogToFileOpen("/tmp/hello");
        FLog(  111, 222);
        DebugGetOutPutLogFile() << "hello" ;

        for(int i=0;i<100;i++){
            auto th = new std::thread([](){
                FLog( "world", "ppp");
                return 0;
            });
        }


        DebugLogToFileClose ();
        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
