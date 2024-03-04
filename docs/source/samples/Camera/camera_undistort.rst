Undistort camera stream
=======================

This example shows how you can use :ref:`Camera` node to undistort wide FOV camera stream. :ref:`Camera` node will automatically undistort ``still``, ``video`` and ``preview`` streams, while ``isp`` stream will be left as is.

Demo
####

.. figure:: https://github.com/luxonis/depthai-python/assets/18037362/936b9ad7-179b-42a5-a6cb-25efdbdf73d9

    Left: Camera.isp output. Right: Camera.video (undistorted) output

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/Camera/camera_undistort.py>`__

        .. literalinclude:: ../../../../examples/Camera/camera_undistort.py
           :language: python
           :linenos:

    .. tab:: C++

        Work in progress.


..
    Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/Camera/camera_undistort.cpp>`__

    .. literalinclude:: ../../../../depthai-core/examples/Camera/camera_undistort.cpp
        :language: cpp
        :linenos:

.. include::  /includes/footer-short.rst
