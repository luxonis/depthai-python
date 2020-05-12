cmake_minimum_required(VERSION 3.2)

# Add depthai-cpp dependency
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../depthai-cpp ${CMAKE_CURRENT_BINARY_DIR}/depthai-cpp EXCLUDE_FROM_ALL)

set(TARGET_NAME myapp)
project(${TARGET_NAME})

# Add source files
add_executable("${TARGET_NAME}" 
    main.cpp
)

# Link with libraries
target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        depthai-cpp
)
