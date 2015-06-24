#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <testing.h>
#include <list>
#include <string>

class AssetManager{
private:

public:
    AssetManager();
    ~AssetManager();

    void addSearchPath(std::string str);

private:
    std::list<std::string> m_searchPaths;
};

#endif // ASSETMANAGER_H

