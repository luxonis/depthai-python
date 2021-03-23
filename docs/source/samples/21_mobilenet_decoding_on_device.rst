21 - RGB & MobilenetSSD decoding on device
==========================================

This example shows how to run MobileNetv2SSD on the RGB input frame, and how to display both the RGB
preview and the metadata results from the MobileNetv2SSD on the preview. It's similar to example '08_rgb_mobilenet' except
decoding is done on Myriad instead on the host.


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

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/21_mobilenet_device_side_decoding.py>`__

.. literalinclude:: ../../../examples/21_mobilenet_device_side_decoding.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
