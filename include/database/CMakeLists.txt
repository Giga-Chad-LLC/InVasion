cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
add_executable(db test.cpp)
target_link_libraries(db sqlite3)
