cmake_minimum_required(VERSION 3.15)

project(InVasionControllers CXX)

set(CONTROLLERS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FixedTimeIntervalInvoker/fixed-time-interval-invoker.cpp

	PARENT_SCOPE
)