cmake_minimum_required(VERSION 3.15)

project(InVasionHttpServer CXX)

set(HTTP-SERVER-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/HttpServer/http-server.cpp

	PARENT_SCOPE
)