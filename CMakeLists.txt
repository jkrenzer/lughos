### Toolchain-switch for cross-compiling
# For cross-compiling issue at commandline: cmake -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchain-cross-*.cmake
# Make sure to have alls needed tools installed beforehand!

### Include modules





### Project description

project(lugos)
cmake_minimum_required(VERSION 2.8.12)

set(PROJECT_NAME lughos)
set(PROJECT_BRIEF_DESCRIPTION "Library for exposing of classes to various interfaces")
set(DOCUMENTATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(CMAKE_TOOLCHAIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchains)
#set(PROJECT_LOGO)

### Configuration switches

set(LINK_STATIC FALSE) # set TRUE to link libraries static, so we get a stand-alone application
set(BUILD_SHARED_LIBS TRUE)

###

add_compile_options(-std=c++11) #Enable C++11 support

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT MINGW)
 message("Enabling GCC Debug-Flags")
 add_compile_options(-g -O0) #Enable debugging-output, no-optimization (for valgrind and gdb)
 set(CMAKE_VERBOSE_MAKEFILE ON)
 elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND MINGW)
 message("Enabling GCC Debug-Flags with DWARF output")
 add_compile_options(-gdwarf-4 -O0)
 set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# What to do when crosscompiling
if(${CMAKE_CROSSCOMPILING})
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_${CMAKE_SYSTEM_NAME})
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_${CMAKE_SYSTEM_NAME})
  set(GENERATOR "MinGW Makefiles")
  list(APPEND INCLUDE_DIRS ${CMAKE_FIND_ROOT_PATH}/include)
else()
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
  set(GENERATOR "Unix Makefiles")
endif()

# What to do if we happen to crosscompile with MINGW
if(MINGW)
message("== Compiling with MINGW ==")
list(APPEND LINK_DIRS /usr/${COMPILER_PREFIX}/lib/ /usr/${COMPILER_PREFIX}/sys-root/mingw/lib/)
list(APPEND INCLUDE_DIRS /usr/${COMPILER_PREFIX}/include/ /lib/${COMPILER_PREFIX}/sys-root/mingw/include/)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -static -static-libstdc++ -static-libgcc -v")
set(Boost_USE_STATIC_LIBS ON)
endif()

#list(APPEND LINK_DIRS ${EXT_LIB_DIR}/lib/)
#list(APPEND INCLUDE_DIRS ${EXT_LIB_DIR}/include/)

# What do do if we are compiling for WINDOwS
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
message("== Compiling for Windows ==")
set(Windows TRUE)
set(WIN32 TRUE)
add_definitions(-DWIN32) #Set c/c++ preprocessor var
endif()

# What do do if we are compiling for LINUX
if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
message("== Compiling for Linux ==")
set(Linux TRUE)
add_definitions(-DLINUX) #Set c/c++ preprocessor var
endif()

# What do do if we are compiling for OSX
if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
message("== Compiling for MacOS X ==")
set(Darwin TRUE)
add_definitions(-DOSX) #Set c/c++ preprocessor var
endif()

# What to do if user requests static linking

# if(${LINK_STATIC})
# list(APPEND LINK_DIRS ${CMAKE_FIND_ROOT_PATH}/lib)
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -static -static-libstdc++ -static-libgcc")

# else()
# list(APPEND LINK_DIRS ${CMAKE_FIND_ROOT_PATH}/bin)
# endif()

### Configure find system

# findBoost
if(${CMAKE_CROSSCOMPILING})
set(Boost_DEBUG "ON")
# set(BOOST_ROOT ${CMAKE_FIND_ROOT_PATH})
# set(BOOST_LIBRARYDIR "${BOOST_ROOT}lib/")
set(BOOST_ALL_DYN_LINK TRUE)
set(Boost_DETAILED_FAILURE_MSG "ON")
else()
set(Boost_DEBUG "ON")
set(BOOST_INCLUDEDIR "/usr/include/boost" "/usr/local/include/boost")
endif()


### Find needed Packages

find_package(Boost COMPONENTS system filesystem thread program_options regex date_time timer random log chrono REQUIRED)
find_package(Doxygen)
find_package(Libharu)
find_package(Wt)



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


### Set include/link directories

if(${Wt_FOUND})
include_directories(${INCLUDE_DIRS} ${Wt_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${LINK_DIRS} ${Boost_LIBRARY_DIRS})
endif()

### Printing some information

message("-= CMAKE FOR LUGHOS SCADA LIBRARY =-")
if(${CMAKE_CROSSCOMPILING})
message("cmake is CROSSCOMPILING!")
message("current toolchain-file: ${CMAKE_CURRENT_TOOLCHAIN_FILE}")
endif()
message("EXT_LIB_DIR: ${EXT_LIB_DIR}")
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_IMPORT_LIBRARY_PREFIX: ${CMAKE_IMPORT_LIBRARY_PREFIX}")
message("CMAKE_IMPORT_LIBRARY_SUFFIX: ${CMAKE_IMPORT_LIBRARY_SUFFIX}")
message("CMAKE_LINK_LIBRARY_SUFFIX: ${CMAKE_LINK_LIBRARY_SUFFIX}")
message("CMAKE_FIND_LIBRARY_SUFFIXES: ${CMAKE_FIND_LIBRARY_SUFFIXES}")
message("CMAKE_FIND_LIBRARY_PREFIXES: ${CMAKE_FIND_LIBRARY_PREFIXES}")
message("CMAKE_LINKER: ${CMAKE_LINKER}")
message("CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}")
message("MINGW: ${MINGW}")
message("INCLUDE_DIRS: ${INCLUDE_DIRS}")
message("LINK_DIRS: ${LINK_DIRS}")
message("LINK_STATIC: ${LINK_STATIC}")


### Providing target to build dependencies

add_subdirectory(dependencies/ dependencies/ EXCLUDE_FROM_ALL)

### Set libraries and dependencies

add_library(exposedValues src/exposedValues.cpp)

add_library(errorHandling src/errorHandling.cpp)

add_library(log src/log.cpp)
target_link_libraries(log ${Boost_LOG_LIBRARY_DEBUG} boost_log_setup)

add_library(connections src/serialConnections.cpp src/tcpConnections.cpp)

add_library(keithley src/keithley.cpp)
target_link_libraries(keithley  measuredValue connections errorHandling log)
add_dependencies(keithley  measuredValue connections)

add_library(keithleySerial src/keithleySerial.cpp)
target_link_libraries(keithleySerial  measuredValue connections errorHandling log)
add_dependencies(keithleySerial  measuredValue connections errorHandling log)

add_library(MaxiGauge src/MaxiGauge.cpp)
target_link_libraries(MaxiGauge  measuredValue unitValue connections errorHandling log)
add_dependencies(MaxiGauge  measuredValue connections errorHandling log)

add_library(coolpak6000 src/coolpak6000.cpp)
target_link_libraries(coolpak6000  unitValue connections errorHandling log)
add_dependencies(coolpak6000  unitValue connections errorHandling log)

add_library(bronkhorst src/bronkhorst.cpp)
target_link_libraries(bronkhorst  unitValue connections errorHandling log)
add_dependencies(bronkhorst  unitValue connections errorHandling log)

add_library(PSAPowersupply src/PSAPowersupply.cpp)
target_link_libraries(PSAPowersupply  unitValue connections errorHandling log)
add_dependencies(PSAPowersupply  unitValue connections errorHandling log)

add_library(Relais src/Relais.cpp)
target_link_libraries(Relais  unitValue connections errorHandling log)
add_dependencies(Relais  unitValue connections errorHandling log)

add_library(FUGNetzteil src/FUGNetzteil.cpp)
target_link_libraries(FUGNetzteil  unitValue connections errorHandling log)
add_dependencies(FUGNetzteil  unitValue connections errorHandling log)

add_library(RFG src/RFG.cpp)
target_link_libraries(RFG  unitValue connections errorHandling log ${Wt_DBO_LIBRARY})
add_dependencies(RFG  unitValue connections errorHandling log)

add_library(unitValue src/unitValue.cpp)

add_library(measuredValue src/measuredValue.cpp)
add_dependencies(measuredValue unitValue)



### Set executables and dependencies
#Test for regexp
add_executable(test_regex src/test_regex.cpp)
target_link_libraries(test_regex boost_regex)

# #Test for exposer
add_executable(test_ioservice src/test_ioservice.cpp)
target_link_libraries(test_ioservice ${Boost_LIBRARIES})


# #Test for exposer
add_executable(test_exposer src/test_exposer.cpp)
target_link_libraries(test_exposer errorHandling ${Boost_LIBRARIES} exposedValues measuredValue unitValue)
 
#Test for wt-dbo
add_executable(test_wtdbo src/test_wtdbo.cpp)
target_link_libraries(test_wtdbo ${EXT_LIB_DIR}/lib/libwtdbo.so ${EXT_LIB_DIR}/lib/libwtdbosqlite3.so)
 
### Test for wt
# Add executbales and libraries if dependencies are found
add_executable(test_gui src/test_gui.cpp)#  src/coolpak6000.cpp src/serialConnections.cpp src/serialSync.cpp src/.cpp src/unitValue.cpp)
add_dependencies(test_gui coolpak6000 MaxiGauge)

if(${Windows})
target_link_libraries(test_gui coolpak6000 MaxiGauge boost_chrono keithleySerial ${Wt_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} ${Boost_LIBRARIES} ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} ws2_32 mswsock mingw32)
else()
target_link_libraries(test_gui coolpak6000 MaxiGauge boost_chrono keithleySerial ${Wt_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} ${Boost_LIBRARIES}  ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} boost_signals)
endif()

### Test for wt2
# Add executbales and libraries if dependencies are found
add_executable(test_gui2 src/test_gui2.cpp)#  src/coolpak6000.cpp src/serialConnections.cpp src/serialSync.cpp src/.cpp src/unitValue.cpp)
add_dependencies(test_gui2 bronkhorst Relais RFG FUGNetzteil PSAPowersupply connections)

if(${Windows})
target_link_libraries(test_gui2 bronkhorst Relais RFG FUGNetzteil PSAPowersupply measuredValue boost_chrono ${Boost_log_LIBRARY_DEBUG} connections ${Wt_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} ${Boost_LIBRARIES} ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} ws2_32 mswsock mingw32 gsl gslcblas)
else()
target_link_libraries(test_gui2 bronkhorst Relais RFG FUGNetzteil PSAPowersupply measuredValue boost_chrono ${Boost_log_LIBRARY_DEBUG} connections ${Wt_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} ${Boost_LIBRARIES}  ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} boost_signals pthread gsl gslcblas)
endif()

### Test for wt-plots
# Add executbales and libraries if dependencies are found
add_executable(test_plots src/test_plots.cpp)#  src/coolpak6000.cpp src/serialConnections.cpp src/serialSync.cpp src/.cpp src/unitValue.cpp)
target_link_libraries(test_plots wtdbo wtdbosqlite3 wt wthttp ${Boost_LIBRARIES} )

### Calibrator for RFG
add_executable(RFGCalibrator src/RFG_calibrator.cpp)
target_link_libraries(RFGCalibrator RFG keithley boost_chrono ${Wt_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} ${Boost_LIBRARIES}  ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} ${Boost_LIBRARIES} pthread gsl gslcblas)
add_dependencies(RFGCalibrator keithley RFG)

### Test for forms
add_executable(test_forms src/test_forms.cpp)
add_dependencies(test_forms wt)
target_link_libraries(test_forms ${EXT_LIB_DIR}/lib/libwt.so ${EXT_LIB_DIR}/lib/libwthttp.so)

### Test of connections
# add_executable(test_connections src/test_serial.cpp src/basicConnections.cpp)


if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
add_executable(test_serial src/test_serial.cpp src/serialConnections.cpp src/RFG.cpp src/keithleySerial.cpp src/.cpp src/coolpak6000.cpp src/MaxiGauge.cpp src/Relais.cpp src/PSANetzteil.cpp  src/unitValue.cpp)
target_link_libraries(test_serial ${Boost_LIBRARIES} libwsock32.a)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
add_executable(test_serial src/test_serial.cpp src/serialConnections.cpp  src/Relais.cpp src/unitValue.cpp)
target_link_libraries(test_serial ${Boost_LIBRARIES}  ${Wt_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} boost_chrono measuredValue)
endif()

### Test of data acquisition
add_executable(test_acquisition src/test_acquisition.cpp)
target_link_libraries(test_acquisition ${Boost_LIBRARIES} ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} ${Wt_DBO_LIBRARY} ${Wt_DBOSQLITE3_LIBRARY} unitValue measuredValue boost_signals)
#add_dependencies(test_acquisition )
 
### Test of TCP


if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
add_executable(test_tcp src/test_tcp.cpp src/tcpConnections.cpp src/httpDict.cpp src/Dict.cpp src/keithley.cpp  )
target_link_libraries(test_tcp ${Boost_LIBRARIES}  libwsock32.a)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
add_executable(test_tcp src/test_tcp.cpp src/tcpConnections.cpp src/httpDict.cpp src/Dict.cpp src/keithley.cpp  src/FUGNetzteil.cpp)
target_link_libraries(test_tcp ${Boost_LIBRARIES} unitValue measuredValue boost_chrono)
endif()

### Test for stringstreams
add_executable(test_stringstream src/test_stringstream.cpp)

### Test for interpolation
add_executable(test_interpolation src/test_interpolation.cpp)
target_link_libraries(test_interpolation gsl gslcblas)

### Test for parser
add_executable(test_parser src/test_parser.cpp)

### Test for ZMQ
add_executable(test_zmq src/test_zmq.cpp)
target_link_libraries(test_zmq zmq ${Boost_LIBRARIES})

#### Installation

### Installation directives

set(CMAKE_INSTALL_COMPONENT dependencies) # Generate dependencies

install(FILES ${EXT_LIB_DIR}/lib/* COMPONENT external_libs DESTINATION lib)
install(FILES ${EXT_LIB_DIR}/include/* COMPONENT external_includes DESTINATION include)

#install(TARGETS test_connections test_exposer test_forms test_gui test_tcp test_wtdbo COMPONENT tests RUNTIME DESTINATION bin)

set(CPACK_COMPONENTS_ALL tests)
set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Testprograms")
set(CPACK_COMPONENT_TESTS_DESCRIPTION "Various test-applications to test library components")
SET(CPACK_GENERATOR "")
set(CPACK_PACKAGE_NAME "LUGHOS")
set(CPACK_PACKAGE_VENDOR "LUGHOS community")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LUGHOS - An lbratory SCADA system")
set(CPACK_PACKAGE_VERSION "0.0.1RC1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "lughos")
# This must always be last!
 include(CPack)
