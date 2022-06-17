AprilTag
========

The **AprilTag node** will find all AprilTag markers on the frame. AprilTag marker is a visual `fiducial marker <https://en.wikipedia.org/wiki/Fiducial_marker>`__,
so it's similar to ArUco, ARTag and ARToolKit markers.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    aprilTag = pipeline.create(dai.node.AprilTag)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto aprilTag = pipeline.create<dai::node::AprilTag>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  inputImage     │                   │passthroughInputImage
  ──────────────►│-------------------│───────────►
                 │                   │         out
                 │     AprilTag      ├───────────►
  inputConfig    │                   │   outConfig
  ──────────────►│-------------------│───────────►
                 │                   │
                 └───────────────────┘

**Message types**

- :code:`inputImage` - :ref:`ImgFrame`
- :code:`inputConfig` - :ref:`AprilTagConfig`
- :code:`passthroughInputImage` - :ref:`ImgFrame`
- :code:`out` - :ref:`AprilTags`
- :code:`outConfig` - :ref:`AprilTagConfig`

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      aprilTag = pipeline.create(dai.node.AprilTag)

      sobelHorizontalKernel = [[1, 0, -1], [2, 0, -2], [1, 0, -1]]
      sobelVerticalKernel = [[1, 2, 1], [0, 0, 0], [-1, -2, -1]]
      edgeDetector.initialConfig.setSobelFilterKernels(sobelHorizontalKernel, sobelVerticalKernel)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto aprilTag = pipeline.create<dai::node::AprilTag>();

      std::vector<std::vector<int>> sobelHorizontalKernel = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
      std::vector<std::vector<int>> sobelVerticalKernel = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
      edgeDetector->setSobelFilterKernels(sobelHorizontalKernel, sobelVerticalKernel);

Examples of functionality
#########################

- :ref:`Edge detector`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.EdgeDetector
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::EdgeDetector
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
