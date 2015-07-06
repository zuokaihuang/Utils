#include <ScriptEngine/GameScriptEngine.h>


#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_GameScriptEngine(){
    cout << "Go Testing" << __FILE__ << endl;

    GameScriptEngine *se = GameScriptEngine::getInstance ();

    cout << se->getCurrentFullPath () << endl;
    std::string searchPath(se->getCurrentFullPath ());
    searchPath += string("/Asset/script");
    se->addSearchPath ( searchPath.c_str () );
    se->addSearchPath ("http://127.0.0.1:8989/");

    se->executeString ("require('gamemain')");
    se->executeString ("load_web()"); // load 第二次时不用重新去网络取数据

    return 0;
}

TC_Entry( GameScriptEngine )

#endif // ENABLE_TESTCASE
