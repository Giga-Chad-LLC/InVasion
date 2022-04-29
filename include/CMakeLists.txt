cmake_minimum_required(VERSION 3.15)

project(InVasionInclude CXX)

# adding subdirs
add_subdirectory(./game-models)
add_subdirectory(./interactors)
add_subdirectory(./controllers)
add_subdirectory(./server)
add_subdirectory(./utils)

# propagating variable from inner subdirs to root level project 
set(GAME-MODELS-SRCS ${GAME-MODELS-SRCS} PARENT_SCOPE)
set(INTERACTORS-SRCS ${INTERACTORS-SRCS} PARENT_SCOPE)
set(CONTROLLERS-SRCS ${CONTROLLERS-SRCS} PARENT_SCOPE)
set(GAME-SERVER-SRCS ${GAME-SERVER-SRCS} PARENT_SCOPE)
set(UTILS-SRCS       ${UTILS-SRCS}       PARENT_SCOPE)