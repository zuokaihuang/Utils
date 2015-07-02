#ifdef ENABLE_TESTCASE

#include <asset/AssetManager.h>
#include <iostream>
using namespace std;
#include <debug/testing.h>


static int test_AssetManager(){
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
}

TC_Entry( AssetManager )

#endif // ENABLE_TESTCASE



