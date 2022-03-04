cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

#### adding subdirs
add_subdirectory(./include/)
add_subdirectory(./tests/)

#### variables propagations
# GAME-MODELS-SRCS - propagated from ./include/game-models
# INTERACTORS-SRCS - propagated from ./include/interactors
# GAME-MODELS-TESTS-SRCS - propagated from ./tests/game-models


#### setting names for executables
set(GAME_MODELS_TESTS_TARGET game-models-tests)
set(SERVER_TARGET server)

#### setting global props
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



#### Protobuf | Need to run only once to generate protobuf cpp-files
# this set is required for Windows, if protobuf installed as vcpkg dependency
# set(Protobuf_PREFIX_PATH
#     "C:/dev/vcpkg/packages/protobuf_x64-windows")
# list(APPEND CMAKE_PREFIX_PATH "${Protobuf_PREFIX_PATH}")

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ./src/proto/player.proto)

message(NOTICE "Protoc compiler: " ${Protobuf_PROTOC_EXECUTABLE})
message(NOTICE "Bin dir: " ${CMAKE_CURRENT_BINARY_DIR})
message	(NOTICE "Proto Source: " ${PROTO_SRCS})
message(NOTICE "Proto Headers: " ${PROTO_HDRS})
message(NOTICE "Proto Libraries: " ${Protobuf_LIBRARIES})

#### compile options for all targets
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(
		/experimental:external /external:I ${Protobuf_INCLUDE_DIR}/google/protobuf /external:W0
		/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Werror)
endif()




#### configuring GAME_MODELS_TESTS_TARGET:
add_executable(${GAME_MODELS_TESTS_TARGET}
	src/doctest_main.cpp
	${GAME-MODELS-TESTS-SRCS}
	${GAME-MODELS-SRCS}
	${INTERACTORS-SRCS}

	${PROTO_SRCS}
	${PROTO_HDRS}
)

target_include_directories(${GAME_MODELS_TESTS_TARGET} PUBLIC 
	./doctest
	./include
	${Protobuf_INCLUDE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${GAME_MODELS_TESTS_TARGET} ${Protobuf_LIBRARIES})





#### configuring SERVER_TARGET:

# add_executable( ${SERVER_TARGET}
# 	src/server.cpp
# 	${PROTO_SRCS}
# 	${PROTO_HDRS}
# )

# # including Protobuf dirs to server target
# target_include_directories(${SERVER_TARGET} PUBLIC
# 	${Protobuf_INCLUDE_DIR} 
# 	${CMAKE_CURRENT_BINARY_DIR}
# )

# # including thread libs for Linux and Windows GCC Compiler
# if(WIN32 AND NOT MSVC AND NOT BORLAND)
# 	target_link_libraries(${SERVER_TARGET} 
# 		ws2_32 
# 		${Protobuf_LIBRARIES}
# 	)
# else()
# 	find_package(Threads REQUIRED)
# 	target_link_libraries(${SERVER_TARGET} PRIVATE
# 		Threads::Threads 
# 		${Protobuf_LIBRARIES}
# 	)
# endif()