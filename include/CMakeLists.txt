cmake_minimum_required(VERSION 3.15)

project(InVasionInclude CXX)

# adding subdirs
add_subdirectory(./game-models)

# propagating variable from inner files to root level CMakeLists.txt 
set(GAME-MODEL-SRCS ${GAME-MODEL-SRCS} PARENT_SCOPE)