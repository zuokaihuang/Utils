#include <AssetManager.h>

AssetManager::AssetManager(){}
AssetManager::~AssetManager (){}

void AssetManager::addSearchPath (std::string str){
    m_searchPaths.push_back (str);
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

        AssetManager* am = SigletonPtr<AssetManager>();
        am->addSearchPath ("/tmp");

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
