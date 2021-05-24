RGB Depth
=========

This example shows usage of RGB depth. Since our boards has 1 color and 2 mono cameres, therefore
you need 2 mono cameras for the depth map, then you need to align the image from the
color camera on top of that to get RGB depth.

Demo
####


Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/rgb_depth_aligned.py>`__

        .. literalinclude:: ../../../examples/rgb_depth_aligned.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/rgb_depth_aligned.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/rgb_depth_aligned.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
