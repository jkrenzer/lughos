# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/irina/projects/lughos/externalLibraries")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so.8"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwttest.so.3.3.4"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwttest.so.8"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwttest.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so.8"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwttest.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_REMOVE
           FILE "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwt.so.3.3.4"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwt.so.38"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/libwt.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwt.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/WtInstall.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/web/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/isapi/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/fcgi/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/http/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

