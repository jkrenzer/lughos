# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Ext/libwtext.so.3.3.4"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Ext/libwtext.so.38"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Ext/libwtext.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtext.so"
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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Ext" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/ComboBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Widget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/DataStore"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/SplitterHandle"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Button"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/AbstractToggleButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/NumberField"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/LineEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/ProgressDialog"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/ToolBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/AbstractButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/PagingToolBar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/RadioButton"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Menu"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/MenuItem"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Container"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/FormField"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/WExtGlobal"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/TabWidget"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/MessageBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Calendar"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/TableView"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Splitter"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/ToolTipConfig"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/DateField"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/CMakeLists.txt"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/TextEdit"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Panel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/CheckBox"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Component"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/Dialog"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Ext" TYPE FILE FILES "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Ext/ExtDllDefs.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

