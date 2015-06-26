#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <vector>
#include <assert.h>

extern "C"{
  #include <lua/lua.hpp>
}

// Function
class Function{
    lua_State* m_pState;
    const char* functionname = nullptr;
    int ref = 0;
public:
    Function(lua_State* s, const char* name, int ref){
        this->m_pState = s;
        this->functionname = name;
        this->ref = ref;
    }
    ~Function(){
        if (ref != 0){
            luaL_unref (m_pState, LUA_REGISTRYINDEX, ref);
        }
    }
    // call lua function 模板
    #define MAKE_PUSHER(_type, _functionname) \
        int push(lua_State *l, _type data){ \
        _functionname(l, data );\
        int ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX); \
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref); \
        return ref;     \
    }
    public:
        MAKE_PUSHER(int, lua_pushinteger)
        MAKE_PUSHER(bool, lua_pushboolean)
        MAKE_PUSHER(float, lua_pushnumber)
        MAKE_PUSHER(double, lua_pushnumber)
        MAKE_PUSHER(const char*, lua_pushstring )
        MAKE_PUSHER(lua_CFunction, lua_pushcfunction)
        MAKE_PUSHER(void*, lua_pushlightuserdata )
    private: void pop_to_top(){
            if ( ref !=0 ){
                lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref );
            }else{
                lua_getglobal ( m_pState, functionname);
                ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX);
                lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);
            }
        }

    public:
        // 无参数
        int call(){
            return call("");
        }
        int call(const char*){
            pop_to_top ();
            if ( !lua_isfunction( m_pState, -1)){
                fprintf(stderr, " %s not a function", functionname);
                return 0;
            }
            int rs = lua_pcall( m_pState, 0, 1, 0);
            if ( rs != 0){  fprintf(stderr, "Call function[%s] error: %s", functionname, lua_tostring( m_pState, -1)); return 0;}
            // global function must return a integer
            if ( !lua_isnumber ( m_pState, -1) && !lua_isnil(m_pState, -1)){
                fprintf(stderr, " Global function[%s] must return integer %s\n", functionname, lua_typename (m_pState, lua_type(m_pState, -1)));
                lua_pop( m_pState, 1);
                return rs;
            }
            rs = lua_tointeger( m_pState, -1);
            lua_pop( m_pState, 1);

            return rs;
        }
    public:
        // 1个参数
        template <typename T>
        int call( const char* functionname, T t){
            pop_to_top ();
            auto value = t;
            if ( !lua_isfunction( m_pState, -1)){
                fprintf(stderr, " %s not a function", functionname);
                return 0;
            }
            push(m_pState, value);
            int rs = lua_pcall( m_pState, 1, 1, 0);
            // global function must return a integer
            if ( !lua_isnumber ( m_pState, -1) && !lua_isnil(m_pState, -1)){
                //DLogLuaError( "Global function must return integer" );
                lua_pop( m_pState, 1);
                return rs;
            }
            rs = lua_tointeger( m_pState, -1);
            lua_pop( m_pState, 1);

            return rs;
        }

    protected:
        // 2个参数以上
        template <typename T>
        int call(const int nArgs, T t){
            auto value = t;
            push( m_pState, value);
            int rs = lua_pcall( m_pState, nArgs, 1, 0);
            // global function must return a integer
            if ( !lua_isnumber ( m_pState, -1) && !lua_isnil(m_pState, -1)){
                lua_pop( m_pState, 1);
                return rs;
            }
            rs = lua_tointeger( m_pState, -1);
            lua_pop( m_pState, 1);

            return rs;
        }
    protected:
        template <typename T, typename U, typename ...Args>
        int call(const int nArgs, T&& t, U&& u,Args&&... args){
            auto value = t;
            push(m_pState, value);
            int rs = call( nArgs, u, args... );
            return rs;
        }
    public:
        template <typename Header, typename Sencond, typename ...Others>
        int call(const char* functionname, const int nArgs, Header&& t, Sencond&& u, Others&&... others){
            auto value = t;
            pop_to_top ();
            if ( !lua_isfunction(m_pState, -1)){
                fprintf(stderr, " %s not a function", functionname);
                return 0;
            }
            push(m_pState, value);
            int rs = call(nArgs, u, others... );
            return rs;
        }
        // end 2个参数以上
};

// table
///
/// Ref: 0 -> default not set
///     -1 -> global table
/// \brief The Table class
///
class Table{
public:
    lua_State* m_pState;
    const char* tablename;
    int ref;
public:
    Table(lua_State* s, const char* tn, int r){
        m_pState = s;
        tablename = tn;
        ref = r;
    }
    ~Table(){
        luaL_unref (m_pState, LUA_REGISTRYINDEX, ref);
    }

    typedef struct Value{
        const char* name;
        int type;
        union{
            const char*     val_string;
            double          val_double;
            bool            val_bool;
            int             val_table_ref;
            int             val_function_ref;
            const void*     val_lightuserdata;
        };
    }Value;

    Table* createTable(const char *name, bool is_global=false);

    void fetch(std::vector<Value*> &values){
        if (ref == -1 ){

        }else{
            lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);
        }
        if ( !lua_istable(m_pState, -1)){
            fprintf(stderr, "foreach not a table");
            return;
        }

        lua_pushvalue ( m_pState , -1);
        lua_pushnil ( m_pState );

        while(lua_next (m_pState, -2) != 0){
            lua_pushvalue (m_pState , -2);
            const char* key = lua_tostring(m_pState, -1);
            Value* val = new Value;
            values.push_back (val);
            val->name = key;

            int type = lua_type (m_pState, -2);
            val->type = type;
            lua_pop(m_pState, 1);
            switch(type){
            case LUA_TSTRING:{
                val->val_string = lua_tostring(m_pState, -1);
            }break;
            case LUA_TNUMBER:{
                val->val_double = lua_tonumber(m_pState, -1);
            }break;
            case LUA_TBOOLEAN:{
                val->val_bool = lua_toboolean (m_pState, -1);
            }break;
            case LUA_TLIGHTUSERDATA:{
                val->val_lightuserdata = lua_topointer (m_pState, -1);
            }break;
            case LUA_TTABLE:{
                val->val_table_ref = luaL_ref(m_pState, LUA_REGISTRYINDEX);
                lua_rawgeti( m_pState, LUA_REGISTRYINDEX, val->val_table_ref);
            }break;
            case LUA_TFUNCTION:{
                val->val_table_ref = luaL_ref(m_pState, LUA_REGISTRYINDEX);
                lua_rawgeti( m_pState, LUA_REGISTRYINDEX, val->val_table_ref);
            }break;
            default:   break;
            }
            lua_pop(m_pState, 1);
        }
        lua_pop(m_pState, 1);
    }

    // getTabel 特殊处理，因为要创建索引
    Table* getTable(const char* field);
    // getFunction 特殊处理，创建索引来引用它
    Function* getFunction(const char* field){
        if (ref == -1){
            lua_getglobal(m_pState, field);
            if ( ! lua_isfunction(m_pState, -1) ){
                fprintf(stderr, "[%s]not a function, is %s\n", field, lua_typename (m_pState, lua_type(m_pState, -1)));
                return nullptr;
            }
            int ref = 0;
            // 在LUA_REGISTRYINDEX创建一个引用 [r, v], v为栈顶的值
            ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX);
            lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);
            return new Function(m_pState, field, ref);
        }else{
            lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref );
        }
        if ( ! lua_istable(m_pState, -1) ){
            fprintf(stderr, "[%s]not a function, is %s\n", field, lua_typename (m_pState, lua_type(m_pState, -1)));
            return nullptr;
        }
        lua_pushvalue (m_pState, -1);
        lua_getfield ( m_pState, -1, field);
        if ( lua_type (m_pState, -1) != LUA_TFUNCTION ) {
            lua_pop(m_pState, 1);
            return nullptr;
        }

        // 得到一个function, 创建一个ref索引它
        int ref = luaL_ref(m_pState, LUA_REGISTRYINDEX);
        lua_rawgeti(m_pState, LUA_REGISTRYINDEX, ref);

        auto tb = new Function(m_pState, field, ref);
        lua_pop(m_pState, 2);
        return tb;
    }

#define do_checkout(ret, func_name, op_function, type, error_return) \
ret func_name(const char* field){ \
    if (ref == -1){ \
        lua_getglobal (m_pState, field); \
    }else{ \
        lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref ); \
    } \
    if ( ! lua_istable(m_pState, -1) ){ \
        fprintf(stderr,"%s[%s] table gone,%s\n", tablename, field, lua_typename (m_pState, lua_type(m_pState, -1)) );\
        return error_return; \
    } \
    if ( type == LUA_TTABLE)lua_pushvalue (m_pState, -1); else lua_pushvalue (m_pState, -1);\
    lua_getfield ( m_pState, -1, field); \
    if ( type != lua_type (m_pState, -1) ){ \
        fprintf(stderr,"%s[%s]type not correct:%s\n", tablename, field, lua_typename (m_pState, lua_type(m_pState, -1)) );\
        lua_pop(m_pState, 1);  \
        return error_return;   \
    } \
    auto rs = op_function(m_pState, -1); \
    lua_pop(m_pState, 2);\
    return rs; \
} // end func
    do_checkout(lua_Number, getNumber, luaL_checknumber , LUA_TNUMBER, 0)
    do_checkout(const char*, getString, luaL_checkstring , LUA_TSTRING, nullptr)
    do_checkout(int, getInt, luaL_checkinteger , LUA_TNUMBER, 0)
    do_checkout(int, getBoolean, luaL_checkinteger , LUA_TNUMBER, 0)

    // push
#define TB_MAKE_PUSHER( var_type, push_func) \
    void push(const char*key, var_type value){ \
        if (ref == -1){ \
            push_func(m_pState, value);\
            lua_setglobal ( m_pState, key); \
        } else { \
            lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref ); \
            if ( !lua_istable(m_pState, -1)) return; \
            push_func(m_pState, value);\
            lua_setfield (m_pState, -2, key);\
        } \
    }
#define TB_MAKE_PUSHER_REF( var_type, push_func) \
    int push_ref(const char* key, var_type value){ \
        if (ref == -1){ \
            push_func(m_pState, value);\
            lua_setglobal ( m_pState, key);\
        } else { \
            lua_rawgeti( m_pState , LUA_REGISTRYINDEX , ref ); \
            if ( !lua_istable(m_pState, -1)) return 0; \
            push_func(m_pState, value);\
            lua_setfield (m_pState, -2, key);\
        } \
        int l_ref = luaL_ref ( m_pState, LUA_REGISTRYINDEX); \
        return l_ref; \
    }
#define TB_MAKE_PUSHERS( var_type, push_func) \
    TB_MAKE_PUSHER_REF( var_type, push_func) \
    TB_MAKE_PUSHER( var_type, push_func)

public:
    TB_MAKE_PUSHERS( const char*, lua_pushstring )
    TB_MAKE_PUSHERS( lua_CFunction, lua_pushcfunction)
    TB_MAKE_PUSHERS( lua_Number, lua_pushnumber )
    TB_MAKE_PUSHERS( int, lua_pushinteger )
    TB_MAKE_PUSHERS( bool, lua_pushboolean )
    TB_MAKE_PUSHERS( void*, lua_pushlightuserdata )
};


class ScriptEngine
{
public:
    ScriptEngine();
    ~ScriptEngine();

    lua_State* getLuaState ();

    const char* getCurrentFullPath();

    void addSearchPath ( const char* path );
    const char* getSearchPath();
    int executeString (const char *code);
    int executeScriptFile (const char *filename);

    Table* getGlobalTable();

protected:
    void init();

protected:
    lua_State* m_pState;
    Table* m_pTableGlobal;
    char* m_pCurrentPath;
};

// lua helper



inline int LuaCheckParamCount(lua_State* env){
    return env != NULL ? lua_gettop (env) : 0;
}

#endif // SCRIPTENGINE_H
