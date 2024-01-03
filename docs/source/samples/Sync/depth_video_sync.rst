Depth and Video Sync
====================

This example demonstrates the use of the DepthAI Sync node to synchronize output from StereoDepth and Color Camera nodes. It showcases how to process and display disparity maps from stereo cameras and video frames from a color camera in real time.


.. rubric:: Similar samples:

- :ref:`IMU and Video Sync`
- :ref:`Multiple Scripts Sync`


Demo
####

.. image:: ../../../../docs/source/_static/images/examples/depth_video_synced.gif
    :alt: Depth and Video Sync Demo
    :width: 100%
    :align: center


Setup
#####

.. include::  /includes/install_from_pypi.rst


Source code
###########

.. tabs::
    
    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/depth_video_synced.py>`__

        .. literalinclude:: ../../../../depthai-core/examples/Sync/depth_video_synced.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/Sync/depth_video_synced.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/Sync/depth_video_synced.cpp
           :language: cpp
           :linenos:

How it Works
############

#. Initialize MonoCamera nodes for left and right cameras.
#. Set up a ColorCamera node.
#. Create a StereoDepth node for depth perception.
#. Configure the Sync node to synchronize disparity from the StereoDepth node and video frames from the ColorCamera node.
#. Display the synchronized frames using OpenCV. Frames are synchronized to threshold of 50 milliseconds.


.. include::  /includes/footer-short.rst
