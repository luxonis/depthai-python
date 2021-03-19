26.1 - RGB & TinyYolo with spatial data
===========================================

This example shows how to run TinyYoloV3 and v4 on the RGB input frame, and how to display both the RGB
preview, detections, depth map and spatial information (X,Y,Z). It's similar to example 
'26_1_spatial_mobilenet' except it is running TinyYolo network.
X,Y,Z coordinates are relative to the center of depth map.


setNumClasses - number of YOLO classes
setCoordinateSize - size of coordinate
setAnchors - yolo anchors
setAnchorMasks - anchorMasks26, anchorMasks13 (anchorMasks52 - additionally for full YOLOv4)
setIouThreshold - intersection over union threshold
setConfidenceThreshold - confidence threshold above which objects are detected

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst


This example also requires YOLOv4-tiny blob (:code:`tiny-yolo-v4_openvino_2021.2_6shave.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v4_openvino_2021.2_6shave.blob>`__

YOLOv3-tiny blob (:code:`tiny-yolo-v3_openvino_2021.2_6shave.blob` file) can be used too - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v3_openvino_2021.2_6shave.blob>`__

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/gen2_develop/examples/26_3_spatial_tiny_yolo.py>`__

.. literalinclude:: ../../../examples/26_3_spatial_tiny_yolo.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
