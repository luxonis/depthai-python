Code Samples
============

.. toctree::
   :hidden:
   :glob:

   ../samples/bootloader/*
   ../samples/calibration/*
   ../samples/ColorCamera/*
   ../samples/crash_report/*
   ../samples/EdgeDetector/*
   ../samples/FeatureTracker/*
   ../samples/host_side/*
   ../samples/ImageManip/*
   ../samples/IMU/*
   ../samples/mixed/*
   ../samples/MobileNet/*
   ../samples/MonoCamera/*
   ../samples/NeuralNetwork/*
   ../samples/ObjectTracker/*
   ../samples/Script/*
   ../samples/SpatialDetection/*
   ../samples/StereoDepth/*
   ../samples/SystemLogger/*
   ../samples/ToF/*
   ../samples/VideoEncoder/*
   ../samples/Warp/*
   ../samples/Yolo/*

Code samples are used for automated testing. They are also a great starting point for the DepthAI API, as different node functionalities
are presented with code.

.. rubric:: Bootloader

- :ref:`Bootloader Config` - Read/Flash/Clear bootloader on the device
- :ref:`Bootloader Version` - Retrieves Version of Bootloader on the device
- :ref:`Flash Bootloader` - Flash the bootloader to the device
- :ref:`POE set IP` - Sets IP of the POE device

.. rubric:: Calibration

- :ref:`Calibration Flash v5` - Flashes calibration data of version 5 (gen1 calibration data) to the device
- :ref:`Calibration Flash` - Flashes calibration data of version 6 (gen2 calibration data) to the device
- :ref:`Calibration Reader` - Reads calibration data stored on device over XLink
- :ref:`Calibration Load` - Loads and uses calibration data of version 6 (gen2 calibration data) in a pipeline

.. rubric:: ColorCamera

- :ref:`Auto Exposure on ROI` - Demonstrates how to use auto exposure based on the selected ROI
- :ref:`RGB Camera Control` - Demonstrates how to control the RGB camera (crop, focus, exposure, sensitivity) from the host
- :ref:`RGB Preview` - Displays a small preview of the RGB camera
- :ref:`RGB scene` - Shows how to select ColorCamera's scene and effect
- :ref:`RGB video` - Displays high resolution frames of the RGB camera

.. rubric:: Crash report

- :ref:`Crash report` - In case of a firmware crash, example reads it from the device and saves it to a json file

.. rubric:: EdgeDetector

- :ref:`Edge Detector` - Performs edge detection on all camera streams

.. rubric:: FeatureTracker

- :ref:`Feature Detector` - Feature detection on input frame
- :ref:`Feature Tracker` - Feature detection and tracking on input frame

.. rubric:: Host side

- :ref:`Latency measurement` - Measure frame latency from capturing to the host
- :ref:`OpenCV support` - Demonstrates how to retrieve an image frame as an OpenCV frame
- :ref:`Device Queue Event` - Demonstrates how to use device queue events
- :ref:`Queue add callback` - Demonstrates how to use queue callbacks

.. rubric:: ImageManip

- :ref:`RGB Rotate Warp` - Demonstrates how to rotate, mirror, flip or perform perspective transform on a frame
- :ref:`ImageManip tiling` - Using ImageManip for frame tiling
- :ref:`ImageManip rotate` - Using ImageManip to rotate color/mono frames

.. rubric:: IMU

- :ref:`IMU Accelerometer & Gyroscope` - Accelerometer and gyroscope at 500hz rate
- :ref:`IMU Rotation Vector` - Rotation vector at 400 hz rate

.. rubric:: Mixed

- :ref:`Mono & MobilenetSSD & Depth` - Runs MobileNetSSD on mono frames and displays detections on mono/disparity frames
- :ref:`Multiple Devices` - Run multiple OAK cameras from the same host computer
- :ref:`RGB Encoding & MobilenetSSD` - Runs MobileNetSSD on RGB frames and encodes FUll-HD RGB into :code:`.h265` and saves it on the host
- :ref:`RGB Encoding & Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame + encodes RGB to :code:`.h265`
- :ref:`RGB Encoding & Mono with MobilenetSSD & Depth` - A combination of **RGB Encoding** and **Mono & MobilenetSSD & Depth** code samples
- :ref:`Spatial detections on rotated OAK` - Spatail detections on upside down OAK camera

.. rubric:: MobileNet

- :ref:`Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame
- :ref:`Video & MobilenetSSD` - Runs MobileNetSSD on the video from the host
- :ref:`RGB & MobilenetSSD` - Runs MobileNetSSD on RGB frames and displays detections on the frame
- :ref:`RGB & MobileNetSSD @ 4K` - Runs MobileNetSSD on RGB frames and displays detections on both preview and 4k frames

.. rubric:: MonoCamera

- :ref:`Mono Preview` - Displays right/left mono cameras
- :ref:`Mono Camera Control` - Demonstrates how to control the mono camera (crop, exposure, sensitivity) from the host
- :ref:`Mono preview - Alternate between dot projector and illumination LED` on OAK Pro devices
- :ref:`Mono Full Resolution Saver` - Saves mono (720P) images to the host (:code:`.png`)

.. rubric:: NeuralNetwork

- :ref:`Multi-Input Frame Concat <Multi-Input Frame Concationation>` - Concat mono/rgb streams on the device with a custom model
- :ref:`Frame Normalization` - Normalize the frame on the device with a custom model

.. rubric:: ObjectTracker

- :ref:`Object tracker on video` - Performs object tracking from the video
- :ref:`Object tracker on RGB` - Performs object tracking from the RGB camera
- :ref:`Spatial object tracker on RGB` - Performs object tracking and also provides spatial coordinates

.. rubric:: Script

- :ref:`Script camera control` - Controlling the camera with the Script node
- :ref:`Script change pipeline flow` - Change the flow of data inside your pipeline in runtime with :ref:`Script` node
- :ref:`Script forward frames` - Forward incoming image stream to two different output streams (demuxing)
- :ref:`Script get local IP` - Get local IP of the device (only OAK-POE devices)
- :ref:`Script HTTP client` - Send HTTP request to a server (only OAK-POE devices)
- :ref:`Script JSON communication` - 2-way JSON communication between Script <-> Host
- :ref:`Script HTTP server` - Serve still image over HTTP response (only OAK-POE devices)
- :ref:`Script MJPEG server` - Serve MJPEG video stream over HTTP response (only OAK-POE devices)
- :ref:`Script NNData example` - Constructs :ref:`NNData` in Script node and sends it to the host
- :ref:`Script UART communication` - UART communication with Script node
- :ref:`Script EMMC access` - Access EMMC memory from the Script node

.. rubric:: SpatialDetection

- :ref:`Spatial Calculator Multi-ROI` - Selects multiple ROIs and calculates spatial coordinates for each of them
- :ref:`Spatial location calculator` - Demonstrates how to use the spatial location calculator
- :ref:`RGB & MobilenetSSD with spatial data` - Displays RGB frames with MobileNet detections and spatial coordinates on them
- :ref:`Mono & MobilenetSSD with spatial data` - Displays mono frames with MobileNet detections and spatial coordinates on them
- :ref:`RGB & TinyYolo with spatial data`- Displays RGB frames with Yolo detections and spatial coordinates on them

.. rubric:: StereoDepth

- :ref:`Depth Crop Control` - Demonstrates how to control cropping of depth frames from the host
- :ref:`Depth Post-Processing` - Depth post-processing filters
- :ref:`Depth Preview` - Displays colorized stereo disparity
- :ref:`Stereo Depth custom Mesh` - Calculate and load custom mesh for stereo depth calculation
- :ref:`Stereo Depth from host` - Generates stereo depth frame from a set of mono images from the host
- :ref:`Stereo Depth Video` - An extended version of **Depth Preview**
- :ref:`RGB Depth alignment` - Displays RGB depth aligned frames

.. rubric:: SystemLogger

- :ref:`System information` - Displays device system information (memory/cpu usage, temperature)

.. rubric:: ToF

- :ref:`ToF depth` - Displays colorized ToF depth frames

.. rubric:: VideoEncoder

- :ref:`Disparity encoding` - Encodes stereo disparity into :code:`.mjpeg`
- :ref:`RGB Encoding` - Encodes RGB (1080P, 30FPS) into :code:`.h265` and saves it on the host
- :ref:`RGB & Mono Encoding`- Encodes RGB (1080P, 30FPS) and both mono streams (720P, 30FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`Encoding Max Limit` - Encodes RGB (4k 25FPS) and both mono streams (720P, 25FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`RGB Full Resolution Saver` - Saves full resolution RGB images (4k) on the host (:code:`.jpeg`)

.. rubric:: Warp

- :ref:`Warp Mesh` - Displays an image warped with 2 different meshes
- :ref:`Interactive Warp Mesh` - Interactively change the warp mesh

.. rubric:: Yolo

- :ref:`RGB & Tiny YOLO` - Runs Tiny YOLO on RGB frames and displays detections on the frame
