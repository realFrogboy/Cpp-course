cmake_minimum_required (VERSION 3.16)
project(tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_TYPE Debug)

if (DUMP)
    add_definitions(-DDUMP)
endif()

#main
set (SOURCES sources/main.cpp sources/range_queries.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-g -Werror -Wall -Wextra -Wpedantic)

#unit_test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT tree_unit_test)
set (TEST_SOURCES test/unit_test.cpp)

add_executable(${UNIT} ${TEST_SOURCES})

target_include_directories (${UNIT} PRIVATE includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

#end_to_end
set (ETE end_to_end)
set (ETE_SOURCES test/end_to_end.cpp)

add_executable(${ETE} ${ETE_SOURCES})

target_include_directories (${ETE} PRIVATE includes)
target_link_libraries(${ETE} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)