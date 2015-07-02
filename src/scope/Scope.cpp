#include "scope/Scope.h"


ScopeGuard::~ScopeGuard (){
    if ( !_dissmissed ){
        _destructor();
    }
}



