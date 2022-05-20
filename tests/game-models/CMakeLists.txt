cmake_minimum_required(VERSION 3.15)

project(InVasionGameModelsTests CXX)

set(GAME-MODELS-TESTS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/general-tests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/game-session-tests.cpp
	
	PARENT_SCOPE
)