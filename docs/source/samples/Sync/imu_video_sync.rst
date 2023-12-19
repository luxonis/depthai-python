IMU and Video Sync
==================

This example demonstrates the use of the DepthAI Sync node to synchronize IMU (Inertial Measurement Unit) data with video frames from a color camera. It highlights the capability to process and display the latest rotation vector from the IMU alongside the video stream in real-time.

.. rubric:: Similar samples:

- :ref:`Depth and Video Sync`
- :ref:`Multiple Scripts Sync`

Demo
####

TODO: Add demo

Setup
#####

.. include::  /includes/install_from_pypi.rst

IMU and Video Synchronization
#############################

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/examples/imu_video_synced.py>`__

        .. literalinclude:: ../../../../depthai-python/examples/imu_video_synced.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/Sync/imu_video_synced.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/Sync/imu_video_synced.cpp
           :language: cpp
           :linenos:

How it Works
############

#. Initialize the DepthAI device.
#. Check the connected IMU type and firmware version.
#. Create a pipeline and add a ColorCamera and IMU node.
#. Set up the Sync node to synchronize the IMU data with the video frames.
#. Link the output of the ColorCamera and IMU nodes to the Sync node.
#. Start the pipeline and continuously receive synchronized data.
#. Display the video frames and print the IMU rotation vector data, including quaternion values.

.. include::  /includes/footer-short.rst
