cmake_minimum_required(VERSION 3.15)

project(InVasionServers CXX)

# adding subdirs
add_subdirectory(./include/)
add_subdirectory(./tests/)

# variables propagations
# GAME-MODEL-SRCS - propagated from ./include/game-models
# INTERACTORS-SRCS - propagated from ./include/interactors

# GAME-MODELS-TESTS-SRCS - propagated from ./tests/game-models


# setting names for executables
set(GAME-TARGET game)
set(GAME-MODELS-TESTS-TARGET game-models-tests)


# configuring GAME-TARGET:

# add_executable(${GAME-TARGET} 
# 	src/game.cpp 
# 	${GAME-MODEL-SRCS}
# )

# target_include_directories(${GAME-TARGET} PUBLIC 
# 	./include
# )

# target_compile_options(${GAME-TARGET} PUBLIC
# 	-Wall 
# 	-Wextra 
# 	-Werror
# )

# set_target_properties(${GAME-TARGET} PROPERTIES
# 	CXX_STANDARD 17
#     CXX_STANDARD_REQUIRED ON
# )



# configuring GAME-MODELS-TESTS-TARGET:

add_executable(${GAME-MODELS-TESTS-TARGET}
	src/doctest_main.cpp
	${GAME-MODELS-TESTS-SRCS}
	${GAME-MODEL-SRCS}
	${INTERACTORS-SRCS}
)

target_include_directories(${GAME-MODELS-TESTS-TARGET} PUBLIC 
	./doctest
	./include
)

target_compile_options(${GAME-MODELS-TESTS-TARGET} PUBLIC
	-Wall
	-Wextra
	-Werror
)

set_target_properties(${GAME-MODELS-TESTS-TARGET} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)