#include "scope/Sigleton.h"


#ifdef ENABLE_TESTCASE

#include <iostream>
using namespace std;

#include <thread>
template <typename T>
T* SigletonPtr_NOT_SAFE(){
    static T* instance = nullptr;
    if (!instance){
        instance = new T();
    }
    return instance;
}
class A{
public:
    A(){
        cout << "A ctor" << endl;
    }
    ~A(){
        cout << "A dctor" << endl;
    }
};


#include <debug/testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("Sigleton", [](){
        cout << "Go Testing" << __FILE__ << endl;

        for(int i=0;i<100;i++){
            auto th1 = new std::thread([](){
                A& a = Sigleton<A>(); // safe, auto delete when app exit.
                SigletonPtr<A>(); // safe
//                SigletonPtr_NOT_SAFE<A>(); // not safe
                return 0;
            });
        }
        // delete by yourself
        A* a = SigletonPtr<A>();
        delete a;

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
