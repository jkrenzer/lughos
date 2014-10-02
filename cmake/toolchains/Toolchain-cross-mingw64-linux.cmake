# the name of the target operating system
include(CMakeForceCompiler)

#Load plattform settings
set(MINGW 1)
set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_CURRENT_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_FILE})
# Choose an appropriate compiler prefix

# for 32 or 64 bits mingw-w64
# see http://mingw-w64.sourceforge.net/
#set(COMPILER_PREFIX "i686-w64-mingw32")
set(COMPILER_PREFIX "x86_64-w64-mingw32")

# here is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/${COMPILER_PREFIX}/sys-root/mingw)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# which compilers to use for C and C++
find_program(CMAKE_RC_COMPILER NAMES ${COMPILER_PREFIX}-windres)
#set(CMAKE_RC_COMPILER ${COMPILER_PREFIX}-windres)

find_program(CMAKE_LINKER NAMES ${COMPILER_PREFIX}-ld)

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
#set(CMAKE_C_COMPILER ${COMPILER_PREFIX}-gcc)
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
find_program(CMAKE_CXX_COMPILER NAMES ${COMPILER_PREFIX}-g++)
#set(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}-g++)

