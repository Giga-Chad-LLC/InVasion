cmake_minimum_required(VERSION 3.15)

project(InVasionInclude CXX)

# adding subdirs
add_subdirectory(./game-models)
add_subdirectory(./interactors)

# propagating variable from inner subdirs to root level project 
set(GAME-MODEL-SRCS ${GAME-MODEL-SRCS} PARENT_SCOPE)
set(INTERACTORS-SRCS ${INTERACTORS-SRCS} PARENT_SCOPE)