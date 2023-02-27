cmake_minimum_required(VERSION 3.16)

project(bsort)

find_package(OpenCLICDLoader)
if (NOT OpenCLICDLoader_FOUND)
    message("If cmake couldn't find OpenCL loader, please set the path to OpenCLICDLoaderConfig.cmake in CMAKE_PREFIX_PATH")
endif()

find_package(OpenCL REQUIRED)
find_package(Threads REQUIRED)

set(OpenCL_libs OpenCL::OpenCL Threads::Threads)

find_package(OpenCLHeaders)
if (NOT OpenCLHeaders_FOUND)
    message("If cmake couldn't find OpenCL header files, please set the path to OpenCLHeadersConfig.cmake in CMAKE_PREFIX_PATH")
else()
    set(OpenCL_libs ${OpenCL_libs} OpenCL::Headers)
endif()

find_package(OpenCLHeadersCpp)
if (NOT OpenCLHeadersCpp_FOUND)
    message("If cmake couldn't find OpenCL C++ header files, please set the path to OpenCLHeadersCppConfig.cmake in CMAKE_PREFIX_PATH")
else()
    set(OpenCL_libs ${OpenCL_libs} OpenCL::HeadersCpp)
endif()


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
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCL_libs})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

#unit_test
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

set (PROJ_TEST_NAME bitonic_sort_test)
set (TEST_SOURCES test/main.cpp sources/bsort.cpp test/unit_test.cpp)

add_executable(${PROJ_TEST_NAME} ${TEST_SOURCES})
target_include_directories (${PROJ_TEST_NAME} PRIVATE includes ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJ_TEST_NAME} PRIVATE ${OpenCL_libs} ${Boost_LIBRARIES})
target_compile_features(${PROJ_TEST_NAME} PRIVATE cxx_std_20)

#gtest
enable_testing()
find_package(GTest REQUIRED)

set (PROJ_GTEST_NAME gtest)
set (GTEST_SOURCES test/gtest.cpp sources/bsort.cpp)

add_executable(${PROJ_GTEST_NAME} ${GTEST_SOURCES})
target_include_directories (${PROJ_GTEST_NAME} PRIVATE includes ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJ_GTEST_NAME} PRIVATE ${OpenCL_libs} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_compile_features(${PROJ_GTEST_NAME} PRIVATE cxx_std_20)
