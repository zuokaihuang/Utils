#include <ScriptEngine/GameScriptEngine.h>
#include <iostream>
using namespace std;

#include <unistd.h>
#include <asset/AssetManager.h>
#include <thread>

#define FILE_PATH_MAX_LEN 1024

GameScriptEngine::GameScriptEngine():
    ScriptEngine()
{
    this->init ();
}

GameScriptEngine::~GameScriptEngine()
{

}


void GameScriptEngine::init (){
    // 注册全局函数

    getGlobalTable ()->push_ref ("prequire", [](lua_State* state) -> int{

        auto se = GameScriptEngine::getInstance ();
        auto env = se->getLuaState ();
        assert ( !(env != state) && "Lua state error!" );

//        LuaDumpStack(env);

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
//cout << "-=--=-=-=-=-=-=-=-=-=-]" << endl;
//        LuaDumpTable (env, 2);
        const char* key = "onSucess";
        int rs = 0;

        lua_getfield ( state, 2, key);
//        rs = lua_pcall(env, 0, 1, 0);
//        cout << "rs " << rs << endl;

        lua_pop(env, 1); // 还原stack, 因为 lua_getfield

//        LuaDumpStack(env);

        //      第四个参数的table
        lua_getfield ( state, 4, key);
//        cout << "--------------------------.==> " << lua_typename ( state, lua_type(state, 2) ) << endl;
        rs = lua_pcall(env, 0, 1, 0);
        cout << "rs " << rs << endl;

        lua_pop(env, 1); // 还原stack, 因为 lua_getfield

//        LuaDumpStack(env);

//        // dump table
//        LuaDumpTable (env, 4);

//        LuaDumpStack(env);
        const size_t buffer_size = 512;
        char buffer[buffer_size];
        memset(buffer, 0, buffer_size);
        snprintf (buffer, buffer_size, "local status, err = pcall(require, '%s');if status ~= true then error('error to load module ') end;\0", path);
        rs = se->executeString (buffer);
        if (rs != 0){
            auto am = AssetManager::getInstance ();
            string abs_path(path);
            abs_path += ".lua";
            string full_path = am->getFileFullPath (abs_path);
            if ( full_path.size () != 0){
                cout << "Found" << full_path << endl;
            }else{
                cout << "Not Found, try in web" << full_path << endl;
                full_path = am->getFileFullWebPath (abs_path);
                if (full_path.size () > 0){
                    cout << "Found in web:" << full_path << endl;
                    string local_path = am->getWebFile (full_path, false, "_prequire_");
                    cout << local_path << endl;
                    se->executeScriptFile (local_path.c_str ());
                }else{
                    cout << "Not found in web:" << full_path << endl;
                }
            }
        }
        cout << "path:" << path << endl;

        lua_pushinteger (state, 1);
        return 1;
    });


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

void GameScriptEngine::addSearchPath (const char *path){
    if ( strncasecmp ("http", path, 4) != 0){
        ScriptEngine::addSearchPath (path);
    }else{
        AssetManager::getInstance ()->addSearchWebPath (path);
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
    testcase.addTestCase ("GameScriptEngine", [](){
        cout << "Go Testing" << __FILE__ << endl;
        GameScriptEngine *se = GameScriptEngine::getInstance ();

        cout << se->getCurrentFullPath () << endl;
        std::string searchPath(se->getCurrentFullPath ());
        searchPath += string("/Asset/script");
        se->addSearchPath ( searchPath.c_str () );
        se->addSearchPath ("http://127.0.0.1:8989/");

        se->executeString ("require('gamemain')");

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE


