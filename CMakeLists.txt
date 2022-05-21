cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

#### adding subdirs
add_subdirectory(./include/)
add_subdirectory(./tests/)

#### variables propagations
# GAME-MODELS-SRCS - propagated from ./include/game-models
# INTERACTORS-SRCS - propagated from ./include/interactors
# CONTROLLERS-SRCS - propagated from ./include/controllers
# GAME-MODELS-TESTS-SRCS - propagated from ./tests/game-models
# CONTROLLERS-TESTS-SRCS - propagated from ./tests/controllers
# GAME-SERVER-SRCS - propagated from ./include/server
# UTILS-SRCS - propagated from ./include/utils


#### setting names for executables
set(TESTS_TARGET tests)
set(SERVER_TARGET server)

#### setting global props
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#### Protobuf
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
		# request-models
        ./src/proto/request-models/move-request-model.proto
        ./src/proto/request-models/update-game-state-request-model.proto
		./src/proto/request-models/rotate-weapon-request-model.proto
		./src/proto/request-models/shoot-request-model.proto
		./src/proto/request-models/respawn-player-request-model.proto
		./src/proto/request-models/game-session-ended-request-model.proto
		./src/proto/request-models/select-player-specialization-request-model.proto
		./src/proto/request-models/change-player-specialization-request-model.proto
		./src/proto/request-models/apply-ability-request-model.proto
		
		# utils
        ./src/proto/utils/vector2d.proto
		./src/proto/utils/player-specialization.proto
		./src/proto/utils/player-team-id.proto
		./src/proto/utils/supply-model.proto
		./src/proto/utils/supply-type.proto

		# response-models
        ./src/proto/response-models/player-info-response-model.proto
        ./src/proto/response-models/player-position-response-model.proto
        ./src/proto/response-models/shooting-state-response-model.proto
		./src/proto/response-models/bullet-position-response-model.proto
		./src/proto/response-models/game-state-response-model.proto
		./src/proto/response-models/damaged-player-response-model.proto
		./src/proto/response-models/killed-player-response-model.proto
		./src/proto/response-models/respawn-player-response-model.proto
		./src/proto/response-models/game-session-stats-response-model.proto
		./src/proto/response-models/player-game-session-stats-response-model.proto
		./src/proto/response-models/game-over-response-model.proto
		./src/proto/response-models/player-specialization-response-model.proto
		./src/proto/response-models/supply-response-model.proto
		)

message(NOTICE "Protoc compiler: " ${Protobuf_PROTOC_EXECUTABLE})
message(NOTICE "Bin dir: " ${CMAKE_CURRENT_BINARY_DIR})
message(NOTICE "Proto Source: " ${PROTO_SRCS})
message(NOTICE "Proto Headers: " ${PROTO_HDRS})
message(NOTICE "Proto Libraries: " ${Protobuf_LIBRARIES})

#### compile options for all targets
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(
            /experimental:external /external:I ${Protobuf_INCLUDE_DIR}/google/protobuf /external:W0)
    # /W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    # add_compile_options(-Wall -Wextra -Werror)
endif ()


#### configuring TESTS_TARGET:
add_executable(${TESTS_TARGET}
        src/doctest_main.cpp
        ${GAME-MODELS-TESTS-SRCS}
		${CONTROLLERS-TESTS-SRCS}
        ${GAME-MODELS-SRCS}
        ${INTERACTORS-SRCS}
        ${CONTROLLERS-SRCS}
		${UTILS-SRCS}

        ${PROTO_SRCS}
        ${PROTO_HDRS}
        )

target_include_directories(${TESTS_TARGET} PUBLIC
        ./doctest
        ./include
        ${Protobuf_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

if (WIN32 AND NOT MSVC AND NOT BORLAND)
    target_link_libraries(${TESTS_TARGET} ws2_32 ${Protobuf_LIBRARIES})
else ()
    find_package(Threads REQUIRED)
    target_link_libraries(${TESTS_TARGET} PRIVATE Threads::Threads ${Protobuf_LIBRARIES})
endif ()


#### configuring SERVER_TARGET:

add_executable(${SERVER_TARGET}
        src/main.cpp
        ${GAME-MODELS-SRCS}
        ${INTERACTORS-SRCS}
        ${CONTROLLERS-SRCS}
        ${GAME-SERVER-SRCS}
		${UTILS-SRCS}

		${PROTO_SRCS}
        ${PROTO_HDRS}
        )

# including Protobuf dirs to server target
target_include_directories(${SERVER_TARGET} PUBLIC
        ./doctest
        ./include
        ${Protobuf_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

# including thread libs for Linux and Windows GCC Compiler
if (MSVC) # WIN32 AND NOT MSVC AND NOT BORLAND
    target_compile_definitions(${SERVER_TARGET} PRIVATE -D_WIN32_WINNT=0x0A00)
    target_link_libraries(${SERVER_TARGET} PRIVATE
            # ws2_32
            ${Protobuf_LIBRARIES}
            )
else ()
    find_package(Threads REQUIRED)
    target_link_libraries(${SERVER_TARGET} PRIVATE
            Threads::Threads
            ${Protobuf_LIBRARIES}
            )
endif ()
