cmake_minimum_required (VERSION 2.6)

PROJECT (Utils)

MESSAGE(STATUS "Building " ${CMAKE_SOURCE_DIR} )

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

### reg all cpp files
SET(SRC_LIST main.cpp testing.cpp Buffer.cpp Debug.cpp MemoryPool.cpp MessageQueue.cpp SafeQueue.cpp Scope.cpp Sigleton.cpp Tree.cpp)

# defines
ADD_DEFINITIONS(-DDEBUG -DENABLE_TESTCASE)
ADD_DEFINITIONS(-DREVERSION=0.1)

# link flags
set(CMAKE_MODULE_LINKER_FLAGS, "-rdynamic")


ADD_EXECUTABLE(Utils.app ${SRC_LIST})
