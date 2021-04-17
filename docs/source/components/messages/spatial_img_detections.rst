SpatialImgDetections
====================

Similar to :ref:`ImageDetections`, but this message includes **XYZ** coordinates of the detected objects as well.

Both :ref:`YoloSpatialDetectionNetwork` and :ref:`MobileNetSpatialDetectionNetwork` output this message, since they both derive
from the :code:`SpatialDetectionNetwork` class.

Examples of functionality
#########################

- :ref:`26.1 - RGB & MobilenetSSD with spatial data`
- :ref:`26.2 - MONO & MobilenetSSD with spatial data`
- :ref:`26.3 - RGB & TinyYolo with spatial data`

Reference
#########

.. autoclass:: depthai.SpatialImgDetections
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
