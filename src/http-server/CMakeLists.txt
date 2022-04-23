cmake_minimum_required(VERSION 3.2)
project(test)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Boost 1.69)
set(CMAKE_CXX_STANDARD 17)
add_executable(unittest http_server.cpp)

target_link_libraries(unittest ${Boost_LIBRARIES})
target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(unittest pthread)

target_link_libraries(unittest gcov)
target_link_libraries(unittest sqlite3)


