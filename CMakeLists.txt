cmake_minimum_required(VERSION 3.2) # For Hunter

# Set defaults
# PIC toolchain as we are building a shared library
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain/pic.cmake" CACHE STRING "")
# Build dependencies as 'Release' only by default
set(HUNTER_CONFIGURATION_TYPES "Release" CACHE STRING "Hunter dependencies list of build configurations")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.258.tar.gz"
    SHA1 "062a19ab13ce8dffa9a882b6ce3e43bdabdf75d3"
    FILEPATH ${CMAKE_CURRENT_LIST_DIR}/depthai-core/cmake/Hunter/config.cmake # Add depthai-core config (hunter limitation)
)

# Move binary dir if windows, to shorten the path
if(WIN32)
    set(HUNTER_BINARY_DIR "${HUNTER_GATE_ROOT}/_bin" CACHE STRING "Hunter binary directory")
endif()

# Pybindings project
set(TARGET_NAME depthai)
project(${TARGET_NAME} VERSION "1") # revision of bindings [depthai-core].[rev]

# First specify options
option(DEPTHAI_PYTHON_ENABLE_TESTS "Enable tests" OFF)
option(DEPTHAI_PYTHON_TEST_EXAMPLES "Test examples - examples will be ran as a part of the test suite" OFF)

# Add external dependencies
add_subdirectory(external)

# Add depthai-cpp dependency
add_subdirectory(depthai-core)

# Add pybind11 dependency
#add_subdirectory(pybind11-2.5.0)
hunter_add_package(pybind11)

# Disable LTO if MINGW compiler
if(MINGW)   
    set(PYBIND11_LTO_CXX_FLAGS "" CACHE STRING "" FORCE)
endif()
find_package(pybind11 CONFIG REQUIRED)

# Add files for python module
pybind11_add_module(${TARGET_NAME} 
    src/py_bindings.cpp
    src/XLinkConnectionBindings.cpp
    src/DeviceBindings.cpp
    src/DeviceBootloaderBindings.cpp
    src/DatatypeBindings.cpp
    src/DataQueueBindings.cpp
    src/pipeline/PipelineBindings.cpp
    src/pipeline/NodeBindings.cpp
    src/pipeline/CommonBindings.cpp
    src/pipeline/AssetManagerBindings.cpp
    src/openvino/OpenVINOBindings.cpp
    src/log/LogBindings.cpp
)

# Add include directory (src)
target_include_directories(${TARGET_NAME} PRIVATE src)

# Link with libraries
target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        # pybind11
        pybind11::pybind11
        depthai-core
        hedley
)

# Add bindings revision
target_compile_definitions(${TARGET_NAME} PRIVATE DEPTHAI_PYTHON_BINDINGS_REVISION="${PROJECT_VERSION}")

# Add default commit hash (dev) if not build by CI
if(NOT DEFINED ENV{CI} AND NOT DEPTHAI_PYTHON_COMMIT_HASH)   
    set(DEPTHAI_PYTHON_COMMIT_HASH dev)
endif()

# Add compile definition for bindings
if(DEPTHAI_PYTHON_COMMIT_HASH)
    target_compile_definitions(${TARGET_NAME} PRIVATE DEPTHAI_PYTHON_COMMIT_HASH="${DEPTHAI_PYTHON_COMMIT_HASH}")
endif()

# Set compiler features (c++14), and disables extensions
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

########################
# Testing
########################
if(DEPTHAI_PYTHON_ENABLE_TESTS OR DEPTHAI_PYTHON_TEST_EXAMPLES)
    include(CTest)
    enable_testing()
endif()


########################
# Tests
########################
if (DEPTHAI_PYTHON_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

########################
# Examples (can also act as tests)
########################
if (DEPTHAI_PYTHON_TEST_EXAMPLES)
    add_subdirectory(examples)
endif()

