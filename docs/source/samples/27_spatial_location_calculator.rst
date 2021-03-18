27 - Spatial location calculator
================================

This example shows how to retrieve spatial location data (X,Y,Z) on a runtime configurable ROI.
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

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/gen2_develop/examples/27_spatial_location_calculator.py>`__

.. literalinclude:: ../../../examples/27_spatial_location_calculator.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
