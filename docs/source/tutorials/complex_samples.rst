Complex
=======

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:

   ../samples/rgb_camera_control.rst
   ../samples/mono_camera_control.rst
   ../samples/depth_crop_control.rst
   ../samples/stereo_depth_from_host.rst
   ../samples/stereo_depth_video.rst
   ../samples/rgb_rotate_warp.rst
   ../samples/rgb_depth_aligned.rst
   ../samples/autoexposure_roi.rst
   ../samples/mono_depth_mobilenetssd.rst
   ../samples/rgb_encoding_mobilenet.rst
   ../samples/rgb_encoding_mono_mobilenet.rst
   ../samples/rgb_encoding_mono_mobilenet_depth.rst
   ../samples/tiny_yolo_v3_decoding_on_device.rst
   ../samples/tiny_yolo_v4_decoding_on_device.rst
   ../samples/object_tracker.rst
   ../samples/object_tracker_video.rst
   ../samples/spatial_location_calculator.rst
   ../samples/spatial_object_tracker.rst
   ../samples/spatial_mobilenet.rst
   ../samples/spatial_mobilenet_mono.rst
   ../samples/spatial_tiny_yolo.rst

These samples are great starting point for the gen2 API.
If you want to see more interesting examples you should check out our `Experiments on GitHub <https://github.com/luxonis/depthai-experiments>`_.

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
