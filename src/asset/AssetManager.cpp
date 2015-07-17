#include <asset/AssetManager.h>
using namespace std;
#include <iostream>
#include <unistd.h>

#include <hash/MD5Helper.h>
#include <debug/Debug.h>

AssetManager::AssetManager(){
    char current_folder[1024];
    getcwd (current_folder, 1024);

    if (strlen(current_folder) != 0){
        m_pCurrentPath = strdup(current_folder);
        m_pCurrentPath.append ("/");
        this->addSearchPath (m_pCurrentPath);
    }

}
AssetManager::~AssetManager (){
    for(std::string path : m_downloadFiles){
        if (access(path.c_str (), R_OK) == F_OK ){
            unlink (path.c_str ());
        }
    }
}

AssetManager* AssetManager::getInstance (){
    static AssetManager instance;
    return &instance;
}

void AssetManager::addSearchPath (std::string str){
    if ( str.c_str ()[0] != '/'){
        str = m_pCurrentPath + str;
    }
    m_searchPaths.push_front (str);
}

const std::string AssetManager::getFileFullPath (string abs_path){
    for( std::string path : m_searchPaths){
        string realpath = path + abs_path;

        if (access(realpath.c_str (), R_OK) == F_OK ){
            return realpath;
        }
    }
    cout << "File [" << abs_path << "] not in:\n";
    for( std::string& path : m_searchPaths){
        cout << path << "\n";
    }
    cout << endl;

    static std::string empty("");
    return empty;
}

const char* AssetManager::getCurrentFullPath (){
    return m_pCurrentPath.c_str ();
}

void AssetManager::addSearchWebPath(string url){
    if (url.size () < 7) return ;
    const char* _url = url.c_str ();

    if ( strncasecmp (_url, "http://", 7) == 0 ){
        m_searchWebPaths.push_back (url);
    }
}

bool AssetManager::isCached(std::string abs_path){
    char* md5 = MD5Helper::md5 ( abs_path.c_str () );
    auto it = m_downloadFilesHashMap.find ( string(md5) );
    if ( it != m_downloadFilesHashMap.end () ){
        return true;
    }
    return false;
}

const std::string AssetManager::getFileFullWebPath (string abs_path){
    for( std::string& path : m_searchWebPaths){
        string realpath = path + abs_path;
        httpc.head (realpath);
        if ( httpc.getStateCode () == 200){
            return realpath;
        }
    }
    cout << "No web file [" << abs_path << "] in:\n";
    for( std::string& path : m_searchWebPaths){
        cout << path << "\n";
    }
    cout << endl;

    static std::string empty("");
    return empty;
}

const std::string AssetManager::getWebFile (string fullpath, bool auto_delete, string tag){
    DAssert( fullpath.size () == 0, "the fullpath is empty");
    char* md5 = MD5Helper::md5 ( fullpath.c_str () );
    std::auto_ptr<char> sp(md5);
    auto it = m_downloadFilesHashMap.find ( string(md5) );
    if ( it != m_downloadFilesHashMap.end () ){
        cout << "found dowloaded:" << (*it).second << endl;
        return (*it).second;
    }

    if (tag.size () > 0){
        httpc.setWriteToFileTag (tag.c_str ());
    }

    httpc.setWriteToFile (true, auto_delete);
    httpc.get (fullpath);
    const char* path = httpc.getWritedFilePath ();

    if ( path ){
        m_downloadFilesHashMap.insert (std::make_pair( string(md5), path ));
        return string(path);
    }


    static std::string empty("");
    return empty;
}


