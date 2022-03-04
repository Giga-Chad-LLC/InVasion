cmake_minimum_required(VERSION 3.15)

project(InVasionInteractors CXX)

set(INTERACTORS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/MoveInteractor/move-interactor.cpp
	
	PARENT_SCOPE
)