cmake_minimum_required (VERSION 2.6)

PROJECT (Utils_src)

MESSAGE(STATUS "Cmake src...")

include( ${CMAKE_SOURCE_DIR}/project.cmake )

ADD_DEFINITIONS( -DENABLE_TESTCASE )

AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_LIBRARY ( Utils ${SRC_LIST})