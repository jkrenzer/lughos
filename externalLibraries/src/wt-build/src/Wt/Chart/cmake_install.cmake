# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Chart" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WChartPalette"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WChart3DImplementation"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WChartGlobal"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAbstractDataSeries3D"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WCartesian3DChart"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WLegend"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAbstractChartImplementation"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAxis3D"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAbstractGridData"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WStandardColorMap"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WStandardPalette"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WEquidistantGridData"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WPieChart"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAxis"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WDataSeries"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WLegend3D"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WGridData"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WCartesianChart"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WSelection"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WChart2DImplementation"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAbstractChart"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WScatterData"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Chart/WAbstractColorMap"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

