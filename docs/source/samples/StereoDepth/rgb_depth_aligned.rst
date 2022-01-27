RGB Depth alignment
===================

This example shows usage of RGB depth alignment. Since OAK-D has a color and a pair of stereo cameras,
you can align depth map to the color frame on top of that to get RGB depth.

Demo
####

.. image:: https://user-images.githubusercontent.com/18037362/151351377-a5752fbe-3b8b-4985-b8d1-d5f8a7d5a868.png

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/StereoDepth/rgb_depth_aligned.py>`__

        .. literalinclude:: ../../../../examples/StereoDepth/rgb_depth_aligned.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/StereoDepth/rgb_depth_aligned.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/StereoDepth/rgb_depth_aligned.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
