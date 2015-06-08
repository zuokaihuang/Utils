#ifndef SCOPE_H
#define SCOPE_H


#include <functional>
class ScopeGuard{
private:
    typedef std::function<void()> destructor_type;
    destructor_type _destructor;
    bool _dissmissed;
public:
    ScopeGuard(destructor_type dctor) : _destructor(dctor), _dissmissed(false){}
    ~ScopeGuard();
    void dismiss() { _dissmissed = true; }
    ScopeGuard(ScopeGuard const &) = delete;
    ScopeGuard& operator=(ScopeGuard const&) = delete;
};


#endif // SCOPE_H
