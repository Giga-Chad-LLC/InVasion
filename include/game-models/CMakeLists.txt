cmake_minimum_required(VERSION 3.15)

project(InVasionGameModels CXX)

set(GAME-MODEL-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2D/vector2d.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BoxCollider/box-collider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Object/object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KinematicObject/kinematic-object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Player/player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameWorldManager/game-world-manager.cpp
	
	PARENT_SCOPE
)