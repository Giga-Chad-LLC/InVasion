cmake_minimum_required(VERSION 3.2)
project(test)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Boost 1.69)
set(CMAKE_CXX_STANDARD 17)
add_executable(unittest main.cpp)

target_link_libraries(unittest ${Boost_LIBRARIES})
target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(unittest pthread)

set_target_properties(unittest PROPERTIES COMPILE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
target_link_libraries(unittest gcov)
#find_package(Boost 1.71 REQUIRED filesystem)  # Required by Boost.DLL
#include_directories(${Boost_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIRS})

