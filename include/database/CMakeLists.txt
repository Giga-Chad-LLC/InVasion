cmake_minimum_required(VERSION 3.15)

project(InVasionDatabase CXX)

set(DATABASE-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Authenticator/autenticator.h
	${CMAKE_CURRENT_SOURCE_DIR}/AuthService/auth-service.h
	${CMAKE_CURRENT_SOURCE_DIR}/DatabaseAccessor/database-accessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/StatisticAccessor/statistic-accessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/StatisticsContainer/statistic-container.h
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerStatisticsService/player-statistics-service.cpp

	PARENT_SCOPE
)
