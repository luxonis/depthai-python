Spatial object tracker on RGB
=============================

This example shows how to run MobileNetv2SSD on the RGB input frame, and perform spatial object tracking on persons.

setConfidenceThreshold - confidence threshold above which objects are detected

.. rubric:: Similiar samples:

- :ref:`Spatial location calculator`
- :ref:`RGB & MobilenetSSD with spatial data`
- :ref:`Mono & MobilenetSSD with spatial data`
- :ref:`RGB & TinyYolo with spatial data`

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires MobilenetSDD blob (:code:`mobilenet.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_6shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/spatial_object_tracker.py>`__

        .. literalinclude:: ../../../examples/spatial_object_tracker.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/spatial_object_tracker.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/spatial_object_tracker.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
