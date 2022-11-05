cmake_minimum_required (VERSION 3.16)
project(triag)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

set (CMAKE_BUILD_TYPE Debug)

#main
set (SOURCES 
    triangles/sources/main.cpp triangles/sources/triangles.cpp triangles/utilities/sources/bsp.cpp visualization/sources/first_app.cpp 
    visualization/sources/keyboard_movement_controller.cpp visualization/sources/lve_camera.cpp visualization/sources/lve_device.cpp 
    visualization/sources/lve_model.cpp visualization/sources/lve_pipeline.cpp visualization/sources/lve_renderer.cpp 
    visualization/sources/lve_swap_chain.cpp visualization/sources/lve_window.cpp visualization/sources/simple_render_system.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE triangles/includes visualization/includes triangles/utilities/includes ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glm::glm glfw ${Vulkan_LIBRARIES})

add_compile_options (-g -Wall -Werror -Wextra -Wpedantic)

#test generator
set (GENERATOR generation)
set (GENERATOR_SOURCES triangles/test/test_generator.cpp)

add_executable(${GENERATOR} ${GENERATOR_SOURCES})

#endtoend test
set(ENDTOEND Test)
set(TEST_SOURCES triangles/test/main.cpp)

add_executable(${ENDTOEND} ${TEST_SOURCES})

#unit tests
#enable_testing()
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

#set (UNIT unit_tests)
#set (UNIT_SOURCES test/unit_tests.cpp )

#add_executable(${UNIT} ${UNIT_SOURCES})

#target_include_directories (${UNIT} PRIVATE includes optimizers/bsp_tree/includes optimizers/bounding_box/includes)
#Starget_link_libraries(${UNIT} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)