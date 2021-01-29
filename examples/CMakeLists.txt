if(DEPTHAI_PYTHON_TEST_EXAMPLES)

    # Macro for adding new python test
    macro(dai_add_python_test test_name python_script_path)
        # Creates a test (python my_test [args])
        #add_test(NAME ${test_name} COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/${python_script_path}" ${ARGN})
        
        # parse the rest of the arguments
        set(arguments ${ARGV})
        list(REMOVE_AT arguments 0 1)

        add_test(NAME ${test_name} COMMAND ${CMAKE_COMMAND} 
            -DTIMEOUT_SECONDS=5
            -P ${CMAKE_CURRENT_LIST_DIR}/cmake/ExecuteTestTimeout.cmake ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/${python_script_path}" ${arguments}
        )
        
        # Points python to our build module
        set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PYTHONPATH=$<TARGET_FILE_DIR:depthai>")

    endmacro() 

    ## Retrieve resources

    # Mobilenet resource
    hunter_private_data(
        URL "https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet.blob"
        SHA1 "e89d3ee9f26d80397e44f89c6b375990064a4a42"
        FILE "mobilenet.blob"
        LOCATION mobilenet_blob
    )

    # Construction vest video resource
    hunter_private_data(
        URL "http://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/construction_vest.mp4"
        SHA1 "271d8d0b702e683ce02957db7c100843de5ceaec"
        FILE "construction_vest.mp4"
        LOCATION construction_vest
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
    dai_add_python_test(15_rgb_mobilenet_4k 15_rgb_mobilenet_4k.py "${mobilenet_blob}")
    dai_add_python_test(16_device_queue_event 16_device_queue_event.py)
    

endif()
