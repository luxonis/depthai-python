IMU and Video Sync
==================

This example demonstrates the use of the DepthAI Sync node to synchronize IMU (Inertial Measurement Unit) data with video frames from a color camera. It highlights the capability to process and display the latest rotation vector from the IMU alongside the video stream in real-time.

.. rubric:: Similar samples:

- :ref:`Depth and Video Sync`
- :ref:`Multiple Scripts Sync`

Demo
####

.. code-block:: 

    ~/depthai-python/examples/Sync $ python3 imu_video_synced.py
    IMU type: BNO086, firmware version: 3.9.7

    Device timestamp imu: 0:00:05.379914
    Device timestamp video:0:00:05.385096
    Quaternion: i: -0.0549 j: -0.0335 k: 0.0018 real: 0.9979


    Device timestamp imu: 0:00:05.410274
    Device timestamp video:0:00:05.418425
    Quaternion: i: -0.0549 j: -0.0334 k: 0.0018 real: 0.9979


    Device timestamp imu: 0:00:05.445439
    Device timestamp video:0:00:05.451753
    Quaternion: i: -0.0548 j: -0.0334 k: 0.0018 real: 0.9979


    Device timestamp imu: 0:00:05.475084
    Device timestamp video:0:00:05.485082
    Quaternion: i: -0.0547 j: -0.0334 k: 0.0018 real: 0.9979


    Device timestamp imu: 0:00:05.510046
    Device timestamp video:0:00:05.518411
    Quaternion: i: -0.0546 j: -0.0334 k: 0.0018 real: 0.9979


Setup
#####

.. include::  /includes/install_from_pypi.rst


Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/examples/imu_video_synced.py>`__

        .. literalinclude:: ../../../../examples/Sync/imu_video_synced.py
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
