# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/examples/feature

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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/auth1/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/auth2/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/broadcast/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/client-ssl-auth/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/dbo/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/locale/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/mediaplayer/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/miniwebgl/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/multiple_servers/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/oauth/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/paypal/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/serverpush/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/socketnotifier/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/suggestionpopup/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/template-fun/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/video/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/widgetset/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

