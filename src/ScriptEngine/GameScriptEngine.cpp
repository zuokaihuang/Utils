#include "GameScriptEngine.h"
#include <iostream>
using namespace std;

#include <unistd.h>

#define FILE_PATH_MAX_LEN 1024

GameScriptEngine::GameScriptEngine():
    ScriptEngine()
{
    this->init ();
}

GameScriptEngine::~GameScriptEngine()
{

}

void dumpstack(lua_State* env);

void GameScriptEngine::init (){
    // 注册全局函数

    getGlobalTable ()->push_ref ("prequire", [](lua_State* state) -> int{

        auto se = GameScriptEngine::getInstance ();
        auto env = se->getLuaState ();
        assert ( !(env != state) && "Lua state error!" );

        dumpstack(env);

//        auto args_cnt = LuaCheckParamCount(state);
//        if ( args_cnt != 2) {
//            cout << "arg num error" << endl;
//            lua_pushnil (state);
//            return 1;
//        }

        const char* path = NULL;
        path = !lua_isstring (state, 1) ? NULL : luaL_checkstring( state, 1);
        if ( !path ){
            cout << "arg error not string:" << lua_typename ( state, lua_type(state, -1) ) << endl;
            lua_pushnil (state);
            return 1;
        }

        if ( !lua_istable(state, 2) ) {
            cout << "==> " << lua_typename ( state, lua_type(state, 2) ) << endl;
            cout << "arg error not table:" << lua_typename ( state, lua_type(state, 2) ) << endl;
            lua_pushnil (state);
            return 1;
        }

        const char* key = "onSucess";
        int rs = 0;

        lua_getfield ( state, 2, key);
        cout << "--------------------------.==> " << lua_typename ( state, lua_type(state, 2) ) << endl;
        rs = lua_pcall(env, 0, 1, 0);
        cout << "rs " << rs << endl;

        lua_pop(env, 1); // 还原stack, 因为 lua_getfield

        dumpstack(env);

//      第四个参数的table
        lua_getfield ( state, 4, key);
        cout << "--------------------------.==> " << lua_typename ( state, lua_type(state, 2) ) << endl;
        rs = lua_pcall(env, 0, 1, 0);
        cout << "rs " << rs << endl;

        lua_pop(env, 1); // 还原stack, 因为 lua_getfield

        dumpstack(env);


        cout << "path:" << path << endl;

        lua_pushinteger (state, 1);
        return 1;
    });

}

void dumpstack(lua_State* env){
    if (!env) return;
    int count = lua_gettop (env);
cout << "\n*****************Dump stack*****************" <<endl;
    for( int i=1; i <= count ;i++){
        int type = lua_type (env, i);
        cout << i << "." << lua_typename (env, type) << " => ";

        switch (type) {
        case LUA_TSTRING:{
            cout << luaL_checkstring(env, i) << endl;
        }break;
        case LUA_TNUMBER:{
            cout << luaL_checknumber (env, i) << endl;
        }break;
        case LUA_TBOOLEAN:{
            cout << (luaL_checkinteger (env, i)==0 ? "false" :"true") << endl;
        }break;
        default:
            cout << endl;
            break;
        }
    }
    cout << "=================Dump End=================" <<endl;
}

void GameScriptEngine::setBasepath (string &path){
    m_basepath = path;
    addSearchPath ( path.c_str () );
}

void GameScriptEngine::addExecuteFile (const char *name, const char *filename){
    char current_folder[FILE_PATH_MAX_LEN];
    getcwd (current_folder, FILE_PATH_MAX_LEN);
    char buffer[FILE_PATH_MAX_LEN];
    memset (buffer, '\0', FILE_PATH_MAX_LEN);
    if ( strlen(filename) > 7 && strncasecmp ("file://", filename, 7) == 0 ){
        // start with 'file://'
        snprintf (buffer, FILE_PATH_MAX_LEN, "%s", (filename+7 ));
    }else{
        // not start with 'file://'
       snprintf (buffer, FILE_PATH_MAX_LEN, "%s/%s", current_folder, filename);
    }
    m_executeFileMap[name] = buffer;

    executeScriptFile (buffer);
}

void GameScriptEngine::pollExecuteFiles (){
    for( auto& it : m_executeFileMap){
        const char* name = it.first;
        runFileFixedUpdate(name);
    }

}

void GameScriptEngine::runFileFixedUpdate(const char *name){
    cout << name << endl;
}


#ifdef ENABLE_TESTCASE

#include <debug/testing.h>
#include <iostream>
using namespace std;
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("GameScriptEngine", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;
        GameScriptEngine *se = GameScriptEngine::getInstance ();

        cout << se->getCurrentFullPath () << endl;
        std::string searchPath(se->getCurrentFullPath ());
        searchPath += string("/Asset/script");
        se->addSearchPath ( searchPath.c_str () );

        se->executeString ("require('gamemain')");

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE


