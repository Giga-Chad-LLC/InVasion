cmake_minimum_required(VERSION 3.15)

project(InVasionTests CXX)

# adding subdirs
add_subdirectory(./game-models)
add_subdirectory(./controllers)

# propagating variable from inner subdirs to root level project
set(GAME-MODELS-TESTS-SRCS ${GAME-MODELS-TESTS-SRCS} PARENT_SCOPE)
SET(CONTROLLERS-TESTS-SRCS ${CONTROLLERS-TESTS-SRCS} PARENT_SCOPE)