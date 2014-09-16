project(lugos)
cmake_minimum_required(VERSION 2.8)
set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externalLibraries) #Path relative to build-dir!
set(PROJECT_NAME exposer)
set(PROJECT_BRIEF_DESCRIPTION "Library for exposing of classes to various interfaces")
#set(PROJECT_LOGO)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(DOCUMENTATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(GENERATOR "Eclipse CDT4 - Unix Makefiles")
list( APPEND CMAKE_CXX_FLAGS "-std=c++11")
include(ExternalProject)

# Generate Version-Information from git tag and commit-information
if( EXISTS "${CMAKE_SOURCE_DIR}/.git" )
  find_package(Git)
  if(GIT_FOUND)
    execute_process(COMMAND git describe --abbrev=4 HEAD
		  OUTPUT_VARIABLE PROJECT_RAW_VERSION
		  OUTPUT_STRIP_TRAILING_WHITESPACE)
    # remove leading "v"
    string(REGEX REPLACE "0-(.*)(-.*)" "\\1" PROJECT_VERSION "${PROJECT_RAW_VERSION}")
    string(REGEX REPLACE "(.*)0-(.*-)(.*)" "\\3" PROJECT_REVISION "${PROJECT_RAW_VERSION}")
    else( EXISTS "${CMAKE_SOURCE_DIR}/.git" )
    set(PROJECT_VERSION 0)
    message("Unable to obtain version-information from git. Setting version to 0.")
  endif(GIT_FOUND)
endif( EXISTS "${CMAKE_SOURCE_DIR}/.git" )
# Generate full custom version-string
set(PROJECT_FULL_VERSION "Version: ${PROJECT_VERSION} Rev.: ${PROJECT_REVISION}")
#find_package(Boost REQUIRED)
add_executable(exposer src/test_exposer.cpp)

install(TARGETS exposer RUNTIME DESTINATION bin)
