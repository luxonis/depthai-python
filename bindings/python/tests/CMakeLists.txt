
set(TARGET_TEST_MODULE "depthai_pybind11_tests")

# Specify path separator
set(SYS_PATH_SEPARATOR ";")
if(UNIX)
    set(SYS_PATH_SEPARATOR ":")
endif()

set(PYBIND11_TEST_FILES
    "xlink_exceptions_test.cpp"
    "utf8_support_test.py"
    "dai_path_conversion_test.py"
    "nndata_tensor_test.py"
)

string(REPLACE ".cpp" ".py" PYBIND11_PYTEST_FILES "${PYBIND11_TEST_FILES}")

# Create the binding library at the end
pybind11_add_module(${TARGET_TEST_MODULE} THIN_LTO ${TARGET_TEST_MODULE}.cpp ${PYBIND11_TEST_FILES})

# A single command to compile and run the tests
add_custom_target(
    pytest COMMAND
        ${CMAKE_COMMAND} -E env
        # PATH (dlls)
        "PATH=${HUNTER_INSTALL_PREFIX}/bin${SYS_PATH_SEPARATOR}$ENV{PATH}"
        # Python path (to find compiled modules)
        "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$<TARGET_FILE_DIR:${TARGET_TEST_MODULE}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
        # ASAN in case of sanitizers
        ${ASAN_ENVIRONMENT_VARS}
        ${PYTHON_EXECUTABLE} -m pytest ${PYBIND11_PYTEST_FILES}
    DEPENDS
        ${TARGET_TEST_MODULE} # Compiled tests
        ${TARGET_NAME} # DepthAI Python Library
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    USES_TERMINAL
)

# Link to depthai
target_link_libraries(${TARGET_TEST_MODULE} PRIVATE pybind11::pybind11 depthai::core)