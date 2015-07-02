#include "debug/testing.h"

#include <dlfcn.h>
#include <iostream>
using namespace std;

TestCase::TestCase()
{
//    cout << __func__ << "=>" << this << endl;
}

TestCase::~TestCase(){
    for( auto item: mTestFunctions){
        TestItem* testItem = item.second;
        delete testItem;
    }
}

void TestCase::addTestCase (string name, TestFunction function, void *arg){
    auto item = new TestItem{function};

    mTestFunctions.insert ( std::make_pair(name, item));
}

void TestCase::runAllTestCase(){
    for( auto item: mTestFunctions){
        TestItem* testItem = item.second;
        if (testItem && testItem->func){
            testItem->func();
        }
    }
}

void TestCase::runTestCase (string name){
    auto item = mTestFunctions.find (name);
    if (item != mTestFunctions.end ()){
        TestItem* i = item->second;
        i->func();
        DLog("Normal");
    }else{
        cout << "TestCase not found [" << name << "]" << endl;
        void* handle = dlopen (NULL, RTLD_LAZY);
        if (!handle){
            ELog("dlopen error");
            return;
        }
        char buffer[512];
        snprintf(buffer, 512, "tc%s", name.c_str ());
DLog("dlopen");
        typedef void(*Fptr)(void*);
        Fptr fptr = (Fptr)dlsym (handle, buffer);
        DLog( (void*)fptr );
        if (!fptr){
        }else{
            fptr( NULL );
        }
    }
}

void TestCase::dump (){
    cout << "TestCase Dump:\n"
         << "Total Case:" << mTestFunctions.size () << "\n"
         << "\n"
         << endl;

    auto item = mTestFunctions.begin ();
    while( item != mTestFunctions.end () ){
        auto name = item->first;
        cout << "=>" << name << endl;

        item++;
    }

}


