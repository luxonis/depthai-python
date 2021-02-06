# This list specifies building of documentation using CMake
option(DEPTHAI_PYTHON_GENERATE_DOCSTRINGS "Generate docstrings from header files - pybind11_mkdoc required" ON)

# Add target to generate docstrings
if (DEPTHAI_PYTHON_GENERATE_DOCSTRINGS)
    include(pybind11-mkdoc)

    # Check if pybind11_mkdoc available and create target
    target_pybind11_mkdoc_setup("${CMAKE_CURRENT_LIST_DIR}/generated/depthai_core_docstring.hpp" depthai-core OFF)
endif()


# Sphinx is required to build python documentation
find_package(Sphinx REQUIRED)

# Set input and output
set(SPHINX_SOURCE ${CMAKE_CURRENT_LIST_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)

# Get doxygen build location form doxygen target
get_target_property(DOXYGEN_OUTPUT_DIR doxygen DOXYGEN_OUTPUT_DIR)
message(STATUS "Retrieved doxygen output dir from doxygen target: ${DOXYGEN_OUTPUT_DIR}")

add_custom_target(sphinx ALL
    ${CMAKE_COMMAND} -E env "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>:$ENV{PYTHONPATH}" 
    ${SPHINX_EXECUTABLE} -b html
    # Tell Breathe where to find the Doxygen output
    #-Dbreathe_projects.depthai-core=${DOXYGEN_OUTPUT_DIR}/xml
    -Dadditional_module_path=
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Sphinx"
    VERBATIM
    COMMAND_EXPAND_LISTS
)

# Add dependency to library
add_dependencies(sphinx ${TARGET_NAME})