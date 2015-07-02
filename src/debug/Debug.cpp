#include "debug/Debug.h"
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


