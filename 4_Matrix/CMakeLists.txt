cmake_minimum_required (VERSION 3.16)
project(matrix)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS  -g -Wall  -Wextra -Wpedantic)
    
else ()
    set (COMPILER_SETTINGS -Wall  -Wextra -Wpedantic)

endif ()

add_compile_options(${COMPILER_SETTINGS})

#main
set (SOURCES sources/main.cpp sources/matrix.cpp sources/input.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE includes)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#unit_test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT matrix_unit_test)
set (TEST_SOURCES sources/matrix.cpp test/unit_test.cpp)

add_executable(${UNIT} ${TEST_SOURCES})

target_include_directories (${UNIT} PRIVATE includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

target_compile_features(${UNIT} PRIVATE cxx_std_17)

#endtoend test
set(ENDTOEND end_to_end)
set(TEST_SOURCES sources/matrix.cpp test/end_to_end.cpp test/test_generator.cpp)

add_executable(${ENDTOEND} ${TEST_SOURCES})

target_include_directories (${ENDTOEND} PRIVATE includes)
target_link_libraries(${ENDTOEND} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

target_compile_features(${ENDTOEND} PRIVATE cxx_std_17)