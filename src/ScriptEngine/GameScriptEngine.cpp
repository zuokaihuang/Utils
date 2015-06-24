#include "GameScriptEngine.h"
#include <iostream>
using namespace std;

#include <unistd.h>

#define FILE_PATH_MAX_LEN 1024

GameScriptEngine::GameScriptEngine():
    ScriptEngine()
{

}

GameScriptEngine::~GameScriptEngine()
{

}

void GameScriptEngine::init (){
    // 注册全局函数
    lua_CFunction fptr = [](lua_State*m_pState){
        cout << "call foo" << endl;
        if ( !lua_istable(m_pState, -1)){
            cout << "is not table" << endl;
            return -1;
        }
        GameScriptEngine* se = GameScriptEngine::getInstance ();

        lua_pushnil ( m_pState );

        while(lua_next (m_pState, -2) != 0){
            lua_pushvalue (m_pState , -2);
            const char* key = lua_tostring(m_pState, -1);

            int type = lua_type (m_pState, -2);
            switch(type){
            case LUA_TTABLE:{
                switch(type){
                case LUA_TTABLE:{
                    const char* name = "<unknow>";
                    const char* path = "<unknow>";
                    lua_pushvalue (m_pState, -2);
                    lua_getfield ( m_pState, -1, "name");
                    name = lua_tostring(m_pState, -1);
                    lua_pop(m_pState, 1);
cout << "name" << name << endl;
                    lua_getfield (m_pState, -1, "path");
                    path = lua_tostring( m_pState, -1);
                    lua_pop(m_pState, 1);

                    cout << "name " << name << " path "<< path << endl;
                    se->addExecuteFile (name, path);
                    lua_pop(m_pState, 1);
                }break;
                default:
                    cout << "[ " << __LINE__ << " Error:\"" << key << "\"] => " << "(" << lua_typename (m_pState, lua_type (m_pState, 2)) << ")" << std::endl;
                }
            }break;
            case LUA_TSTRING:{
                auto val = lua_tostring(m_pState, -2);
                cout << "==>[\"" << key << "\"] => " << val << "(" << lua_typename (m_pState, lua_type (m_pState, -2)) << ")" << std::endl;
            }break;
            default:
                cout << "[ " << __LINE__ << " Error:\"" << key << "\"] => " << "(" << lua_typename (m_pState, lua_type (m_pState, 2)) << ")" << std::endl;
            }

            lua_pop(m_pState, 2);
        }
        lua_pop(m_pState, 1);

        return 0;
    };

    //pushFunctionToLuaStack (fptr, "addscript");
    getGlobalTable ()->push_ref ("addscript", fptr);
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
        //const char* filename = it.second;

        runFileFixedUpdate(name);
    }

//    int rs = call ("bundle_update");
//    cout << rs << endl;
}

void GameScriptEngine::runFileFixedUpdate(const char *name){
    cout << name << endl;
//    int rs = call ("bundle_update");
//    cout << rs << endl;
}

