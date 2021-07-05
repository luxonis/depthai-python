Code Samples
============

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:

   simple_samples.rst
   complex_samples.rst
   mixed_samples.rst

Code samples are used for automated testing. They are also a great starting point for the gen2 API.

.. rubric:: Simple

- :ref:`RGB Preview` - Displays a small preview of the RGB camera
- :ref:`RGB video` - Displays high resolution frames of the RGB camera
- :ref:`Mono Preview` - Displays right/left mono cameras
- :ref:`Depth Preview` - Displays colorized stereo disparity
- :ref:`RGB Encoding` - Encodes RGB (1080P, 30FPS) into :code:`.h265` and saves it on the host
- :ref:`RGB & Mono Encoding`- Encodes RGB (1080P, 30FPS) and both mono streams (720P, 30FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`Encoding Max Limit` - Encodes RGB (4k 25FPS) and both mono streams (720P, 25FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`RGB Full Resolution Saver` - Saves full resolution RGB images (4k) on the host (:code:`.jpeg`)
- :ref:`Mono Full Resolution Saver` - Saves mono (720P) images to the host (:code:`.png`)
- :ref:`RGB & MobilenetSSD` - Runs MobileNetSSD on RGB frames and displays detections on the frame
- :ref:`RGB & MobileNetSSD @ 4K` - Runs MobileNetSSD on RGB frames and displays detections on both preview and 4k frames
- :ref:`Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame
- :ref:`Video & MobilenetSSD` - Runs MobileNetSSD on the video from the host
- :ref:`IMU Accelerometer & Gyroscope` - Accelerometer and gyroscope at 500hz rate
- :ref:`IMU Rotation Vector` - Rotation vector at 400 hz rate
- :ref:`Edge detector` - Edge detection on input frame

.. rubric:: Complex

- :ref:`RGB Camera Control` - Demonstrates how to control the RGB camera (crop, focus, exposure, sensitivity) from the host
- :ref:`Mono Camera Control` - Demonstrates how to control the mono camera (crop, exposure, sensitivity) from the host
- :ref:`Depth Crop Control` - Demonstrates how to control cropping of depth frames from the host
- :ref:`Stereo Depth from host` - Generates stereo depth frame from a set of mono images from the host
- :ref:`Stereo Depth Video` - An extended version of **Depth Preview**
- :ref:`RGB Rotate Warp` - Demonstrates how to rotate, mirror, flip or perform perspective transform on a frame
- :ref:`RGB Depth` - Displays RGB depth frames
- :ref:`Auto Exposure on ROI` - Demonstrates how to use auto exposure based on the selected ROI
- :ref:`Mono & MobilenetSSD & Depth` - Runs MobileNetSSD on mono frames and displays detections on mono/disparity frames
- :ref:`RGB Encoding & MobilenetSSD` - Runs MobileNetSSD on RGB frames and encodes FUll-HD RGB into :code:`.h265` and saves it on the host
- :ref:`RGB Encoding & Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame + encodes RGB to :code:`.h265`
- :ref:`RGB Encoding & Mono with MobilenetSSD & Depth` - A combination of **RGB Encoding** and **Mono & MobilenetSSD & Depth** code samples
- :ref:`RGB & TinyYoloV3 decoding on device` - Runs YoloV3 on RGB frames and displays detections on the frame
- :ref:`RGB & TinyYoloV4 decoding on device` - Runs YoloV4 on RGB frames and displays detections on the frame
- :ref:`Object tracker on RGB` - Performs object tracking from the RGB camera
- :ref:`Object tracker on video` - Performs object tracking from the video
- :ref:`Spatial location calculator` - Demonstrates how to use the spatial location calculator
- :ref:`Spatial object tracker on RGB` - Performs object tracking and also provides spatial coordinates
- :ref:`RGB & MobilenetSSD with spatial data` - Displays RGB frames with MobileNet detections and spatial coordinates on them
- :ref:`Mono & MobilenetSSD with spatial data` - Displays mono frames with MobileNet detections and spatial coordinates on them
- :ref:`RGB & TinyYolo with spatial data`- Displays RGB frames with Yolo detections and spatial coordinates on them

.. rubric:: Mixed

- :ref:`System information` - Displays device system information (memory/cpu usage, temperature)
- :ref:`OpenCV support` - Demonstrates how to retrieve an image frame as an OpenCV frame
- :ref:`Device Queue Event` - Demonstrates how to use device queue events
- :ref:`Queue add callback` - Demonstrates how to use queue callbacks
- :ref:`Calibration Flash v5` - Demonstrates how to flash calibration data of version 5 (gen1 calibration data) to the device
- :ref:`Calibration Flash` - Demonstrates how to flash calibration data of version 6 (gen2 calibration data) to the device
- :ref:`Calibration Reader` - Demonstrates how to read calibration data stored on device over XLink
- :ref:`Calibration Load` - Demonstrates how to load and use calibration data of version6 (gen2 calibration data) in a pipeline
