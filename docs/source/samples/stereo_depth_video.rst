Stereo Depth Video
==================

This example is an upgraded :ref:`Depth Preview`. It has higher resolution (720p), each frame can be shown
(mono left-right, rectified left-right, disparity and depth). There are 6 modes which you can select
inside the code:
#. withDepth: if you turn it off it will became :ref:`Mono Preview`, so it will show only the 2 mono cameras
#. outputDepth: if you turn it on it will show the depth
#. lrcheck: computes disparity with sub-pixel interpolation (5 fractional bits), suitable for long range
#. extended: mirror rectified frames: true to have disparity/depth normal (non-mirrored)
#. subpixel: suitable for short range objects

.. rubric:: Similiar samples:

- :ref:`Depth Preview`
- :ref:`Stereo Depth from host`

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/stereo_depth_video.py>`__

        .. literalinclude:: ../../../examples/stereo_depth_video.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/stereo_depth_video.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/stereo_depth_video.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
