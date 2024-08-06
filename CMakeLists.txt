cmake_minimum_required(VERSION 3.15)
project(libsr2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/debug)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release)

set (all_sources "")
macro(add_sources group dir)
    set(_all "")

    file(GLOB _inc "include/libsr2/${dir}/*.h" "include/libsr2/${dir}/*.hpp" "include/libsr2/${dir}/*.inl")
    list(APPEND _all ${_inc})

    file(GLOB _src "src/${dir}/*.cpp")
    list(APPEND _all ${_src})

    list(APPEND all_sources ${_all})
    string(REPLACE "/" "\\" group_path "${group}")
    source_group(${group_path} FILES ${_all})

    unset(_inc)
    unset(_src)
    unset(_all)
endmacro(add_sources)

add_sources("src"           ".")
add_sources("src/ui"        "ui")
add_sources("src/io"        "io")
add_sources("src/gfx"       "gfx")
add_sources("src/sim"       "sim")
add_sources("src/math"      "math")
add_sources("src/i18n"      "i18n")
add_sources("src/audio"     "audio")
add_sources("src/utils"     "utils")
add_sources("src/states"    "states")
add_sources("src/loaders"   "loaders")
add_sources("src/terrain"   "terrain")
add_sources("src/vehicle"   "vehicle")
add_sources("src/mission"   "mission")
add_sources("src/unknown"   "unknown")
add_sources("src/creature"  "creature")
add_sources("src/frontend"  "frontend")
add_sources("src/managers"  "managers")
add_sources("src/particles" "particles")
add_sources("src/instances" "instances")
add_sources("src/utilities" "utilities")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(deps)
add_subdirectory(game)

file(GLOB _src "deps/libmpeg2/src/*.c")
list(APPEND all_sources ${_src})
unset(_src)

add_library(libsr2 ${all_sources})

target_include_directories(libsr2 PUBLIC src)
target_include_directories(libsr2 PUBLIC include)
target_include_directories(libsr2 PUBLIC "deps/zlib/include")
target_include_directories(libsr2 PUBLIC "deps/renderer/include")
target_include_directories(libsr2 PUBLIC "deps/renderer/deps/utils/include")
target_include_directories(libsr2 PUBLIC "deps/libmpeg2/src")
target_include_directories(libsr2 PUBLIC "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\include")

target_link_directories(libsr2 PUBLIC "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\libs\\Win64")
target_link_directories(libsr2 PUBLIC lib)

SET(debug_libs 
	debug/zlibstaticd
	OpenAL32
    utils
    render
)

SET(release_libs
	release/zlibstatic
	OpenAL32
    utils
    render
)

target_link_libraries(libsr2 ${debug_libs})
#target_link_libraries(libsr2 ${release_libs})
set_target_properties(libsr2 PROPERTIES LINK_FLAGS "/ignore:4099")

set_property(TARGET libsr2 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")

add_definitions(-D_CRT_NO_VA_START_VALIDATION)