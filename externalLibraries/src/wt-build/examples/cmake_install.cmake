# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/examples

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
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/blog/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/charts/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/codeview/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/composer/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/dialog/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/dragdrop/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/feature/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/filetreetable/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/form/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/gitmodel/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/hangman/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/hello/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/javascript/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/mandelbrot/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/mission/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/onethread/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/painting/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/planner/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/qrlogin/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/simplechat/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/style/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/treelist/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/treeview/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/treeview-dragdrop/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/webgl/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/wt-homepage/cmake_install.cmake")
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/examples/wtwithqt/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

