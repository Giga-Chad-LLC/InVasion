cmake_minimum_required(VERSION 3.15)

project(InVasionUtils CXX)

set(UTILS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/TimeUtilities/time-utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerSpecializationUtilities/player-specialization-utilities.cpp

	PARENT_SCOPE
)