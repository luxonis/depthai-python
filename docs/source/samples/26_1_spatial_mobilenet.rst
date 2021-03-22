26.1 - RGB & MobilenetSSD with spatial data
===========================================

This example shows how to run MobileNetv2SSD on the RGB input frame, and how to display both the RGB
preview, detections, depth map and spatial information (X,Y,Z). It's similar to example 
'21_mobilenet_decoding_on_device' except it has spatial data.
X,Y,Z coordinates are relative to the center of depth map.

setConfidenceThreshold - confidence threshold above which objects are detected

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst


This example also requires MobilenetSDD blob (:code:`mobilenet.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_6shave.blob>`__

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/gen2_develop/examples/26_1_spatial_mobilenet.py>`__

.. literalinclude:: ../../../examples/26_1_spatial_mobilenet.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
