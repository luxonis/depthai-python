# Helper to create a pybind11_mkdoc target which takes

# Usage:
# target_pybind11_mkdoc_setup([path/to/output/docstring.hpp] [Library for which to generate: target-name] [Enforce pybind11_mkdoc existing ON/OFF])
function(target_pybind11_mkdoc_setup output_file target enforce)

    # Get both interface include directories and system include directories
    get_target_property(header_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(header_system_dirs ${target} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
    
    # Remove system include directories
    set(non_system_dirs ${header_dirs})
    foreach(sys_dir ${header_system_dirs})
        list(REMOVE_ITEM non_system_dirs "${sys_dir}")
    endforeach()
    
    # Get all header files
    header_directories("${non_system_dirs}" header_files)
    
    # Setup mkdoc target
    pybind11_mkdoc_setup_internal("${target}" "${output_file}" "${header_files}" ${enforce})

endfunction()

# Internal helper, sets up pybind11_mkdoc target
function(pybind11_mkdoc_setup_internal target output_path mkdoc_headers enforce)

    # constants
    set(PYBIND11_MKDOC_MODULE_NAME "pybind11_mkdoc")
    set(PYBIND11_MKDOC_TARGET_NAME "pybind11_mkdoc")

    # Execute module pybind11_mkdoc to check if present
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -m ${PYBIND11_MKDOC_MODULE_NAME} RESULT_VARIABLE error OUTPUT_QUIET)
    if(error)
        set(messageStatus "STATUS")
        if(enforce)
            set(messageStatus "FATAL_ERROR")
        endif()
        message(${messageStatus} "pybind11_mkdoc: Module ${PYBIND11_MKDOC_MODULE_NAME} not found! Target '${PYBIND11_MKDOC_TARGET_NAME}' not available, no docstrings will be generated")
        # Exit
        return()
    endif()

    # Prepare the output folder for the mkdoc
    get_filename_component(output_directory "${output_path}" DIRECTORY)
    # Create the command
    add_custom_command(
        OUTPUT "${output_path}"
        # Create directory first (if it doesn't exist)
        COMMAND ${CMAKE_COMMAND} -E make_directory "${output_directory}"
        # Execute mkdoc
        COMMAND
            ${PYTHON_EXECUTABLE}
            -m ${PYBIND11_MKDOC_MODULE_NAME}
            -o "${output_path}"
            # List of include directories
            "-I$<JOIN:$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>,;-I>"
            # List of compiler definitions
            "-D$<JOIN:$<TARGET_PROPERTY:${target},COMPILE_DEFINITIONS>,;-D>"
            # List of headers for which to generate docstrings
            "${mkdoc_headers}"
            # Redirect stderr output to not spam output
            2> /dev/null
        DEPENDS ${target}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Creating docstrings with ${PYTHON_EXECUTABLE} -m ${PYBIND11_MKDOC_MODULE_NAME} ..."
        VERBATIM
        COMMAND_EXPAND_LISTS
    )

    # Create a target
    add_custom_target(
        ${PYBIND11_MKDOC_TARGET_NAME}
        DEPENDS "${output_path}"
    )

    # Add dependency to mkdoc target (makes sure that mkdoc is executed, and docstrings available)
    add_dependencies(${TARGET_NAME} ${PYBIND11_MKDOC_TARGET_NAME})

endfunction()

# Internal helper, converts header directory list to list of .hpp files
macro(header_directories header_dirs return_list)

    #message(STATUS "folders: ${header_dirs}")
    foreach(header_dir ${header_dirs})
        # only use build_interface includes

        string(REGEX MATCHALL "\\$<BUILD_INTERFACE:([^ \t\n\r]+)>" tmp_output_variable "${header_dir}")
        #message(STATUS "inspecting folder: ${header_dir}, stripped ${hdir}, regex count ${CMAKE_MATCH_COUNT}, regex match 0 and 1, ${CMAKE_MATCH_0}, ${CMAKE_MATCH_1}")
        # If regex matched and path is present
        if(CMAKE_MATCH_COUNT GREATER 0)
            set(hdir "${CMAKE_MATCH_1}")
        else()    
            # Strip generator expresssions
            string(GENEX_STRIP "${header_dir}" hdir)
        endif()
        
        # if hdir isn't an empty path
        if(hdir)

            #message(STATUS "inspecting folder: ${hdir}")
            file(GLOB_RECURSE new_list "${hdir}/*.hpp")
            set(file_list "")

            foreach(file_path ${new_list})
                #message(STATUS "inspecting file: ${file_path}")
                set(file_list "${file_list}" ${file_path})
            endforeach()
            
            list(REMOVE_DUPLICATES file_list)
            set(${return_list} "${${return_list}}" "${file_list}")
            
        endif()

    endforeach()

endmacro()

