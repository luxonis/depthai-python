Code samples
============

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:
   :caption: Code samples:

   ../samples/*

Code samples are used for automated testing. They are also a great starting point for the gen2 API.

.. rubric:: List of code samples

- :ref:`01 - RGB Preview` - Displays a small preview of the RGB camera
- :ref:`02 - Mono Preview` - Displays right/left mono cameras
- :ref:`03 - Depth Preview` - Displays colorized stereo disparity
- :ref:`04 - RGB Encoding` - Encodes RGB (1080P, 30FPS) into :code:`.h265` and saves it on the host
- :ref:`05 - RGB & Mono Encoding`- Encodes RGB (1080P, 30FPS) and both mono streams (720P, 30FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`06 - RGB Full Resolution Saver` - Saves full resolution RGB images (4k) on the host (:code:`.jpeg`)
- :ref:`07 - Mono Full Resolution Saver` - Saves mono (720P) images to the host (:code:`.png`)
- :ref:`08 - RGB & MobilenetSSD` - Runs MobileNetSSD on RGB frames and displays detections on the frame
- :ref:`09 - Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame
- :ref:`10 - Mono & MobilenetSSD & Depth` - Runs MobileNetSSD on mono frames and displays detections on mono/disparity frames
- :ref:`11 - RGB & Encoding & Mono & MobilenetSSD` - Runs MobileNetSSD on mono frames and displays detections on the frame + encodes RGB to :code:`.h265`
- :ref:`12 - RGB Encoding & Mono with MobilenetSSD & Depth` - A combination of **04** and **10** code samples
- :ref:`13 - Encoding Max Limit` - Encodes RGB (4k 25FPS) and both mono streams (720P, 25FPS) into :code:`.h265`/:code:`.h264` and saves them on the host
- :ref:`14.1 - Color Camera Control` - Demonstrates how to control the color camera (crop, focus, exposure, sensitivity) from the host
- :ref:`14.2 - Mono Camera Control` - Demonstrates how to control the mono camera (crop, exposure, sensitivity) from the host
- :ref:`14.3 - Depth Crop Control` - Demonstrates how to control cropping of depth frames from the host
- :ref:`15 - 4K RGB MobileNetSSD` - Runs MobileNetSSD on RGB frames and displays detections on both preview and 4k frames
- :ref:`16 - Device Queue Event` - Demonstrates how to use device queue events
- :ref:`17 - Video & MobilenetSSD` - Runs MobileNetSSD on the video from the host
- :ref:`18 - RGB Encoding with MobilenetSSD` - Runs MobileNetSSD on RGB frames and encodes FUll-HD RGB into :code:`.h265` and saves it on the host
- :ref:`19 - Mono Camera Control` - Demonstrates how to control the mono camera (exposure, sensitivity) from the host
- :ref:`20 - Color Rotate Warp` - Demonstrates how to rotate, mirror, flip or perform perspective transform on a frame
- :ref:`22.1 - RGB & TinyYoloV3 decoding on device` - Runs YoloV3 on RGB frames and displays detections on the frame
- :ref:`22.2 - RGB & TinyYoloV4 decoding on device` - Runs YoloV4 on RGB frames and displays detections on the frame
- :ref:`23 - Auto Exposure on ROI` - Demonstrates how to use auto exposure based on the selected ROI
- :ref:`24 - OpenCV support` - Demonstrates how to retrieve an image frame as an OpenCV frame
- :ref:`25 - System information` - Displays device system information (memory/cpu usage, temperature)
- :ref:`26.1 - RGB & MobilenetSSD with spatial data` - Displays RGB frames with MobileNet detections and spatial coordinates on them
- :ref:`26.2 - Mono & MobilenetSSD with spatial data` - Displays mono frames with MobileNet detections and spatial coordinates on them
- :ref:`26.3 - RGB & TinyYolo with spatial data`- Displays RGB frames with Yolo detections and spatial coordinates on them
- :ref:`27 - Spatial location calculator` - Demonstrates how to use the spatial location calculator
- :ref:`28 - Camera video high resolution` - Demonstrates how to use the video output of the color camera
- :ref:`29.1 - Object tracker on RGB camera` - Performs object tracking from the color camera
- :ref:`29.2 - Spatial object tracker on RGB camera` - Performs object tracking and also provides spatial coordinates
- :ref:`29.3 - Object tracker on video` - Performs object tracking from the video
- :ref:`30 - Stereo Depth from host` - Generates stereo depth frame from a set of mono images from the host
