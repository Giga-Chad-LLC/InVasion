cmake_minimum_required(VERSION 3.15)

project(InVasionBcrypt CXX)

set(BCRYPT-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/src/bcrypt_impl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blowfish.cpp

	PARENT_SCOPE
)