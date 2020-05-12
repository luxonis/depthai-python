set(DEPTHAI_SHARED_FOLDER ${CMAKE_CURRENT_LIST_DIR}/depthai-shared)

set(DEPTHAI_SHARED_SOURCES 
    ${DEPTHAI_SHARED_FOLDER}/src/general/data_writer.cpp
    ${DEPTHAI_SHARED_FOLDER}/src/logger/logs_writer.cpp
    ${DEPTHAI_SHARED_FOLDER}/src/stream/stream_info.cpp
    ${DEPTHAI_SHARED_FOLDER}/src/xlink/xlink_wrapper.cpp
    ${DEPTHAI_SHARED_FOLDER}/src/json_helper.cpp
    ${DEPTHAI_SHARED_FOLDER}/src/disparity_luts.cpp
)

set(DEPTHAI_SHARED_INCLUDE
    ${DEPTHAI_SHARED_FOLDER}/include
)
