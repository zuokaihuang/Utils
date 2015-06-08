cmake_minimum_required (VERSION 2.6)

PROJECT (Utils)

MESSAGE(STATUS "Building " ${CMAKE_SOURCE_DIR} )

include( ${CMAKE_SOURCE_DIR}/project.cmake )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( test )
