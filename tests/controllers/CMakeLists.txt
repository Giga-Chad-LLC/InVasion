cmake_minimum_required(VERSION 3.15)

project(InVasionControllersTests CXX)

set(CONTROLLERS-TESTS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/io-tests.cpp
	
	PARENT_SCOPE
)