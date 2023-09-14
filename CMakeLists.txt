cmake_minimum_required(VERSION 3.26)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
    message(FATAL_ERROR
            "You can not use CMake to build from the root of it's source tree!\n"
            "Remove the CMakeCache.txt file from this directory, then create a separate directory\n"
            "(either below this directory or elsewhere), and then re-run CMake from there.")
endif (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)

project(MonoWrapper)

set(TARGET_NAME MonoWrapper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

include(Mono)

file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/MonoWrapper/*.cpp
    ${PROJECT_SOURCE_DIR}/MonoWrapper/*.h
)

include_directories(
    ${MONO_INCLUDE_PATH}
    ${PROJECT_SOURCE_DIR}/PaladiumLoaderBDS
)

add_library(${TARGET_NAME} STATIC ${SRC_FILES})

target_link_libraries(${TARGET_NAME}
    ${MONO_LIBRARIES}
)