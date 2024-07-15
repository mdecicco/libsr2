cmake_minimum_required(VERSION 3.15)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)

file(GLOB all_sources "src/*.cpp")

add_executable(game ${all_sources})

target_include_directories(game PUBLIC "../include" "src")
target_link_directories(game PUBLIC "../lib")

target_link_libraries(game libsr2)
#target_link_libraries(game ${release_libs})
set_property(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
add_definitions(-D_CRT_NO_VA_START_VALIDATION)