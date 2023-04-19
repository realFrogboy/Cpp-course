cmake_minimum_required(VERSION 3.16)

project(paracl)

set(COMPILE_NAME ${PROJECT_NAME})
set(INTERP_NAME "${PROJECT_NAME}i")

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
    set (COMPILER_SETTINGS -gdwarf-4 -g -Wall -Wextra -Wpedantic)  
else ()
    set (COMPILER_SETTINGS -O2 -Wall -Wextra -Wpedantic)
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

add_executable(${COMPILE_NAME}  
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  sources/main.cpp sources/codegen.cpp sources/iterate.cpp
)

target_compile_definitions(${COMPILE_NAME} PRIVATE CODEGEN=1)

add_executable(${INTERP_NAME}  
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  sources/main.cpp sources/codegen.cpp sources/iterate.cpp
)

target_compile_definitions(${INTERP_NAME} PRIVATE CODEGEN=0)

set(TARGETS
  ${COMPILE_NAME}
  ${INTERP_NAME}
)

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core codegen irreader mc mcparser option)

foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_20)
  target_include_directories(${TNAME} PRIVATE includes ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR} ${LLVM_INCLUDE_DIRS})
  target_link_libraries(${TNAME} PRIVATE ${Boost_LIBRARIES} ${llvm_libs})
endforeach()