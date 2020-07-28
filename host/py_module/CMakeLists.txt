set(APP_NAME depthai)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(dldt_dir ../../shared/3rdparty/dldt)
cmake_minimum_required(VERSION 2.8.12)

project(${APP_NAME})


add_definitions(-D__PC__)
add_definitions(-DUSE_USB_VSC) # for XLink communication
add_definitions(-DXLINK_USB_DATA_TIMEOUT=0)
add_definitions(-DXLINK_COMMON_TIMEOUT_MSEC=0)

add_definitions(-DHOST_PYTHON_MODULE)

# To avoid many "different visibility" warnings during final link on macOS
add_definitions(-fvisibility=hidden)


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++17")   
endif(CMAKE_COMPILER_IS_GNUCXX)

include(${dldt_dir}/inference-engine/thirdparty/movidius/XLink/XLink.cmake)


execute_process(
    COMMAND python3 -c "v = __import__('sys').version_info; \
                        print(v.major, v.minor, sep = '.', end = ''); \
                       "
    OUTPUT_VARIABLE PYTHON_VERSION
    RESULT_VARIABLE PYTHON_ERROR
)
if(PYTHON_ERROR)
    message(FATAL_ERROR "python3 get version error: " ${PYTHON_ERROR})
endif(PYTHON_ERROR)

find_package(PythonLibs   ${PYTHON_VERSION} EXACT REQUIRED)
find_package(PythonInterp ${PYTHON_VERSION} EXACT REQUIRED)
# set(PYTHON_EXECUTABLE "/usr/bin/python3.6-dbg")
# set(PYTHON_EXECUTABLE "/usr/bin/python")

add_subdirectory("3rdparty/pybind11")

include_directories(
    ./
    ../core/
    ../../shared/
    ${XLINK_INCLUDE_DIRECTORIES}
    ${XLINK_PLATFORM_INCLUDE}
    ../../shared/3rdparty/boost_1_71_0/
    ../../shared/3rdparty/json/include/
)

pybind11_add_module(
    ${APP_NAME}
    py_bindings.cpp
    capture_af_bindings.cpp
    ../core/device_support_listener.cpp
    ../core/pipeline/host_pipeline.cpp
    ../core/pipeline/host_pipeline_config.cpp
    ../core/pipeline/cnn_host_pipeline.cpp
    ../core/types.cpp
    ../core/host_json_helper.cpp
    ../core/host_data_reader.cpp
    ../core/host_capture_command.cpp
    ../core/disparity_stream_post_processor.cpp
    ../core/model_downloader.cpp
    ../../shared/json_helper.cpp
    ../../shared/stream/stream_info.cpp
    ../../shared/xlink/xlink_wrapper.cpp
    ${XLINK_SOURCES}
)


# TODO: unite this part of code (duplication with other CMakeLists.txt):


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

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})

# link libraries
target_link_libraries(
   ${APP_NAME}
   PRIVATE
   -L/usr/local/lib  # For macOS
   -lusb-1.0
   -lpthread
   ${CURL_LIBRARIES}
   nlohmann_json::nlohmann_json
   nlohmann_json_schema_validator
)
