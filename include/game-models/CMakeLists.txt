cmake_minimum_required(VERSION 3.15)

project(InVasionGameModels CXX)

set(GAME-MODEL-SRCS
	./include/game-models/Vector2D/vector2d.cpp
	./include/game-models/BoxCollider/box-collider.cpp
	./include/game-models/Object/object.cpp
	./include/game-models/KinematicObject/kinematic-object.cpp
	PARENT_SCOPE
)