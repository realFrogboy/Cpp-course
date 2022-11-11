cmake_minimum_required (VERSION 3.20)
project(triag)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS  -g -Wall  -Wextra -Wpedantic)
    
else ()
    set (COMPILER_SETTINGS -Wall  -Wextra -Wpedantic)

endif ()

add_compile_options(${COMPILER_SETTINGS})


#shaders compilation
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "visualization/shaders/*.frag"
  "visualization/shaders/*.vert"
)
 
foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "../visualization/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND Vulkan::glslc ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)
 
add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

#main

set (SOURCES
    triangles/sources/line_t.cpp triangles/sources/plate_t.cpp triangles/sources/triangle_t.cpp
    triangles/sources/main.cpp triangles/sources/input.cpp triangles/utilities/sources/bsp.cpp visualization/sources/first_app.cpp 
    visualization/sources/keyboard_movement_controller.cpp visualization/sources/lve_camera.cpp visualization/sources/lve_device.cpp 
    visualization/sources/lve_model.cpp visualization/sources/lve_pipeline.cpp visualization/sources/lve_renderer.cpp visualization/sources/help.cpp
    visualization/sources/lve_swap_chain.cpp visualization/sources/lve_window.cpp visualization/sources/simple_render_system.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} Shaders)

target_include_directories (${PROJECT_NAME} PRIVATE triangles/includes visualization/includes triangles/utilities/includes ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glm::glm glfw ${Vulkan_LIBRARIES})

#test generator
set (GENERATOR generation)
set (GENERATOR_SOURCES triangles/test/test_generator.cpp triangles/sources/line_t.cpp triangles/sources/plate_t.cpp triangles/sources/triangle_t.cpp)

add_executable(${GENERATOR} ${GENERATOR_SOURCES})

target_include_directories (${GENERATOR} PRIVATE triangles/includes)

#endtoend test
set(ENDTOEND Test)
set(TEST_SOURCES triangles/test/main.cpp triangles/sources/line_t.cpp triangles/sources/plate_t.cpp triangles/sources/triangle_t.cpp)

add_executable(${ENDTOEND} ${TEST_SOURCES})

target_include_directories (${ENDTOEND} PRIVATE triangles/includes)

#unit tests
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (UNIT unit_tests)
set (UNIT_SOURCES triangles/test/unit_tests.cpp triangles/sources/line_t.cpp triangles/sources/plate_t.cpp triangles/sources/triangle_t.cpp)

add_executable(${UNIT} ${UNIT_SOURCES})

target_include_directories (${UNIT} PRIVATE triangles/includes triangles/utilities/includes)
target_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)