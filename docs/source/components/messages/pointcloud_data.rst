PointCloudData
==============

:ref:`PointCloudData` encapsulates 3D spatial information, representing a collection of points in a 3D space. Each point within the point cloud has its own position (X, Y, Z coordinates).
PointCloudData is used to represent the output of :ref:`PointCloud` nodes, and can be used to perform a variety of spatial analysis and reconstruction tasks.

Setter methods are only used to provide metadata to the :ref:`PointCloudData` message (will be used for recording and replaying pointclouds).

Reference
#########

The detailed API for `PointCloudData` offers control over the generation, manipulation, and retrieval of 3D point cloud data.

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.PointCloudData
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::PointCloudData
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst