if(DEPTHAI_PYTHON_TEST_EXAMPLES)

    # Macro for adding new python test
    macro(dai_add_python_test test_name python_script_path)
        # Creates a test (python my_test [args])
        #add_test(NAME ${test_name} COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/${python_script_path}" ${ARGN})
        
        # parse the rest of the arguments
        set(arguments ${ARGV})
        list(REMOVE_AT arguments 0 1)

        # Adds test with 5 seconds timeout and bumps all python warnings to errors
        add_test(NAME ${test_name} COMMAND ${CMAKE_COMMAND} 
            -DTIMEOUT_SECONDS=5
            -P ${CMAKE_CURRENT_LIST_DIR}/cmake/ExecuteTestTimeout.cmake ${PYTHON_EXECUTABLE} -Werror "${CMAKE_CURRENT_LIST_DIR}/${python_script_path}" ${arguments}
        )
        
        # Points python to our build module
        set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PYTHONPATH=$<TARGET_FILE_DIR:depthai>")

        # Sets a regex catching any logged warnings, errors or critical (coming either from device or host)
        set_tests_properties (${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION "\\[warning\\];\\[error\\];\\[critical\\]") 

    endmacro() 

    ## Retrieve resources

    # Mobilenet resource
    hunter_private_data(
        URL "https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_6shave.blob"
        SHA1 "f0e14978b3f77a4f93b9f969cd39e58bb7aef490"
        FILE "mobilenet-ssd_openvino_2021.2_6shave.blob"
        LOCATION mobilenet_blob
    )

    # Mobilenet resource
    hunter_private_data(
        URL "https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_5shave.blob"
        SHA1 "d715f85e474609cf3f696d7a2e3750804ed6c726"
        FILE "mobilenet-ssd_openvino_2021.2_5shave.blob"
        LOCATION mobilenet_5shave_blob
    )

    # Construction vest video resource
    hunter_private_data(
        URL "http://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/construction_vest.mp4"
        SHA1 "271d8d0b702e683ce02957db7c100843de5ceaec"
        FILE "construction_vest.mp4"
        LOCATION construction_vest
    )

    hunter_private_data(
        URL "https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v3_openvino_2021.2_6shave.blob"
        SHA1 "f0ac263a0d55c374e1892eea21c9b7d1170bde46"
        FILE "tiny-yolo-v3_openvino_2021.2_6shave.blob"
        LOCATION tiny_yolo_v3_blob
    )

    # Add tests
    dai_add_python_test(01_rgb_preview 01_rgb_preview.py)
    dai_add_python_test(02_mono_preview 02_mono_preview.py)
    dai_add_python_test(03_depth_preview 03_depth_preview.py)
    dai_add_python_test(04_rgb_encoding 04_rgb_encoding.py)
    dai_add_python_test(05_rgb_mono_encoding 05_rgb_mono_encoding.py)
    dai_add_python_test(06_rgb_full_resolution_saver 06_rgb_full_resolution_saver.py)
    dai_add_python_test(07_mono_full_resolution_saver 07_mono_full_resolution_saver.py)
    dai_add_python_test(08_rgb_mobilenet 08_rgb_mobilenet.py "${mobilenet_blob}")
    dai_add_python_test(09_mono_mobilenet 09_mono_mobilenet.py "${mobilenet_blob}")
    dai_add_python_test(10_mono_depth_mobilenetssd 10_mono_depth_mobilenetssd.py "${mobilenet_blob}")
    dai_add_python_test(11_rgb_encoding_mono_mobilenet 11_rgb_encoding_mono_mobilenet.py "${mobilenet_blob}")
    dai_add_python_test(12_rgb_encoding_mono_mobilenet_depth 12_rgb_encoding_mono_mobilenet_depth.py "${mobilenet_blob}")
    dai_add_python_test(13_encoding_max_limit 13_encoding_max_limit.py)
    dai_add_python_test(14_color_camera_control 14_color_camera_control.py)
    dai_add_python_test(15_rgb_mobilenet_4k 15_rgb_mobilenet_4k.py "${mobilenet_5shave_blob}")
    dai_add_python_test(16_device_queue_event 16_device_queue_event.py)
    dai_add_python_test(17_video_mobilenet 17_video_mobilenet.py "${mobilenet_blob}" "${construction_vest}")
    dai_add_python_test(18_rgb_encoding_mobilenet 18_rgb_encoding_mobilenet.py "${mobilenet_blob}")
    dai_add_python_test(19_mono_camera_control 19_mono_camera_control.py)
    dai_add_python_test(20_color_rotate_warp 20_color_rotate_warp.py)
    dai_add_python_test(21_mobilenet_device_side_decoding 21_mobilenet_device_side_decoding.py "${mobilenet_blob}")
    dai_add_python_test(22_tiny_yolo_v3_device_side_decoding 22_tiny_yolo_v3_device_side_decoding.py "${tiny_yolo_v3_blob}")
    dai_add_python_test(23_autoexposure_roi 23_autoexposure_roi.py "${mobilenet_blob}")
    dai_add_python_test(24_opencv_support 24_opencv_support.py)

endif()
