cmake_minimum_required (VERSION 3.16)
project(tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#main
set (SOURCES sources/main.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-Werror -Wall -Wextra -Wpedantic)