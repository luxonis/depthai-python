PointCloud Control
==================

This example demonstrates how to use :ref:`PointCloudConfig` message to dynamically update the transformation matrix of a point cloud and visualize the transformed point cloud using Open3D.

The transformation matrix is updated to make it look like the point cloud is rotating about the Z-axis. This is achieved by first moving the pointcloud along Y axis:

.. code-block:: python

    # Move the point cloud along Y axis
    translate_y_matrix = np.array([[1, 0, 0, 0],
                                  [0, 1, 0, 500],
                                  [0, 0, 1, 0],
                                  [0, 0, 0, 1]])


Then, the point cloud is rotated about Z axis:

.. code-block:: python

    # Rotate the point cloud about Z axis
    rotate_z_matrix = np.array([[np.cos(angle), -np.sin(angle), 0, 0],
                               [np.sin(angle), np.cos(angle), 0, 0],
                               [0, 0, 1, 0],
                               [0, 0, 0, 1]])


Demo
####

.. image:: /_static/images/examples/pointcloud_control.gif
  :alt: PointCloud control

Setup
#####

Ensure DepthAI and Open3D are installed in your Python environment:

.. code-block:: bash

    python3 -m pip install depthai open3d


Source code
###########

The example initializes the DepthAI pipeline with color and mono cameras and a stereo depth node to generate depth information. It then creates a point cloud node, dynamically updates its transformation matrix based on a rotation value, and visualizes this transformed point cloud using Open3D. Each point's color corresponds to the color image captured by the RGB camera.

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/PointCloud/pointcloud_control.py>`__

        .. literalinclude:: ../../../../examples/PointCloud/pointcloud_control.py
           :language: python
           :linenos:


.. include::  /includes/footer-short.rst