cmake_minimum_required(VERSION 3.10)

# 项目名称
project(APASP)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(GraphGen)
add_subdirectory(Floyd)
add_subdirectory(APASP)