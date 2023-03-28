cmake_minimum_required(VERSION 3.13.4)

project(paracl)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_LINKER lld)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (DUMP)
    add_definitions(-DDUMP)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS -gdwarf-4 -g -fsanitize=leak -Wall -Wextra -Wpedantic)  
else ()
    set (COMPILER_SETTINGS -O2 -Wall -Wextra -Wpedantic)
endif ()

add_compile_options(${COMPILER_SETTINGS})

flex_target(scanner
  lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

if (CODEGEN)
    bison_target(parser
      IRparser.y
      ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
      COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh")
else()
    bison_target(parser
      parser.y
      ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
      COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh")
endif()

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME}   
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  sources/main.cpp sources/codegen.cpp sources/iterate.cpp
)

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core codegen irreader mc mcparser option)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE includes ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${llvm_libs})