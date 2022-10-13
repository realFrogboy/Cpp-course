cmake_minimum_required (VERSION 3.16)
project(triag)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_TYPE Debug)

#main
set (SOURCES sources/main.cpp sources/triangles.cpp sources/vector_t.cpp sources/point_t.cpp sources/line_t.cpp sources/plate_t.cpp sources/triangle_t.cpp optimizers/bsp_tree/sources/bsp.cpp optimizers/bounding_box/sources/bounding_box.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes optimizers/bsp_tree/includes optimizers/bounding_box/includes)

add_compile_options (-g -Wall -Werror -Wextra -Wpedantic)

#test generator
set (GENERATOR generation)
set (GENERATOR_SOURCES test/test_generator.cpp)

add_executable(${GENERATOR} ${GENERATOR_SOURCES})

#endtoend test
set(ENDTOEND Test)
set(TEST_SOURCES test/main.cpp)

add_executable(${ENDTOEND} ${TEST_SOURCES})

#unit tests
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT unit_tests)
set (UNIT_SOURCES test/unit_tests.cpp sources/vector_t.cpp sources/point_t.cpp sources/line_t.cpp sources/plate_t.cpp sources/triangle_t.cpp optimizers/bounding_box/sources/bounding_box.cpp)

add_executable(${UNIT} ${UNIT_SOURCES})

target_include_directories (${UNIT} PRIVATE includes optimizers/bsp_tree/includes optimizers/bounding_box/includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)