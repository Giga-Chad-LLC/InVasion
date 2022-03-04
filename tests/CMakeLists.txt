cmake_minimum_required(VERSION 3.15)

project(InVasionTests CXX)

# adding subdirs
add_subdirectory(./game-models)

# propagating variable from inner subdirs to root level project
set(GAME-MODELS-TESTS-SRCS ${GAME-MODELS-TESTS-SRCS} PARENT_SCOPE)