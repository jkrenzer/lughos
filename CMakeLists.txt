project(lugos)
cmake_minimum_required(VERSION 2.8)
set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externalLibraries) #Path relative to build-dir!
set(PROJECT_NAME lughos)
set(PROJECT_BRIEF_DESCRIPTION "Library for exposing of classes to various interfaces")
#set(PROJECT_LOGO)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(DOCUMENTATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(GENERATOR "Eclipse CDT4 - Unix Makefiles")
list( APPEND CMAKE_CXX_FLAGS "-std=c++11") #Enable C++11 support
#list( APPEND CMAKE_CXX_FLAGS "-g -O0") #Enable debugging-output, no-optimization (for valgrind and gdb)
include(ExternalProject)

### Find needed Packages

find_package(Boost COMPONENTS system filesystem thread regex REQUIRED)
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
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCONFIGDIR:STRING=<INSTALL_DIR>/etc 
  )
endif()

if(WT_FOUND)
add_custom_target(libharu COMMENT "Wt was sucessfully found. Skipping build.")
else()
ExternalProject_Add(
    wt
    PREFIX ${EXT_LIB_DIR}
    GIT_REPOSITORY https://github.com/kdeforche/wt.git #Comment-out when cmake keeps buggin you with rebuilds
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCONFIGDIR:STRING=<INSTALL_DIR>/etc  -DCONNECTOR_FCGI=OFF
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

#Test for wt
add_executable(test_wtdbo src/test_wtdbo.cpp)
target_link_libraries(test_wtdbo ${EXT_LIB_DIR}/lib/libwtdbo.so ${EXT_LIB_DIR}/lib/libwtdbosqlite3.so)

#Test for forms
add_executable(test_forms src/test_forms.cpp)
add_dependencies(test_forms wt)
include_directories(${EXT_LIB_DIR}/include/)
link_directories(${EXT_LIB_DIR}/lib/)
target_link_libraries(test_forms ${EXT_LIB_DIR}/lib/libwt.so ${EXT_LIB_DIR}/lib/libwthttp.so)

#Test of connections
# add_executable(test_connections src/test_serial.cpp src/basicConnections.cpp)
add_executable(test_connections src/test_serial.cpp src/serialConnections.cpp src/serialSync.cpp src/kithleighSerial.cpp src/serialAsync.cpp src/coolpak6000.cpp src/MaxiGauge.cpp)
target_link_libraries(test_connections ${Boost_LIBRARIES} )

#Installation directives
#install(TARGETS lughos RUNTIME DESTINATION bin)
