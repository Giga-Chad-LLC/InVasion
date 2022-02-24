cmake_minimum_required(VERSION 3.15)

project(invasion-server)

set(CMAKE_CXX_STANDARD 17)

# Protobuf
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# set(PROTO_SRCS "/usr/local/src/protobuf/protobuf/src")
# set(PROTO_HDRS "/usr/local/src/protobuf/protobuf/src")
# message  (NOTICE "Proto Source: " ${PROTO_SRCS})
# message(NOTICE "Proto Headers: " ${PROTO_HDRS})
# message(NOTICE "Proto Libraries: " ${Protobuf_LIBRARIES})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto/player.proto)

add_executable(
		server.out
		server.cpp
		${PROTO_SRCS}
		${PROTO_HDRS}
)

if(WIN32 AND NOT MSVC AND NOT BORLAND)
	target_link_libraries(main ws2_32 ${Protobuf_LIBRARIES})
else()
	find_package(Threads REQUIRED)
	target_link_libraries(server.out PRIVATE Threads::Threads ${Protobuf_LIBRARIES})
endif()

# add_compile_options(-Wall -Wextra -pedantic -Werror)