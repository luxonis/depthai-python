Object tracker on video
=======================

This example shows how to run MobileNetv2SSD on video input frame, and perform object tracking on persons.

.. rubric:: Similiar samples:

- :ref:`Object tracker on RGB`
- :ref:`Spatial object tracker on RGB`

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires MobilenetSDD based person-detection blob (:code:`person-detection-0201_openvino_2021.3_7shave.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/person-detection-0201_openvino_2021.3_7shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/object_tracker_video.py>`__

        .. literalinclude:: ../../../examples/object_tracker_video.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/object_tracker_video.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/object_tracker_video.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
