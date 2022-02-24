cmake_minimum_required(VERSION 3.15)

project(invasion-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(
	server.out
	server.cpp
)

if(WIN32 AND NOT MSVC AND NOT BORLAND)
	target_link_libraries(main ws2_32 ${Protobuf_LIBRARIES})
else()
	find_package(Threads REQUIRED)
	target_link_libraries(server.out PRIVATE Threads::Threads)
endif()
