cmake_minimum_required (VERSION 3.16)
project(matrix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_TYPE Debug)

#main
set (SOURCES sources/main.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-g -Werror -Wall -Wextra -Wpedantic)

#unit_test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT matrix_unit_test)
set (TEST_SOURCES test/unit_test.cpp)

add_executable(${UNIT} ${TEST_SOURCES})

target_include_directories (${UNIT} PRIVATE includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)