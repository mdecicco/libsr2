cmake_minimum_required(VERSION 3.15)

project(libmpeg2)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Find library sources
file(GLOB_RECURSE LIB_SRC_FILES src/*.c)

# Build static/shared library
add_library(libmpeg2 STATIC ${LIB_SRC_FILES})

target_include_directories(libmpeg2 PUBLIC "include")
