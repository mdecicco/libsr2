cmake_minimum_required(VERSION 3.15)
project(libsr2)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE libsr2_src
    "src/*.cpp"
    "src/vehicle/*.cpp"
    "src/terrain/*.cpp"
    "src/sim/*.cpp"
    "src/utils/*.cpp"
    "src/gfx/*.cpp"
    "src/loaders/*.cpp"
    "src/math/*.cpp"

    "include/libsr2/*.h"
    "include/libsr2/*.hpp"
    "include/vehicle/*.h"
    "include/vehicle/*.hpp"
    "include/terrain/*.h"
    "include/terrain/*.hpp"
    "include/sim/*.h"
    "include/sim/*.hpp"
    "include/utils/*.h"
    "include/utils/*.hpp"
    "include/gfx/*.h"
    "include/gfx/*.hpp"
    "include/loaders/*.h"
    "include/loaders/*.hpp"
    "include/math/*.h"
    "include/math/*.hpp"
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(libsr2 ${libsr2_src})

MACRO(SOURCE_GROUP_BY_FOLDER target)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${target}_src})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
ENDMACRO(SOURCE_GROUP_BY_FOLDER)
SOURCE_GROUP_BY_FOLDER(libsr2)

target_include_directories(libsr2 PUBLIC src)
target_include_directories(libsr2 PUBLIC include)
target_include_directories(libsr2 PUBLIC "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\include")

target_link_directories(libsr2 PUBLIC "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\libs\\Win64")
target_link_directories(libsr2 PUBLIC lib)

SET(debug_libs 
	debug/zlibstaticd
	OpenAL32
)

SET(release_libs
	release/zlibstatic
	OpenAL32
)

target_link_libraries(libsr2 ${debug_libs})
#target_link_libraries(libsr2 ${release_libs})


set_property(TARGET libsr2 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")

add_definitions(-D_CRT_NO_VA_START_VALIDATION)