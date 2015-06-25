#include <AssetManager.h>
using namespace std;
#include <iostream>
#include <unistd.h>

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

const std::string& AssetManager::getFileFullPath (string abs_path){
    for( std::string& path : m_searchPaths){
        string realpath = path + abs_path;
        if (access(realpath.c_str (), R_OK) == F_OK ){
            return realpath;
        }
    }
    cout << "No file [" << abs_path << "] in:\n";
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

const std::string& AssetManager::getFileFullWebPath (string abs_path){
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
    if (tag.size () > 0){
        httpc.setWriteToFileTag (tag.c_str ());
    }
    httpc.setWriteToFile (true, auto_delete);
    httpc.get (fullpath);
    const char* path = httpc.getWritedFilePath ();

    if (path) return string(path);

    static std::string empty("");
    return empty;
}

#ifdef ENABLE_TESTCASE
#include <iostream>
using namespace std;
#include <testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("AssetManager", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        AssetManager* am = AssetManager::getInstance();
//        am->addSearchPath ("Asset/script/");
//        string path = am->getFileFullPath("main.lua");
//        cout << "1.full path:" << path << endl;

//        path = am->getFileFullPath("main.lua_notexist");
//        cout << "2.full path:" << path << endl;

        am->addSearchWebPath ("http://www.google.com/");
        string web;

        web = am->getFileFullWebPath ("img/bdlogo.png");
        cout << "1.full web path:" << web << endl;

        web = am->getFileFullWebPath ("images/srpr/logo11w.png");
        cout << "2.full web path:" << web << endl;

        if (web.size () > 0){
            string tmp_file = am->getWebFile(web, false, "logo11w.png");
            cout << "the temp file:" << tmp_file << endl;
        }


        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
