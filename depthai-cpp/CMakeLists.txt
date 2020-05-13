cmake_minimum_required(VERSION 3.2) # For Hunter

# Set defaults
set(HUNTER_CONFIGURATION_TYPES "Release" CACHE STRING "Hunter dependencies list of build configurations")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.253.tar.gz"
    SHA1 "88ea6d37c897a81a080eb9ae0f69d7807bbb3c73"
    LOCAL # Local config for dependencies
)

# Move binary dir if windows, to shorten the path
if(WIN32)
    set(HUNTER_BINARY_DIR "${HUNTER_GATE_ROOT}/_hunter_bin" CACHE STRING "Hunter binary directory")
endif()

set(TARGET_NAME depthai-cpp)
project(${TARGET_NAME})


### Get and find dependencies

# Nlohmann JSON
hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

# json schema validator
hunter_add_package(nlohmann_json_schema_validator)
find_package(nlohmann_json_schema_validator CONFIG REQUIRED)

# XLink
hunter_add_package(XLink)
find_package(XLink CONFIG REQUIRED)

# BZip2 (for bspatch)
hunter_add_package(BZip2)
find_package(BZip2 CONFIG REQUIRED)

# Boost
hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

# Add depthai-shared, and definitions that it is PC side
include(${CMAKE_CURRENT_LIST_DIR}/shared/depthai-shared.cmake)

# Add threads (c++)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

### End of dependencies


# Create library
add_library(${TARGET_NAME}
    # depthai-shared sources
    "${DEPTHAI_SHARED_SOURCES}"
    # sources
    src/pipeline/cnn_host_pipeline.cpp
    src/pipeline/host_pipeline_config.cpp
    src/pipeline/host_pipeline.cpp
    src/pipeline/pipeline_builder.cpp
    src/device_support_listener.cpp
    src/disparity_stream_post_processor.cpp
    src/host_data_reader.cpp
    src/host_json_helper.cpp
    src/types.cpp
    src/device.cpp
    src/bspatch/bspatch.c
)

# Set compiler features (c++11)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 11)

# Add cmake folder to modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

# First specify options
option(DEPTHAI_BINARIES_RESOURCE_COMPILE "Compile Depthai device side binaries into library" ON)
option(DEPTHAI_USB2_PATCH_ONLY_MODE "Use patch file and full depthai.cmd binary for usb2 mode" ON)
option(DEPTHAI_CMD_PATH "Use local path for depthai.cmd instead of downloading" OFF)
if(DEPTHAI_USB2_PATCH_ONLY_MODE)
    option(DEPTHAI_USB2_PATCH_PATH "Use local path for depthai-usb2-patch.patch instead of downloading" OFF)
else()
    option(DEPTHAI_USB2_CMD_PATH "Use local path for depthai-usb2.cmd instead of downloading" OFF)
endif()

if(DEPTHAI_USB2_PATCH_ONLY_MODE)
    message(STATUS "Compiling ${TARGET_NAME} resources in PATCH_ONLY mode")
else()
    message(STATUS "Compiling ${TARGET_NAME} depthai and depthai-usb2 resources")
endif()

# Set constant
set(DEPTHAI_RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

# Include configuration
include(Depthai/DepthaiDeviceSideConfig)

# Download libraries
include(DepthaiDownloader)

# Then get the Depthai device side binaries (local or download)
if(DEPTHAI_CMD_PATH OR DEPTHAI_USB2_CMD_PATH OR DEPTHAI_USB2_PATCH_PATH)
    # Atleast one of the paths is set. include binaries locally
    message(STATUS "Using local Depthai device side binaries...")

    DepthaiLocal(
        PATCH_ONLY ${DEPTHAI_USB2_PATCH_ONLY_MODE}
        "${DEPTHAI_RESOURCES_OUTPUT_DIR}"            # Output folder
        DEPTHAI_RESOURCE_LIST                       # List of output resources
        "${DEPTHAI_CMD_PATH}"                       # depthai.cmd
        "${DEPTHAI_USB2_CMD_PATH}"                  # depthai-usb2.cmd
        "${DEPTHAI_USB2_PATCH_PATH}"                # depthai-usb2-patch.patch
    )

else()
    # No user specified paths, download from server
    message(STATUS "Downloading Depthai device side binaries from server...")
    
    DepthaiDownload(
        PATCH_ONLY ${DEPTHAI_USB2_PATCH_ONLY_MODE}
        "${DEPTHAI_RESOURCES_OUTPUT_DIR}"            # Output folder 
        DEPTHAI_RESOURCE_LIST                       # List of output resources
        "${DEPTHAI_DEVICE_SIDE_MATURITY}"           # Maturity
        "${DEPTHAI_DEVICE_SIDE_COMMIT}"             # commit hash
        "${DEPTHAI_DEVICE_SIDE_VERSION}"            # Optional version
    )
endif()


message(STATUS "LIST OF RESOURCES: ${DEPTHAI_RESOURCE_LIST}")

if(DEPTHAI_BINARIES_RESOURCE_COMPILE)
    # Add RC and resource compile the binares
    include(CMakeRC)

    set(DEPTHAI_RESOURCE_LIBRARY_NAME "depthai-resources")

    # Add resource library
    cmrc_add_resource_library("${DEPTHAI_RESOURCE_LIBRARY_NAME}" NAMESPACE depthai
        WHENCE "${DEPTHAI_RESOURCES_OUTPUT_DIR}"
        "${DEPTHAI_RESOURCE_LIST}"
    )    

    # Link to resource library
    target_link_libraries(${TARGET_NAME} PRIVATE "${DEPTHAI_RESOURCE_LIBRARY_NAME}")

    # Set define that binaries are resource compiled
    target_compile_definitions(${TARGET_NAME} PRIVATE DEPTHAI_RESOURCE_COMPILED_BINARIES)

else()
    # TODO
    # Don't add RC and don't resource compile the binaries

endif()


# Add include directories
target_include_directories(${TARGET_NAME}   
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<BUILD_INTERFACE:${DEPTHAI_SHARED_INCLUDE}>"

    #INTERFACE
    #    # ...
    PRIVATE 
        "${CMAKE_CURRENT_LIST_DIR}/include/bspatch"
)


# link libraries
target_link_libraries(${TARGET_NAME}
    PUBLIC
        nlohmann_json::nlohmann_json
        nlohmann_json_schema_validator
        XLink
        Boost::boost
    PRIVATE
        Threads::Threads
        BZip2::bz2
)


# Add definition that it is PC side
target_compile_definitions(${TARGET_NAME} PRIVATE -D__PC__)

# Add patch only mode definition
if(DEPTHAI_USB2_PATCH_ONLY_MODE)
    target_compile_definitions(${TARGET_NAME} PRIVATE DEPTHAI_PATCH_ONLY_MODE)
endif()

# INSTALLATION steps
include(GNUInstallDirs)
install(
    TARGETS 
        ${TARGET_NAME} "${DEPTHAI_RESOURCE_LIBRARY_NAME}" cmrc-base
    EXPORT 
        "${TARGET_NAME}Config"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)


install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/depthai")

# Add resource installation if not RC'd
if(DEPTHAI_BINARIES_RESOURCE_COMPILE)
    install(DIRECTORY "${DEPTHAI_RESOURCES_OUTPUT_DIR}/" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/depthai")
endif()

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}")
export(TARGETS
    ${TARGET_NAME} "${DEPTHAI_RESOURCE_LIBRARY_NAME}" cmrc-base
    FILE "${config_install_dir}/${TARGET_NAME}Config.cmake"
)
install(EXPORT
    "${TARGET_NAME}Config"
    DESTINATION "${config_install_dir}"
)