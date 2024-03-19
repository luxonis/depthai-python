SpatialImgDetections
====================

Similar to :ref:`ImgDetections`, but this message includes **XYZ** coordinates of the detected objects as well.

Both :ref:`YoloSpatialDetectionNetwork` and :ref:`MobileNetSpatialDetectionNetwork` output this message.

Examples of functionality
#########################

- :ref:`RGB & MobilenetSSD with spatial data`
- :ref:`Mono & MobilenetSSD with spatial data`
- :ref:`RGB & TinyYolo with spatial data`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.SpatialImgDetections
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::SpatialImgDetections
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
