cmake_minimum_required(VERSION 2.8)
project(protobufcodec_prj)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-std=gnu++11 -O3)

add_subdirectory(src)
add_subdirectory(examples)