cmake_minimum_required(VERSION 3.13.4)
project(paracl)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (DUMP)
    add_definitions(-DDUMP)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS  -g -Wall  -Wextra -Wpedantic)
    
else ()
    set (COMPILER_SETTINGS -Wall  -Wextra -Wpedantic)

endif ()

add_compile_options(${COMPILER_SETTINGS})

flex_target(scanner
  lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
  parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh")

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME}   
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  sources/main.cpp sources/ast.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE includes)