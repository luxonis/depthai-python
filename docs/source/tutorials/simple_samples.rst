Simple
======

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:

   ../samples/rgb_preview.rst
   ../samples/rgb_video.rst
   ../samples/mono_preview.rst
   ../samples/depth_preview.rst
   ../samples/rgb_encoding.rst
   ../samples/rgb_mono_encoding.rst
   ../samples/encoding_max_limit.rst
   ../samples/rgb_full_resolution_saver.rst
   ../samples/mono_full_resolution_saver.rst
   ../samples/rgb_mobilenet.rst
   ../samples/rgb_mobilenet_4k.rst
   ../samples/mono_mobilenet.rst
   ../samples/video_mobilenet.rst
   ../samples/imu_accelerometer_gyroscope.rst
   ../samples/imu_rotation_vector.rst
   ../samples/edge_detector.rst

These samples are great starting point for the gen2 API.

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
- :ref:`Edge detector` - Edge detection on input frame
