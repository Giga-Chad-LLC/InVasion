cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

#### adding subdirs
add_subdirectory(./include/)
add_subdirectory(./tests/)

#### variables propagations
# GAME-MODEL-SRCS - propagated from ./include/game-models
# INTERACTORS-SRCS - propagated from ./include/interactors
# GAME-MODELS-TESTS-SRCS - propagated from ./tests/game-models


#### setting names for executables
set(GAME_TARGET game)
set(GAME_MODELS_TESTS_TARGET game-models-tests)
set(SERVER_TARGET server)





#### Protobuf | Need to run only once to generate protobuf cpp-files

# this set is required for Windows, if protobuf installed as vcpkg dependency
set(Protobuf_PREFIX_PATH
    "C:/dev/vcpkg/packages/protobuf_x64-windows")
list(APPEND CMAKE_PREFIX_PATH "${Protobuf_PREFIX_PATH}")

find_package(Protobuf REQUIRED)

message(NOTICE "Protoc compiler: " ${Protobuf_PROTOC_EXECUTABLE})
message(NOTICE "Bin dir: " ${CMAKE_CURRENT_BINARY_DIR})
message	(NOTICE "Proto Source: " ${PROTO_SRCS})
message(NOTICE "Proto Headers: " ${PROTO_HDRS})
message(NOTICE "Proto Libraries: " ${Protobuf_LIBRARIES})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ./src/proto/player.proto)






#### configuring GAME_TARGET:

# add_executable(${GAME_TARGET} 
# 	src/game.cpp 
# 	${GAME-MODEL-SRCS}
# )

# target_include_directories(${GAME_TARGET} PUBLIC 
# 	./include
# )

# target_compile_options(${GAME_TARGET} PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror>
# )

# set_target_properties(${GAME_TARGET} PROPERTIES
# 	CXX_STANDARD 17
#   CXX_STANDARD_REQUIRED ON
# )






#### configuring GAME_MODELS_TESTS_TARGET:

message(SRCs ${PROTO_SRCS})
message(HDRs ${PROTO_HDRS})


add_executable(${GAME_MODELS_TESTS_TARGET}
	src/doctest_main.cpp
	${GAME-MODELS-TESTS-SRCS}
	${GAME-MODEL-SRCS}
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

target_compile_options(${GAME_MODELS_TESTS_TARGET} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror>
)

set_target_properties(${GAME_MODELS_TESTS_TARGET} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)





#### configuring SERVER_TARGET:

# add_executable( ${SERVER_TARGET}
# 	src/server.cpp
	# ${PROTO_SRCS}
	# ${PROTO_HDRS}
# )

# # including Protobuf dirs to server target
# target_include_directories(${SERVER_TARGET} PUBLIC
	# ${Protobuf_INCLUDE_DIR} 
	# ${CMAKE_CURRENT_BINARY_DIR}
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