cmake_minimum_required(VERSION 3.16)

project(bsort)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS  -g -Wall  -Wextra -Wpedantic)
else ()
    set (COMPILER_SETTINGS -Wall  -Wextra -Wpedantic)
endif ()
add_compile_options(${COMPILER_SETTINGS})

set (SOURCES bsort.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

find_package(OpenCL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)