include(CMakeParseArguments)
include(CopyToRuntimePath)

find_package(Mono)

if (NOT MONO_FOUND)
    message(FATAL_ERROR "Mono is required for this CMake script. Please install it.")
endif ()

find_file(MONO_DLL_PATH NAMES mono-2.0.dll mono-2.0-sgen.dll PATHS ${MONO_BINARY_PATH})
copy_files_to_runtime_path(FILES ${MONO_DLL_PATH})

function (add_mono_assembly)
    cmake_parse_arguments(
        MONO_ASSEMBLY_PARSED_ARGS
        ""
        "TARGET;DESTINATION;TYPE"
        "SOURCES;ADDITIONAL_ARGS"
        ${ARGN})

    if (NOT MONO_ASSEMBLY_PARSED_ARGS_TYPE)
        set(MONO_ASSEMBLY_PARSED_ARGS_TYPE "library")
    endif ()

    set(_FILE_EXTENSION "")
    if (MONO_ASSEMBLY_PARSED_ARGS_TYPE STREQUAL "library")
        set(_FILE_EXTENSION ".dll")
    elseif (MONO_ASSEMBLY_PARSED_ARGS_TYPE STREQUAL "exe")
        set(_FILE_EXTENSION ".exe")
    else ()
        message(FATAL_ERROR "Unknown assembly type: ${MONO_ASSEMBLY_PARSED_ARGS_TYPE}")
    endif ()

    set(_SOURCES ${MONO_ASSEMBLY_PARSED_ARGS_SOURCES})

    if (NOT MONO_ASSEMBLY_PARSED_ARGS_DESTINATION)
        set(MONO_ASSEMBLY_PARSED_ARGS_DESTINATION ${CMAKE_BINARY_DIR})
    endif ()

    file(MAKE_DIRECTORY ${MONO_ASSEMBLY_PARSED_ARGS_DESTINATION})

    add_custom_target(
        ${MONO_ASSEMBLY_PARSED_ARGS_TARGET} ALL
        ${MONO_MCS_EXECUTABLE} "-t:${MONO_ASSEMBLY_PARSED_ARGS_TYPE}" ${MONO_ASSEMBLY_PARSED_ARGS_ADDITIONAL_ARGS} ${_SOURCES} "-out:${MONO_ASSEMBLY_PARSED_ARGS_TARGET}${_FILE_EXTENSION}"
        WORKING_DIRECTORY "${MONO_ASSEMBLY_PARSED_ARGS_DESTINATION}"
        COMMENT "Building Mono Library ${MONO_ASSEMBLY_PARSED_ARGS_TARGET}"
        SOURCES ${_SOURCES}
    )
endfunction ()