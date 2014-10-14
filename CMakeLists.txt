### Toolchain-switch for cross-compiling
# For cross-compiling issue at commandline: cmake -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchain-cross-*.cmake
# Make sure to have alls needed tools installed beforehand!

### Project description

project(lugos)
cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME lughos)
set(PROJECT_BRIEF_DESCRIPTION "Library for exposing of classes to various interfaces")
#set(PROJECT_LOGO)

if(${CMAKE_CROSSCOMPILING})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_crosscompile)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_crosscompile)
set(EXT_LIB_CROSSCOMPILE_CMAKE_OPT "")
set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externalLibraries_crosscompile) #Path relative to build-dir!
set(GENERATOR "MinGW Makefiles")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -static -static-libstdc++ -static-libgcc")
list(APPEND LINK_DIRS ${CMAKE_FIND_ROOT_PATH}/lib ${EXT_LIB_DIR}/lib/)
list(APPEND INCLUDE_DIRS ${CMAKE_FIND_ROOT_PATH}/include ${EXT_LIB_DIR}/include/)
else()
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externalLibraries) #Path relative to build-dir!
set(GENERATOR "Unix Makefiles")
list(APPEND LINK_DIRS ${EXT_LIB_DIR}/lib/)
list(APPEND INCLUDE_DIRS ${EXT_LIB_DIR}/include/)
endif()

include_directories(${INCLUDE_DIRS})
link_directories(${LINK_DIRS})

set(DOCUMENTATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(CMAKE_TOOLCHAIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchains)

list( APPEND CMAKE_CXX_FLAGS "-std=c++11") #Enable C++11 support
#list( APPEND CMAKE_CXX_FLAGS "-g -O0") #Enable debugging-output, no-optimization (for valgrind and gdb)
include(ExternalProject)

### Printing some information

message("-= CMAKE FOR LUGHOS SCADA LIBRARY =-")
if(${CMAKE_CROSSCOMPILING})
message("cmake is CROSSCOMPILING!")
message("current toolchain-file: ${CMAKE_CURRENT_TOOLCHAIN_FILE}")
endif()
message("CMAKE_IMPORT_LIBRARY_PREFIX: ${CMAKE_IMPORT_LIBRARY_PREFIX}")
message("CMAKE_IMPORT_LIBRARY_SUFFIX: ${CMAKE_IMPORT_LIBRARY_SUFFIX}")
message("CMAKE_LINK_LIBRARY_SUFFIX: ${CMAKE_LINK_LIBRARY_SUFFIX}")
message("CMAKE_FIND_LIBRARY_SUFFIXES: ${CMAKE_FIND_LIBRARY_SUFFIXES}")
message("CMAKE_FIND_LIBRARY_PREFIXES: ${CMAKE_FIND_LIBRARY_PREFIXES}")
message("CMAKE_LINKER: ${CMAKE_LINKER}")
message("MINGW: ${MINGW}")
message("INCLUDE_DIRS: ${INCLUDE_DIRS}")
message("LINK_DIRS: ${LINK_DIRS}")

### Find needed Packages
if(${CMAKE_CROSSCOMPILING})
set(Boost_DEBUG "ON")
set(BOOST_ROOT ${CMAKE_FIND_ROOT_PATH})
set(BOOST_LIBRARYDIR "${BOOST_ROOT}lib/")
set(Boost_DETAILED_FAILURE_MSG "ON")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(${CMAKE_LINK_LIBRARY_SUFFIX} .a)
else()
set(Boost_DEBUG "ON")
set(BOOST_INCLUDEDIR "/usr/include/boost")
endif()

find_package(Boost COMPONENTS system filesystem thread program_options regex date_time random REQUIRED)
find_package(Doxygen)



### Taget for generating Doxygen documentation (if Doygen is found)

if(DOXYGEN_FOUND)
 configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
 add_custom_target(doc
 ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
 #COMMAND ${CMAKE_MAKE_PROGRAM} -C ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex/proc/
 #COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex/proc/refman.pdf ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex/manual.pdf
 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
 COMMENT "Generating API documentation with Doxygen" VERBATIM
 )
endif(DOXYGEN_FOUND)

### Download and compile missing external libraries

if(LIBARU_FOUND)
add_custom_target(libharu COMMENT "libharu was sucessfully found. Skipping build.")
else()
ExternalProject_Add(
      libharu
      PREFIX ${EXT_LIB_DIR}
      GIT_REPOSITORY https://github.com/libharu/libharu.git #Comment-out when cmake keeps buggin you with rebuilds
      CMAKE_ARGS 
        -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_CURRENT_TOOLCHAIN_FILE}
	-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
	-DCONFIGDIR:STRING=<INSTALL_DIR>/etc
	-DCMAKE_FIND_LIBRARY_SUFFIXES:STRING=".a;.dll.a"

  )
endif()

if(WT_FOUND)
add_custom_target(libharu COMMENT "Wt was sucessfully found. Skipping build.")
else()
ExternalProject_Add(
    wt
    PREFIX ${EXT_LIB_DIR}
    GIT_REPOSITORY https://github.com/kdeforche/wt.git #Comment-out when cmake keeps buggin you with rebuilds
    CMAKE_ARGS 
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_CURRENT_TOOLCHAIN_FILE} 
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
      -DCONFIGDIR:STRING=<INSTALL_DIR>/etc  
      -DCONNECTOR_FCGI:BOOL=OFF 
      -DBoost_USE_STATIC_LIBS=ON
      -DBOOST_LIBRARYDIR:PATH=${CMAKE_FIND_ROOT_PATH}/lib
      -DBOOST_ROOT:PATH=${CMAKE_FIND_ROOT_PATH}
#       -DWT_WRASTERIMAGE_IMPLEMENTATION:STRING=GraphicsMagick
      -DHARU_PREFIX:STRING=${EXT_LIB_DIR}
      -DCMAKE_FIND_LIBRARY_SUFFIXES:STRING=".a;.dll.a"

 )
endif()

### Generate Version-Information from git tag and commit-information

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

### Set executables and dependencies

#Test for exposer
add_executable(test_exposer src/test_exposer.cpp)

#Test for wt-dbo
add_executable(test_wtdbo src/test_wtdbo.cpp)
target_link_libraries(test_wtdbo ${EXT_LIB_DIR}/lib/libwtdbo.so ${EXT_LIB_DIR}/lib/libwtdbosqlite3.so)

#Test for wt
find_library(LIBWT NAMES wt PATHS ${EXT_LIB_DIR}/lib CMAKE_FIND_ROOT_PATH_BOTH)
find_library(LIBWTHTTP NAMES wthttp PATHS ${EXT_LIB_DIR}/lib CMAKE_FIND_ROOT_PATH_BOTH)
find_library(LIBWS2_32 NAMES ws2_32 REQUIRED)
find_library(LIBMSWSOCK NAMES mswsock REQUIRED)
add_executable(test_gui src/test_gui.cpp)
target_link_libraries(test_gui ${LIBWT} ${LIBWTHTTP} ${Boost_LIBRARIES} ${LIBWS2_32} ${LIBMSWSOCK})

#Test for forms
add_executable(test_forms src/test_forms.cpp)
add_dependencies(test_forms wt)
target_link_libraries(test_forms ${EXT_LIB_DIR}/lib/libwt.so ${EXT_LIB_DIR}/lib/libwthttp.so)

#Test of connections
# add_executable(test_connections src/test_serial.cpp src/basicConnections.cpp)
add_executable(test_connections src/test_serial.cpp src/serialConnections.cpp src/serialSync.cpp src/kithleighSerial.cpp src/serialAsync.cpp src/coolpak6000.cpp src/MaxiGauge.cpp)
target_link_libraries(test_connections ${Boost_LIBRARIES} )

#Test of TCP
add_executable(test_tcp src/test_tcp.cpp src/tcpConnections.cpp src/httpDict.cpp)
target_link_libraries(test_tcp ${Boost_LIBRARIES} )



#Installation directives
#install(TARGETS lughos RUNTIME DESTINATION bin)
#install(TARGETS connectionimpl RUNTIME DESTINATION bin)