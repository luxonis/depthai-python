Edge detector
=============

This example performs edge detection on 3 different inputs: left, right and RGB camera.
HW accelerated sobel filter 3x3 is used.
Sobel filter parameters can be changed by keys 1 and 2.

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/edge_detector.py>`__

        .. literalinclude:: ../../../examples/edge_detector.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/edge_detector.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/edge_detector.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
