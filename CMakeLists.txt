cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

# adding subdirs
add_subdirectory(./include/)

# setting names for executables
set(GAME-TARGET game)


# configuring GAME-TARGET:

add_executable(${GAME-TARGET}
	./src/game.cpp
	${GAME-MODEL-SRCS}
	# ./include/game-models/Vector2D/vector2d.cpp
	# ./include/game-models/BoxCollider/box-collider.cpp
	# ./include/game-models/Object/object.cpp
	# ./include/game-models/KinematicObject/kinematic-object.cpp
)

target_include_directories(${GAME-TARGET} PUBLIC ./include)

target_compile_options(${GAME-TARGET} PUBLIC
	-Wall 
	-Wextra 
	-Werror
)

set_target_properties(${GAME-TARGET} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)