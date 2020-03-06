set(APP_NAME depthai_app)

set(dldt_dir ../../shared/3rdparty/dldt)

cmake_minimum_required(VERSION 2.8.12)

project(${APP_NAME})


add_definitions(-D__PC__)
add_definitions(-DUSE_USB_VSC) # for XLink communication
# add_definitions(-DXLINK_USB_DATA_TIMEOUT=0)
# add_definitions(-DXLINK_COMMON_TIMEOUT_MSEC=0)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++17")   
endif(CMAKE_COMPILER_IS_GNUCXX)


# find_package(OpenCV REQUIRED )
# find_package(PythonLibs 3 REQUIRED)

include(${dldt_dir}/inference-engine/thirdparty/movidius/XLink/XLink.cmake)

include_directories(
    ./
    ../core/
    ../../shared/
    ${XLINK_INCLUDE_DIRECTORIES}
)



# if(DEFINED BUILD_FOR_PYTHON)
#     message("building for python")
#     add_subdirectory(pybind11)
#     pybind11_add_module(
#         ${APP_NAME}        
#         wrapper.cpp
#         program_raw_options.cpp
#         ../core/pipeline/host_pipeline.cpp
#         ../../shared/xlink/xlink_wrapper.cpp
#         ${XLINK_SOURCES}
#     )
# else(DEFINED BUILD_FOR_PYTHON)
    message("building for direct")
    add_executable( ${APP_NAME}
        main.cpp
        program_raw_options.cpp
        ../../shared/logger/logs_writer.cpp
        ../../host/core/3rdparty/loguru/loguru.cpp
        ../../shared/general/data_writer.cpp
        ../../shared/json_helper.cpp
        host_logs_writer.cpp
        ../core/pipeline/host_pipeline.cpp
        ../core/pipeline/host_pipeline_config.cpp
        ../core/pipeline/cnn_host_pipeline.cpp
        ../../shared/stream/stream_info.cpp
        ../../shared/xlink/xlink_wrapper.cpp
        ../core/types.cpp
        ../core/host_json_helper.cpp
        ../core/host_data_reader.cpp
        ../core/host_data_reader.cpp
        ../core/disparity_stream_post_processor.cpp        
        ../../shared/disparity_luts.cpp
        ${XLINK_SOURCES}
    )
# endif(DEFINED BUILD_FOR_PYTHON)



# nlohman JSON
set(nlohmann_json_DIR ../../shared/3rdparty/json)

include_directories(${nlohmann_json_DIR}/include/)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${nlohmann_json_DIR} ${CMAKE_CURRENT_BINARY_DIR}/json)


# nlohman JSON validator
set(BUILD_TESTS OFF CACHE INTERNAL "")
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../shared/3rdparty/json-schema-validator/ ${CMAKE_CURRENT_BINARY_DIR}/json-schema-validator)



# for commit hash
# TODO: maybe we shoud add it into another .cmake and than include here?
find_package(Git)

set(commit_version "unknown")

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE commit_version
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else()
  message(STATUS "GIT module not found")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../shared/version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/version.hpp @ONLY)



# link libraries
target_link_libraries(
   ${APP_NAME}
   PRIVATE
   -lusb-1.0
   -lpthread
   -ldl
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
)
