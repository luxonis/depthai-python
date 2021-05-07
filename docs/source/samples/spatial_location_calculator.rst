Spatial location calculator
===========================

This example shows how to retrieve spatial location data (X,Y,Z) on a runtime configurable ROI. You can move the ROI using WASD keys.
X,Y,Z coordinates are relative to the center of depth map.


setConfidenceThreshold - confidence threshold above which objects are detected

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/spatial_location_calculator.py>`__

        .. literalinclude:: ../../../examples/spatial_location_calculator.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/spatial_location_calculator.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/spatial_location_calculator.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
