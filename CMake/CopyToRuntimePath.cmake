include(CMakeParseArguments)

function (copy_files_to_runtime_path)
    cmake_parse_arguments(
        FUNCTION_ARGS
        ""
        "DESTINATION"
        "FILES;CONFIGURATIONS"
        ${ARGN}
    )

    # Argument validation.
    if (NOT FUNCTION_ARGS_FILES)
        message(FATAL_ERROR "No files provided for copying.")
    endif ()

    # Use default values if applicable.
    set(FUNCTION_ARGS_DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    # Copying of the files.
    set(DESTINATION "${FUNCTION_ARGS_DESTINATION}")

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${DESTINATION}")
    foreach (FILENAME ${FUNCTION_ARGS_FILES})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILENAME}" "${DESTINATION}/")
    endforeach ()
endfunction ()