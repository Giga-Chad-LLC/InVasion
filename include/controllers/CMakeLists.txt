cmake_minimum_required(VERSION 3.15)

project(InVasionControllers CXX)

set(CONTROLLERS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FixedTimeIntervalInvoker/fixed-time-interval-invoker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TilemapsFileReader/tilemaps-file-reader.cpp

	PARENT_SCOPE
)