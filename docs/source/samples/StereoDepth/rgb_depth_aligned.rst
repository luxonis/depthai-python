RGB Depth
=========

This example shows usage of RGB depth. Since our boards has 1 color and 2 mono cameres, therefore
you need 2 mono cameras for the depth map, then you need to align the image from the
color camera on top of that to get RGB depth.

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/UB-R3mGBVW0" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>


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
