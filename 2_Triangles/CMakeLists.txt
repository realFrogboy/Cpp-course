cmake_minimum_required (VERSION 3.16)
project(ideal_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#tests
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (TEST_SOURCES sources/unit_tests.cpp)

add_executable(unit_tests ${TEST_SOURCES})

target_include_directories (unit_tests PRIVATE includes)
target_link_libraries(unit_tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)