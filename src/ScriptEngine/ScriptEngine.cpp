#include "ScriptEngine/ScriptEngine.h"
#include <assert.h>
#include <sstream>
#include <iostream>
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define DLog(...) do{ fprintf( stdout, __VA_ARGS__ ); }while(0);
#define DLogLuaError( msg ) DLog("[%s %d] [LUA ERROR] %s\n", __FILE__, __LINE__, msg );

ScriptEngine::ScriptEngine() {
    init();
}

ScriptEngine::~ScriptEngine()
{
    delete m_pTableGlobal;
    m_pTableGlobal = nullptr;


    if (m_pCurrentPath) free(m_pCurrentPath);
    m_pCurrentPath = nullptr;

    lua_close (m_pState);
    m_pState = nullptr;
}


lua_State* ScriptEngine::getLuaState (){
    return m_pState;
}

void ScriptEngine::addSearchPath ( const char* path ){
    lua_getglobal (m_pState, "package");
    lua_getfield (m_pState, -1, "path");
    const char* currentPath = lua_tostring(m_pState, -1);

    lua_pop(m_pState, 1);
    lua_pushfstring (m_pState, "%s;%s/?.lua", currentPath, path);
    lua_setfield (m_pState, -2, "path");

    lua_pop(m_pState, 1);
}

const char* ScriptEngine::getSearchPath (){
    lua_getglobal (m_pState, "package");
    lua_getfield (m_pState, -1, "path");
    const char* currentPath = lua_tostring(m_pState, -1);

    lua_pop(m_pState, 1);

    return currentPath;
}

int ScriptEngine::executeString (const char *code){
    int rs = luaL_dostring(m_pState, code);
    if ( rs != 0){
        DLogLuaError( lua_tostring( m_pState, -1) );
        lua_pop( m_pState, 1);
        return rs;
    }
    return 0;
}

int ScriptEngine::executeScriptFile (const char *filename){
    DLog("executeScriptFile:[%s]\n",filename);

    int rs;
    rs = access (filename, R_OK);
    if (rs == -1){
        DLogLuaError("File not exist");
    }

    rs = luaL_dofile(m_pState, filename);
    if ( rs != 0){
        DLogLuaError( lua_tostring( m_pState, -1) );
        cout << rs << endl;
        lua_pop( m_pState, 1);
        return rs;
    }
    return 0;
}

void ScriptEngine::init (){
    m_pState = luaL_newstate();
    luaL_openlibs (m_pState);
    m_pTableGlobal = new Table(m_pState, "", -1);

    char current_folder[1024];
    getcwd (current_folder, 1024);

    m_pCurrentPath = strdup(current_folder);
    this->addSearchPath (m_pCurrentPath);
}

Table* ScriptEngine::getGlobalTable (){
     return m_pTableGlobal;
}

const char* ScriptEngine::getCurrentFullPath (){
    return m_pCurrentPath;
}

// Table

Table* Table::getTable (const char *field){
    if (ref == -1){ // 全局表
        lua_getglobal(m_pState, field);
        if ( ! lua_istable(m_pState, -1) ){
            fprintf(stderr, "[%s]not a table, is %s\n", tablename, lua_typename (m_pState, lua_type(m_pState, -1)));
            return nullptr;
        }
        int ref = 0;
        // 在LUA_REGISTRYINDEX创建一个引用 [r, v], v为栈顶的值
        ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX);
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);
        return new Table(m_pState, tablename, ref);
    }else{
        lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref );
    }
    if ( ! lua_istable(m_pState, -1) ) {
        fprintf(stderr, "table gone.");
        return nullptr;
    }
    lua_pushvalue (m_pState, -1);
    lua_getfield ( m_pState, -1, field);
    if ( lua_type (m_pState, -1) != LUA_TTABLE ) {
        lua_pop(m_pState, 1);
        return nullptr;
    }
    // 得到一个table, 创建一个ref索引它
    int ref = luaL_ref(m_pState, LUA_REGISTRYINDEX);
    lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);

    auto tb = new Table(m_pState, field, ref);
    lua_pop(m_pState, 2);
    return tb;
}




Table* Table::createTable (const char* name, bool is_global){
    int new_ref = 0;
    if (is_global){
        lua_newtable( m_pState );
        int new_ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX);
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, new_ref);
        lua_setglobal (m_pState, name);
    }else{
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);
        if ( !lua_istable(m_pState, -1)) return nullptr;
        lua_pushstring (m_pState, name);

        lua_newtable( m_pState );
        int new_ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX);
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, new_ref);
    }
    return new Table(m_pState, name, new_ref);
}


#include <iomanip>
static void printitem(lua_State* env, const int i){
    int type = lua_type (env, i);
    cout << "[";
    switch (type) {
    case LUA_TSTRING:{
        cout << luaL_checkstring(env, i) ;
    }break;
    case LUA_TNUMBER:{
        cout << luaL_checknumber (env, i) ;
    }break;
    case LUA_TBOOLEAN:{
        cout << (luaL_checkinteger (env, i)==0 ? "false" :"true") ;
    }break;
    default:
        break;
    }
    cout << "](" << lua_typename (env, type) << ")";
}


void LuaDumpStack(lua_State* env){
    if (!env) return;

    int count = lua_gettop (env);
    cout << "\n*****************Dump stack*****************" <<endl;
    for( int i=1; i <= count ;i++){
        cout << '"' << i << '"' ;
        printitem(env, i);
        cout << endl;
    }
    cout << "=================Dump End=================" <<endl;
}

void LuaDumpTable(lua_State* env, const int index){
    if (!env) return;

    cout << "\n*****************Dump table*****************" <<endl;
    /*
     * pushnil
     * while( lua_next(env, tabla_index_of_stack) { -1 -> key; -2 -> value; lua_pop(env, 2); }
    */
    lua_pushnil (env);
    while(lua_next (env, index)){
        lua_pushvalue (env, -2);
        const char* key = lua_tostring(env, -1);
        cout << '"' << key << '"';
        printitem (env, -2);
        cout << endl;

        lua_pop(env, 2);
    }
    cout << "=================Dump End=================" <<endl;
}


