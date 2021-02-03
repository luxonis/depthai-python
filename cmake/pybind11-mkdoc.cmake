# Copyright Tomas Zeman 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# constants
set(PYBIND11_MKDOC_MODULE_NAME "pybind11_mkdoc")
set(PYBIND11_MKDOC_TARGET_NAME "pybind11_mkdoc")

function(pybind11_mkdoc_setup target output_path include_dirs mkdoc_headers)


    # Execute module pybind11_mkdoc to check if present
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -m ${PYBIND11_MKDOC_MODULE_NAME} RESULT_VARIABLE error OUTPUT_QUIET)
    if(error)
        message(STATUS "pybind11_mkdoc: Module ${PYBIND11_MKDOC_MODULE_NAME} not found! Target '${PYBIND11_MKDOC_TARGET_NAME}' not available, no docstrings will be generated")
    endif()

    message(STATUS "pybind11_mkdoc headers: " ${mkdoc_headers})

    # Add include directories as -I to arguments of mkdoc
    set(inc_dirs "")
    foreach(include_dir ${include_dirs})
        list(APPEND inc_dirs "-I${include_dir}")
    endforeach()

    # Create a target
    add_custom_command(
        OUTPUT "${output_path}"
        COMMAND
            ${PYTHON_EXECUTABLE}
            -m ${PYBIND11_MKDOC_MODULE_NAME}
            -o "${output_path}"
            # List of include directories
            ${inc_dirs}
            # list of headers
            ${mkdoc_headers}
        DEPENDS ${target}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Creating docstrings with ${PYTHON_EXECUTABLE} -m ${PYBIND11_MKDOC_MODULE_NAME} ..."
    )

    # Create a target
    add_custom_target(
        ${PYBIND11_MKDOC_TARGET_NAME}
        DEPENDS "${output_path}"
    )

    # if(TARGET ${PYBIND11_MKDOC_TARGET_NAME})
    #     add_dependencies(${PYBIND11_MKDOC_TARGET_NAME} ${PROJECT_NAME}__${PYBIND11_MKDOC_TARGET_NAME})
    # else()
    #     add_custom_target(${PYBIND11_MKDOC_TARGET_NAME} DEPENDS ${PROJECT_NAME}_${PYBIND11_MKDOC_TARGET_NAME})
    # endif()

endfunction()


macro(header_directories header_dirs return_list)

  #message(STATUS "folders: ${header_dirs}")
  
  foreach(header_dir ${header_dirs})
    #message(STATUS "inspecting folder: ${header_dir}")
    file(GLOB_RECURSE new_list "${header_dir}/*.hpp")
    set(file_list "")
    foreach(file_path ${new_list})
      set(file_list "${file_list}" ${file_path})
    endforeach()
    list(REMOVE_DUPLICATES file_list)
    set(${return_list} "${${return_list}}" "${file_list}")
  endforeach()

endmacro()

function(target_pybind11_mkdoc_setup output_file target)
  get_target_property(header_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
  header_directories("${header_dirs}" header_files)
  # set(target_files_to_format )
  pybind11_mkdoc_setup("${target}" "${output_file}" "${header_dirs}" "${header_files}")
endfunction()
