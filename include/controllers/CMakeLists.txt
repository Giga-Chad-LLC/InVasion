cmake_minimum_required(VERSION 3.15)

project(InVasionControllers CXX)

set(CONTROLLERS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/FixedTimeIntervalInvoker/fixed-time-interval-invoker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StaticObjectsFileReader/static-objects-file-reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryFilesContainer/directory-files-container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SupplyTypeModelChecker/supply-type-model-checker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RespawnPointsFileReader/respawn-points-file-reader.cpp

	PARENT_SCOPE
)