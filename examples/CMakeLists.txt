# Prevent from being configured
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(FATAL_ERROR "Use projects root CMakeLists.txt to configure")
endif()

# Add options
option(DEPTHAI_PYTHON_TEST_EXAMPLES "Test examples - examples will be ran as a part of the test suite" OFF)

# Specify path separator
set(SYS_PATH_SEPARATOR ";")
if(UNIX)
    set(SYS_PATH_SEPARATOR ":")
endif()

# Add a target to install_requirements (added to ALL)
add_custom_target(install_requirements ALL
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/install_requirements.py" "--skip_depthai"
    DEPENDS ${TARGET_NAME}
    VERBATIM
    COMMAND_EXPAND_LISTS
)

# Function for adding new python test
function(add_python_example example_name python_script_path)
    # Modify example name to signify that its Python based
    set(example_name "py_${example_name}")

    # parse the rest of the arguments
    set(arguments ${ARGV})
    list(REMOVE_AT arguments 0 1)

    # Creates a target (python my_test [args])
    add_custom_target(${example_name}
        ${CMAKE_COMMAND} -E env
        # Environment variables
        # Python path (to find compiled module)
        "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
        # ASAN in case of sanitizers
        "${ASAN_ENVIRONMENT_VARS}"
        # Example
        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/${python_script_path} ${ARGN}
        DEPENDS ${TARGET_NAME} install_requirements
        VERBATIM
        COMMAND_EXPAND_LISTS
    )

    if(DEPTHAI_PYTHON_TEST_EXAMPLES)

        # Adds test with 5 seconds timeout and bumps all python warnings to errors
        add_test(NAME ${example_name} COMMAND
            ${CMAKE_COMMAND} -E env
            # Python path (to find compiled module)
            "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
            # ASAN in case of sanitizers
            ${ASAN_ENVIRONMENT_VARS}
            ${CMAKE_COMMAND} -DTIMEOUT_SECONDS=5 -P ${CMAKE_CURRENT_LIST_DIR}/cmake/ExecuteTestTimeout.cmake
            # Actual script to run
            ${PYTHON_EXECUTABLE} -Werror "${CMAKE_CURRENT_LIST_DIR}/${python_script_path}" ${arguments}
        )

        # Sets a regex catching any logged warnings, errors or critical (coming either from device or host)
        set_tests_properties (${example_name} PROPERTIES FAIL_REGULAR_EXPRESSION "\\[warning\\];\\[error\\];\\[critical\\]")

    endif()

endfunction()

if(DEPTHAI_PYTHON_TEST_EXAMPLES)

    # Adds install requirements test with 5 minute timeout
    add_test(NAME install_requirements COMMAND
        ${CMAKE_COMMAND} -E env
        # Python path (to find compiled module)
        "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
        # ASAN in case of sanitizers
        ${ASAN_ENVIRONMENT_VARS}
        ${CMAKE_COMMAND} -DFORCE_TIMEOUT_SECONDS=300 -P ${CMAKE_CURRENT_LIST_DIR}/cmake/ExecuteTestTimeout.cmake
        # Actual script to run
        ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/install_requirements.py" "--skip_depthai"
    )

    # Sets a regex catching any logged warnings, errors or critical (coming either from device or host)
    set_tests_properties (install_requirements PROPERTIES FAIL_REGULAR_EXPRESSION "\\[warning\\];\\[error\\];\\[critical\\]")

endif()

# Add examples
add_python_example(rgb_preview rgb_preview.py)
add_python_example(mono_preview mono_preview.py)
add_python_example(depth_preview depth_preview.py)
add_python_example(rgb_encoding rgb_encoding.py)
add_python_example(rgb_mono_encoding rgb_mono_encoding.py)
add_python_example(rgb_full_resolution_saver rgb_full_resolution_saver.py)
add_python_example(mono_full_resolution_saver mono_full_resolution_saver.py)
add_python_example(rgb_mobilenet rgb_mobilenet.py)
add_python_example(mono_mobilenet mono_mobilenet.py)
add_python_example(mono_depth_mobilenetssd mono_depth_mobilenetssd.py)
add_python_example(rgb_encoding_mono_mobilenet rgb_encoding_mono_mobilenet.py)
add_python_example(rgb_encoding_mono_mobilenet_depth rgb_encoding_mono_mobilenet_depth.py)
add_python_example(encoding_max_limit encoding_max_limit.py)
add_python_example(rgb_camera_control rgb_camera_control.py)
add_python_example(mono_camera_control mono_camera_control.py)
add_python_example(depth_crop_control depth_crop_control.py)
add_python_example(rgb_mobilenet_4k rgb_mobilenet_4k.py)
add_python_example(device_queue_event device_queue_event.py)
add_python_example(video_mobilenet video_mobilenet.py)
add_python_example(rgb_encoding_mobilenet rgb_encoding_mobilenet.py)
add_python_example(rgb_rotate_warp rgb_rotate_warp.py)
add_python_example(tiny_yolo_v3_device_side_decoding tiny_yolo_v3_device_side_decoding.py)
add_python_example(tiny_yolo_v4_device_side_decoding tiny_yolo_v4_device_side_decoding.py)
add_python_example(autoexposure_roi autoexposure_roi.py)
add_python_example(opencv_support opencv_support.py)
add_python_example(system_information system_information.py)
add_python_example(spatial_mobilenet spatial_mobilenet.py)
add_python_example(spatial_mobilenet_mono spatial_mobilenet_mono.py)
add_python_example(spatial_tiny_yolo_v3 spatial_tiny_yolo.py)
add_python_example(spatial_tiny_yolo_v4 spatial_tiny_yolo.py)
add_python_example(spatial_location_calculator spatial_location_calculator.py)
add_python_example(rgb_video rgb_video.py)
add_python_example(object_tracker object_tracker.py)
add_python_example(spatial_object_tracker spatial_object_tracker.py)
add_python_example(object_tracker_video object_tracker_video.py)
add_python_example(stereo_depth_from_host stereo_depth_from_host.py)
add_python_example(stereo_depth_video stereo_depth_video.py)
add_python_example(imu_gyroscope_accelerometer imu_gyroscope_accelerometer.py)
add_python_example(imu_rotation_vector imu_rotation_vector.py)
add_python_example(rgb_depth_aligned rgb_depth_aligned.py)
add_python_example(edge_detector edge_detector.py)
