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
        # PATH (dlls)
        "PATH=${HUNTER_INSTALL_PREFIX}/bin${SYS_PATH_SEPARATOR}$ENV{PATH}"
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
            # PATH (dlls)
            "PATH=${HUNTER_INSTALL_PREFIX}/bin${SYS_PATH_SEPARATOR}$ENV{PATH}"
            # Python path (to find compiled module)
            "PYTHONPATH=$<TARGET_FILE_DIR:${TARGET_NAME}>${SYS_PATH_SEPARATOR}$ENV{PYTHONPATH}"
            # ASAN in case of sanitizers
            ${ASAN_ENVIRONMENT_VARS}
            ${CMAKE_COMMAND} -DTIMEOUT_SECONDS=10 -P ${CMAKE_CURRENT_LIST_DIR}/cmake/ExecuteTestTimeout.cmake
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
        # PATH (dlls)
        "PATH=${HUNTER_INSTALL_PREFIX}/bin${SYS_PATH_SEPARATOR}$ENV{PATH}"
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

## Bootloader
# add_python_example(bootloader_config bootloader/bootloader_config.py)
add_python_example(bootloader_version bootloader/bootloader_version.py)
# add_python_example(flash_bootloader bootloader/flash_bootloader.py)

## Calibration
# add_python_example(calibration_flash_v5 calibration/calibration_flash_v5.py)
# add_python_example(calibration_flash calibration/calibration_flash.py)
add_python_example(calibration_load calibration/calibration_load.py)
add_python_example(calibration_reader calibration/calibration_reader.py)

## ColorCamera
add_python_example(autoexposure_roi ColorCamera/autoexposure_roi.py)
add_python_example(rgb_camera_control ColorCamera/rgb_camera_control.py)
add_python_example(rgb_preview ColorCamera/rgb_preview.py)
add_python_example(rgb_scene ColorCamera/rgb_scene.py)
add_python_example(rgb_video ColorCamera/rgb_video.py)
add_python_example(rgb_isp_scale ColorCamera/rgb_isp_scale.py)

## EdgeDetector
add_python_example(edge_detector EdgeDetector/edge_detector.py)

## FeatureTracker
add_python_example(feature_tracker FeatureTracker/feature_tracker.py)
add_python_example(feature_tracker_color FeatureTracker/feature_tracker_color.py)
add_python_example(feature_detector FeatureTracker/feature_detector.py)

## HostSide
add_python_example(device_queue_event host_side/device_queue_event.py)
add_python_example(opencv_support host_side/opencv_support.py)
add_python_example(queue_add_callback host_side/queue_add_callback.py)

## ImageManip
add_python_example(image_manip_rotate ImageManip/image_manip_rotate.py)
add_python_example(image_manip_tiling ImageManip/image_manip_tiling.py)
add_python_example(rgb_rotate_warp ImageManip/rgb_rotate_warp.py)
add_python_example(image_manip_warp_mesh ImageManip/image_manip_warp_mesh.py)
add_python_example(crop_input_chunck_oom issue_solved/crop_input_chunck_oom.py)

## IMU
add_python_example(imu_gyroscope_accelerometer IMU/imu_gyroscope_accelerometer.py)
add_python_example(imu_rotation_vector IMU/imu_rotation_vector.py)

## Mixed
add_python_example(mono_depth_mobilenetssd mixed/mono_depth_mobilenetssd.py)
add_python_example(rgb_encoding_mobilenet mixed/rgb_encoding_mobilenet.py)
add_python_example(rgb_encoding_mono_mobilenet mixed/rgb_encoding_mono_mobilenet.py)
add_python_example(rgb_encoding_mono_mobilenet_depth mixed/rgb_encoding_mono_mobilenet_depth.py)

## MobileNet
add_python_example(mono_mobilenet MobileNet/mono_mobilenet.py)
add_python_example(rgb_mobilenet_4k MobileNet/rgb_mobilenet_4k.py)
add_python_example(rgb_mobilenet MobileNet/rgb_mobilenet.py)
add_python_example(video_mobilenet MobileNet/video_mobilenet.py)

## MonoCamera
add_python_example(mono_preview MonoCamera/mono_preview.py)
add_python_example(mono_full_resolution_saver MonoCamera/mono_full_resolution_saver.py)
add_python_example(mono_camera_control MonoCamera/mono_camera_control.py)

## NeuralNetwork
# add_python_example(concat_multiple_input NeuralNetwork/concat_multiple_input.py)
# add_python_example(normalization_multiple_input NeuralNetwork/normalization_multiple_input.py)

## ObjectTracker
add_python_example(object_tracker_video ObjectTracker/object_tracker_video.py)
add_python_example(object_tracker ObjectTracker/object_tracker.py)
add_python_example(spatial_object_tracker ObjectTracker/spatial_object_tracker.py)
# add_python_example(spatial_object_tracker_yolo ObjectTracker/spatial_object_tracker_yolo.py)

## Script
add_python_example(script_camera_control Script/script_camera_control.py)
add_python_example(script_json_communication Script/script_json_communication.py)
add_python_example(script_get_device_info Script/script_get_device_info.py)

## SpatialDetection
add_python_example(spatial_tiny_yolo_v3 SpatialDetection/spatial_tiny_yolo.py yolo3)
add_python_example(spatial_tiny_yolo_v4 SpatialDetection/spatial_tiny_yolo.py yolo4)
add_python_example(spatial_mobilenet SpatialDetection/spatial_mobilenet.py)
add_python_example(spatial_mobilenet_mono SpatialDetection/spatial_mobilenet_mono.py)
add_python_example(spatial_location_calculator SpatialDetection/spatial_location_calculator.py)

## StereoDepth
add_python_example(depth_crop_control StereoDepth/depth_crop_control.py)
add_python_example(depth_preview StereoDepth/depth_preview.py)
add_python_example(depth_post_processing StereoDepth/depth_post_processing.py)
add_python_example(rgb_depth_aligned StereoDepth/rgb_depth_aligned.py)
add_python_example(rgb_depth_confidence_aligned StereoDepth/rgb_depth_confidence_aligned.py)
add_python_example(stereo_depth_from_host StereoDepth/stereo_depth_from_host.py)
add_python_example(stereo_depth_video StereoDepth/stereo_depth_video.py)

## SystemLogger
add_python_example(system_information SystemLogger/system_information.py)

## VideoEncoder
add_python_example(encoding_max_limit VideoEncoder/encoding_max_limit.py)
add_python_example(rgb_encoding VideoEncoder/rgb_encoding.py)
add_python_example(rgb_full_resolution_saver VideoEncoder/rgb_full_resolution_saver.py)
add_python_example(rgb_mono_encoding VideoEncoder/rgb_mono_encoding.py)

## Yolo
add_python_example(tiny_yolo_v3_device_side_decoding Yolo/tiny_yolo.py yolo3)
add_python_example(tiny_yolo_v4_device_side_decoding Yolo/tiny_yolo.py yolo4)

## AprilTag
add_python_example(apriltag AprilTag/apriltag.py)
add_python_example(apriltag_rgb AprilTag/apriltag_rgb.py)
