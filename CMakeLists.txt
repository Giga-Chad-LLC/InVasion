cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

#### adding subdirs
add_subdirectory(./include/)
add_subdirectory(./tests/)
add_subdirectory(./include/libs/bcrypt)

#### variables propagations
# GAME-MODELS-SRCS       - propagated from ./include/game-models
# INTERACTORS-SRCS       - propagated from ./include/interactors
# CONTROLLERS-SRCS       - propagated from ./include/controllers
# GAME-MODELS-TESTS-SRCS - propagated from ./tests/game-models
# CONTROLLERS-TESTS-SRCS - propagated from ./tests/controllers
# GAME-SERVER-SRCS       - propagated from ./include/server
# UTILS-SRCS             - propagated from ./include/utils
# DATABASE-SRCS          - propagated from ./include/database
# HTTP-SERVER-SRCS       - propagated from ./include/http-server
# BCRYPT-SRCS			 - propagated from ./include/libs/bcrypt


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
		./src/proto/request-models/change-player-specialization-request-model.proto
		./src/proto/request-models/apply-ability-request-model.proto
		./src/proto/request-models/use-supply-request-model.proto
		./src/proto/request-models/weapon-direction-request-model.proto
		./src/proto/request-models/reload-weapon-request-model.proto
		./src/proto/request-models/client-credentials-request-model.proto

		# utils
        ./src/proto/utils/vector2d.proto
		./src/proto/utils/player-specialization.proto
		./src/proto/utils/player-team-id.proto
		./src/proto/utils/supply-model.proto
		./src/proto/utils/supply-type.proto
		./src/proto/utils/player-health-model.proto
		./src/proto/utils/player-data-model.proto
		./src/proto/utils/game-match-result.proto

		# response-models
        ./src/proto/response-models/player-position-response-model.proto
        ./src/proto/response-models/weapon-state-response-model.proto
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
		./src/proto/response-models/use-supply-response-model.proto
		./src/proto/response-models/handshake-response-model.proto
		./src/proto/response-models/update-player-ammo-response-model.proto
		./src/proto/response-models/update-player-hitpoints-response-model.proto
		./src/proto/response-models/weapon-direction-response-model.proto
		./src/proto/response-models/username-response-model.proto
		./src/proto/response-models/client-connected-response-model.proto
		./src/proto/response-models/client-disconnected-response-model.proto
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
		${BCRYPT-SRCS}
		# ./include/libs/bcrypt/src/bcrypt_impl.cpp
		# ./include/libs/bcrypt/src/blowfish.cpp
		${GAME-MODELS-SRCS}
		${INTERACTORS-SRCS}
		${CONTROLLERS-SRCS}
		${GAME-SERVER-SRCS}
		${UTILS-SRCS}
		${DATABASE-SRCS}
		${HTTP-SERVER-SRCS}

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
	find_package(unofficial-sqlite3 CONFIG REQUIRED)
	target_link_libraries(${SERVER_TARGET} PRIVATE
			# ws2_32
			unofficial::sqlite3::sqlite3
			${Protobuf_LIBRARIES}
			)
else ()
	find_package(Threads REQUIRED)
	target_link_libraries(${SERVER_TARGET} PRIVATE
			Threads::Threads
			#==== for http-server ======
			gcov
			sqlite3
			#==========
			${Protobuf_LIBRARIES}
			)
endif ()

