# This list specifies building of documentation using CMake

# Sphinx is required to build python documentation
find_package(Sphinx REQUIRED)

# Set input and output
set(SPHINX_SOURCE ${CMAKE_CURRENT_LIST_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)

# Get doxygen build location form doxygen target
get_target_property(DOXYGEN_OUTPUT_DIR doxygen DOXYGEN_OUTPUT_DIR)
message(STATUS "Retrieved doxygen output dir from doxygen target: ${DOXYGEN_OUTPUT_DIR}")

set(SYS_PATH_SEPARATOR ";")
if(UNIX)
    set(SYS_PATH_SEPARATOR ":")
endif()

add_custom_target(sphinx ALL
    ${CMAKE_COMMAND} -E env 
    # Environment variables
    # Python path (to find compiled module)
    "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
    # ASAN in case of sanitizers
    "${ASAN_ENVIRONMENT_VARS}" 
    ############ SPHINX
    ${SPHINX_EXECUTABLE} -b html
    # Tell Breathe where to find the Doxygen output
    -Dbreathe_projects.depthai-core=${DOXYGEN_OUTPUT_DIR}/xml
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Sphinx"
    VERBATIM
    COMMAND_EXPAND_LISTS
)

# Add dependency to library
add_dependencies(sphinx ${TARGET_NAME})