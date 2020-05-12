cmake_minimum_required(VERSION 3.2) # For Hunter

# Set compile with -fPIC (on all targets)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.253.tar.gz"
    SHA1 "88ea6d37c897a81a080eb9ae0f69d7807bbb3c73"
    FILEPATH ${CMAKE_CURRENT_LIST_DIR}/depthai-cpp/cmake/Hunter/config.cmake # Add depthai-cpp config (hunter limitation)
)

# Pybindings project
set(TARGET_NAME depthai)
project(${TARGET_NAME})

# Add depthai-cpp dependency
add_subdirectory(depthai-cpp EXCLUDE_FROM_ALL)

# Add pybind11 dependency
#add_subdirectory(pybind11-2.5.0)
hunter_add_package(pybind11)
find_package(pybind11 CONFIG REQUIRED)
# Find python lib
#find_package(PythonLibs REQUIRED)

# Add files for python module
pybind11_add_module(${TARGET_NAME} 
    src/py_bindings.cpp
    src/host_data_packet_bindings.cpp
    src/nnet_packet_bindings.cpp
    src/py_tensor_entry_container_iterator.cpp
    src/device_bindings.cpp
)
# Link with libraries
target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        # pybind11
        pybind11::pybind11
        #pybind11::embed 
        #pybind11::module
        depthai-cpp
)
