ImgDetections
=============

Both :ref:`YoloDetectionNetwork` and :ref:`MobileNetDetectionNetwork` output this message. This message contains a list of :code:`detections`,
which contains :code:`label`, :code:`confidence`, and the bounding box information (:code:`xmin`, :code:`ymin`, :code:`xmax`, :code:`ymax`).

Examples of functionality
#########################

- :ref:`08 - RGB & MobilenetSSD`
- :ref:`09 - Mono & MobilenetSSD`
- :ref:`22.2 - RGB & TinyYoloV4 decoding on device`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.ImgDetections
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::ImgDetections
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
