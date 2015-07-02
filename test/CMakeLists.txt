cmake_minimum_required (VERSION 2.6)

PROJECT (Utils_test)

MESSAGE(STATUS "Cmake test...")

include( ${CMAKE_SOURCE_DIR}/project.cmake )

# defines
ADD_DEFINITIONS(-DDEBUG -DENABLE_TESTCASE)
ADD_DEFINITIONS(-DREVERSION=0.1)

include( ${CMAKE_SOURCE_DIR}/module.cmake )
AUX_SOURCE_DIRECTORY(. SRC_LIST)

# recompile all cpp files, static init
# AUX_SOURCE_DIRECTORY(../src SRC_LIST)

ADD_EXECUTABLE(Utils.app ${SRC_LIST} ${LIB_LIST} )
target_link_libraries(Utils.app ${LIBRARY_OUTPUT_PATH}/libUtils.a)
target_link_libraries(Utils.app ${CMAKE_SOURCE_DIR}/lib/liblua-debug.a)
