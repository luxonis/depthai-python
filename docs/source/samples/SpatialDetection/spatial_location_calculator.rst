Spatial location calculator
===========================

This example shows how to retrieve spatial location data (X,Y,Z) on a runtime configurable ROI. You can move the ROI using WASD keys.
X,Y,Z coordinates are relative to the center of depth map.

setConfidenceThreshold - confidence threshold above which objects are detected

.. rubric:: Similiar samples:

- :ref:`Spatial object tracker on RGB`
- :ref:`RGB & MobilenetSSD with spatial data`
- :ref:`Mono & MobilenetSSD with spatial data`
- :ref:`RGB & TinyYolo with spatial data`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/mbZViuS4WEQ" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/SpatialDetection/spatial_location_calculator.py>`__

        .. literalinclude:: ../../../../examples/SpatialDetection/spatial_location_calculator.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/SpatialDetection/spatial_location_calculator.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/SpatialDetection/spatial_location_calculator.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
