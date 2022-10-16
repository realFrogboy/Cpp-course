cmake_minimum_required (VERSION 3.16)
project(tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (DUMP)
    add_definitions(-DDUMP)
endif()

#main
set (SOURCES sources/main.cpp sources/range_queries.cpp sources/tree.cpp sources/tree_dump.cpp sources/input.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

add_compile_options (-g -Wall -Werror -Wextra -Wpedantic)

#unit_test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT tree_unit_test)
set (TEST_SOURCES sources/tree.cpp test/unit_test.cpp sources/tree_dump.cpp)

add_executable(${UNIT} ${TEST_SOURCES})

target_include_directories (${UNIT} PRIVATE includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

#end_to_end
set (ETE end_to_end)
set (ETE_SOURCES sources/tree.cpp sources/range_queries.cpp test/end_to_end.cpp sources/tree_dump.cpp sources/input.cpp)

add_executable(${ETE} ${ETE_SOURCES})

target_include_directories (${ETE} PRIVATE includes)
target_link_libraries(${ETE} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)