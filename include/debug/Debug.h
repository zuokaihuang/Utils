#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

// log to stdout/stderr
static std::ostream& print(std::ostream& os){
    return os;
}

template <typename T>
std::ostream& print(std::ostream& os, T&& t){
    return os << std::forward<T>(t);
}

template <typename T, typename U, typename... Args>
std::ostream& print(std::ostream& os, T&& t, U&& u, Args&&... args){
    return print(print(os, std::forward<T>(t)) << ' ', std::forward<U>(u), std::forward<Args>(args)...  );
}

template <typename...Args>
std::ostream& __DLog(Args&&...args){
    return print(std::cout, std::forward<Args>(args)...) << std::endl;
}

template <typename...Args>
std::ostream& __ELog(Args&&...args){
    print(std::cerr, std::forward<Args>(args)...) << std::endl;
}

// log to file
std::mutex& get_log_file_mutex();
template <typename T, typename U, typename... Args>
std::ostream& fd_print(std::ostream& os, T&& t, U&& u, Args&&... args){
    return print( print(os, std::forward<T>(t)) << ' ', std::forward<U>(u), std::forward<Args>(args)...);

}
#include <fstream>
template <typename...Args>
void _FLog(std::ostream& os, Args&&...args){
    std::lock_guard<std::mutex> _l( get_log_file_mutex () );
    fd_print( os, std::forward<Args>(args)...) << std::endl;
}

void DebugLogToFileOpen(const std::string filename);
std::ostream& DebugGetOutPutLogFile();
void DebugLogToFileClose();


#define DLog(...) __DLog(__VA_ARGS__)
#define ELog(...) __ELog(__VA_ARGS__)
#define FLog(...) _FLog(DebugGetOutPutLogFile(), __VA_ARGS__)


#endif // DEBUG_H
