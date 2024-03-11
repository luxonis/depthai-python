PointCloud Visualization
========================

This example demonstrates how to visualize an on-device created point cloud using DepthAI and Open3D. It captures color and depth data from an OAK device, combines them to create a colored point cloud, and displays it in real-time. The FPS counter is included to monitor performance.

Key Concepts
############

- Capturing synchronized color and depth frames using the DepthAI API.
- Creating a point cloud with color data for visualization.
- Real-time visualization of the point cloud using Open3D.
- Implementing an FPS counter to monitor the application's performance.


Demo
####

.. code-block:: bash

    python3 visualize_pointcloud.py

Setup
#####

If you are using Python, ensure you have DepthAI and Open3D installed in your Python environment:

.. code-block:: bash

    python3 -m pip install depthai open3d


If Open3D is not already installed, the script will prompt you to install it.

Source code
###########

The example initializes the DepthAI pipeline with color and mono cameras and a stereo depth node to generate depth information. It then creates a point cloud node and configures it for sparse point cloud generation. The script visualizes this point cloud using Open3D, with each point's color corresponding to the color image captured by the RGB camera.

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/PointCloud/visualize_pointcloud.py>`__

        .. literalinclude:: ../../../../examples/PointCloud/visualize_pointcloud.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/PointCloud/visualize_pointcloud.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/PointCloud/visualize_pointcloud.cpp
           :language: cpp
           :linenos:
the full scene and observe how the point cloud updates in real-time to reflect the environment.

.. include::  /includes/footer-short.rst