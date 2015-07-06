#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <list>
#include <string>
#include <net/HttpC.h>
#include <map>

class AssetManager{
public:
    static AssetManager* getInstance();
private:
    AssetManager();
    ~AssetManager();
public:
    void addSearchPath (std::string str);
    const char* getCurrentFullPath();
    const std::string getFileFullPath(std::string abs_path);

    void addSearchWebPath(std::string url);
    bool isCached(std::string abs_path);
    const std::string getFileFullWebPath(std::string abs_path);
    const std::string getWebFile (std::string fullpath, bool auto_delete, std::string tag);

private:
    std::list<std::string> m_searchPaths;
    std::string m_pCurrentPath;

    std::list<std::string> m_searchWebPaths;
    std::string m_pCurrentWebPath;
    HttpC httpc;
    std::list<std::string> m_downloadFiles;

    // hash key = fullpath --> md5
    // hash< md5, fullpath>
    std::map<std::string, std::string> m_downloadFilesHashMap;
};

#endif // ASSETMANAGER_H

