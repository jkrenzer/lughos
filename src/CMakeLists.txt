project(connectionimpl)

set(Boost_INCLUDE_DIR /usr/include/)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
find_package(Boost COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(connectionimpl test_tcp.cpp tcpConnections.cpp httpDict.cpp)
target_link_libraries( connectionimpl ${Boost_LIBRARIES} )
install(TARGETS connectionimpl RUNTIME DESTINATION bin)

