#ifndef GAMESCRIPTENGINE_H
#define GAMESCRIPTENGINE_H

#include "ScriptEngine.h"
#include <map>
#include <string>

class GameScriptEngine : public ScriptEngine
{
public:
    static GameScriptEngine* getInstance(){
        static GameScriptEngine gse;
        return &gse;
    }
    void init();
    void setBasepath(std::string& path);
    void addExecuteFile(const char* name,const char* filename);
    void pollExecuteFiles();
    void addSearchPath(const char *path);
private:
    GameScriptEngine();
    ~GameScriptEngine();

    void runFileFixedUpdate(const char* name);

    std::map<const char*, const char*>  m_executeFileMap;
    std::string                         m_basepath;
};

#endif // GAMESCRIPTENGINE_H
