cmake_minimum_required(VERSION 3.15)

project(InVasionDatabase CXX)

set(DATABASE-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Authenticator/autenticator.h
	${CMAKE_CURRENT_SOURCE_DIR}/AuthService/auth-service.h
	${CMAKE_CURRENT_SOURCE_DIR}/DatabaseAccessor/database-accessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/StatisticAccessor/statistic-accessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/StatisticsContainer/statistic-container.h

	PARENT_SCOPE
)

# cmake_minimum_required(VERSION 3.2)
# set(CMAKE_CXX_STANDARD 17)
# add_executable(db test.cpp)
# target_link_libraries(db sqlite3)
