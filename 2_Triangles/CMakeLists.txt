cmake_minimum_required (VERSION 3.16)
project(triag)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_TYPE Debug)

#main
set (SOURCES sources/main.cpp sources/line_t.cpp sources/plate_t.cpp sources/triangle_t.cpp sources/triangles.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-g -Wall -Werror -Wextra -Wpedantic)

#tests
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (TEST_SOURCES test/unit_tests.cpp sources/line_t.cpp sources/plate_t.cpp sources/triangle_t.cpp)

add_executable(unit_tests ${TEST_SOURCES})

target_include_directories (unit_tests PRIVATE includes)
target_link_libraries(unit_tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)