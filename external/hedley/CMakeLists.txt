if(NOT TARGET hedley)
    add_library(hedley INTERFACE IMPORTED GLOBAL)
    target_include_directories(hedley INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
endif()