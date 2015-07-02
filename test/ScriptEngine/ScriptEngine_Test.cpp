#include "ScriptEngine/ScriptEngine.h"


#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_ScriptEngine (){
    cout << "Go Testing" << __FILE__ << endl;

    ScriptEngine se;

    cout << se.getCurrentFullPath () << endl;
    std::string searchPath(se.getCurrentFullPath ());
    searchPath += string("/Asset/script");
    se.addSearchPath ( searchPath.c_str () );

    se.executeString ("require('main')");

    return 0;
}

TC_Entry( ScriptEngine )

#endif // ENABLE_TESTCASE
