cmake_minimum_required(VERSION 3.5.1)

project(InVasion)

#include(cmake-build-debug/common.cmake)

# Proto file
get_filename_component(hw_proto "../proto/player.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)
include_directories(${Protobuf_INCLUDE_DIRS})
# Generated sources
set(hw_proto_srcs "../proto/player.pb.cc")
set(hw_proto_hdrs "../proto/player.pb.h")
#set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
#set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(hw_grpc_proto

        ${hw_proto_srcs}
        ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
        ${_REFLECTION}

        ${_PROTOBUF_LIBPROTOBUF})


add_executable(server "../server_src/Server.cpp")
target_link_libraries(server hw_grpc_proto
        ${_REFLECTION}
#        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

#add_executable(server "server/server.cc")
#target_link_libraries(server hw_grpc_proto
#        ${_REFLECTION}
#        ${_GRPC_GRPCPP}
#        ${_PROTOBUF_LIBPROTOBUF})