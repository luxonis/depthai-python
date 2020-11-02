cmake_minimum_required(VERSION 3.2) # For Hunter

# Set compile with -fPIC (on all targets)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set defaults
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
project(${TARGET_NAME} VERSION "0") # revision of bindings [depthai-core].[rev]

# Add depthai-cpp dependency
add_subdirectory(depthai-core EXCLUDE_FROM_ALL)

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
    src/host_data_packet_bindings.cpp
    src/nnet_packet_bindings.cpp
    src/device_bindings.cpp
)

# Link with libraries
target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        # pybind11
        pybind11::pybind11
        depthai-core
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

