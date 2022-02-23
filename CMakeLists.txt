cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

# setting executables
set(MULTIPLAYER-SERVER-TARGET multiplayer-server)

add_executable("${MULTIPLAYER-SERVER-TARGET}"
	src/multiplayer-server.cpp
	game-models/Vector2D/vector2d.cpp
)


target_include_directories(${MULTIPLAYER-SERVER-TARGET} PUBLIC 
	game-models
)

target_compile_options(${MULTIPLAYER-SERVER-TARGET} PUBLIC
	-Wall 
	-Wextra 
	-Werror
)

set_target_properties(${MULTIPLAYER-SERVER-TARGET} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)