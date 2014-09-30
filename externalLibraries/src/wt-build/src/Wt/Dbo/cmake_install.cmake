# Install script for directory: /home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Dbo/libwtdbo.so.3.3.4"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Dbo/libwtdbo.so.38"
    "/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Dbo/libwtdbo.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so.3.3.4"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so.38"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwtdbo.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt" TYPE FILE FILES "/home/irina/projects/lughos/externalLibraries/src/wt-build/Wt/WConfig.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Dbo" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/WDboDllDefs.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/ptr_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Call_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlTraits_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/QueryModel_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/EscapeOStream.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Session_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/weak_ptr_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/DbAction_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/StringStream.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/collection_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Field_impl.h"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Query_impl.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Dbo" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/ptr_tuple"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Dbo"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlConnection"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/FixedSqlConnectionPool"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Query"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/DbAction"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/QueryModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Field"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/weak_ptr"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Json"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/ptr"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Session"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Types"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/WtSqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/QueryColumn"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Impl"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Call"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/StdSqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlConnectionPool"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlStatement"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Transaction"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Exception"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/collection"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Wt/Dbo" TYPE FILE FILES
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/ptr_tuple"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Dbo"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlConnection"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/FixedSqlConnectionPool"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Query"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/DbAction"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/QueryModel"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Field"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/weak_ptr"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Json"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/ptr"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Session"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Types"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/WtSqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/QueryColumn"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Impl"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Call"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/StdSqlTraits"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlConnectionPool"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/SqlStatement"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Transaction"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/Exception"
    "/home/irina/projects/lughos/externalLibraries/src/wt/src/Wt/Dbo/collection"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/irina/projects/lughos/externalLibraries/src/wt-build/src/Wt/Dbo/backend/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

