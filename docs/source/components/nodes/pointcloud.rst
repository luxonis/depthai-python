PointCloud
==========

The PointCloud node is essential for 3D spatial data processing, transforming depth maps into point clouds. This node, combined with visualization libraries such as Open3D, facilitates advanced 3D modeling and analysis, supporting applications in robotics, AR/VR, and environmental scanning.

How to Place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    pointCloud = pipeline.create(dai.node.PointCloud)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto pointCloud = pipeline.create<dai::node::PointCloud>();


Inputs and Outputs
##################

.. code-block::

                 ┌─────────────────────┐
  inputDepth     │                     │
  ──────────────►│                     │        outputPointCloud
  inputConfig    │     PointCloud      ├────────────────►
  ──────────────►│                     │
                 │                     │        passthroughDepth
                 │                     ├────────────────►
                 └─────────────────────┘

**Message types**

- :code:`inputDepth` - :ref:`ImgFrame`
- :code:`inputConfig` - :ref:`PointCloudConfig`
- :code:`outputPointCloud` - :ref:`PointCloudData`
- :code:`passthroughDepth` - :ref:`ImgFrame` (passthrough input depth map)


Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.PointCloud
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::PointCloud
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
