cmake_minimum_required(VERSION 3.16)

project(bsort)

find_package(OpenCL REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS -g -Wall -Wextra -Wpedantic)
else ()
    set (COMPILER_SETTINGS -O2 -Wall -Wextra -Wpedantic)
endif ()
add_compile_options(${COMPILER_SETTINGS})

set (SOURCES sources/main.cpp sources/bsort.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE includes)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#unit_test
set (PROJ_TEST_NAME bitonic_sort_test)
set (TEST_SOURCES test/main.cpp sources/bsort.cpp test/unit_test.cpp)

add_executable(${PROJ_TEST_NAME} ${TEST_SOURCES})
target_include_directories (${PROJ_TEST_NAME} PRIVATE includes)
target_link_libraries(${PROJ_TEST_NAME} OpenCL::OpenCL)
target_compile_features(${PROJ_TEST_NAME} PRIVATE cxx_std_17)