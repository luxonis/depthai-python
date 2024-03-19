EdgeDetector
============

Edge detector uses `Sobel filter <https://en.wikipedia.org/wiki/Sobel_operator>`__ to create an image that emphasises edges.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    edgeDetector = pipeline.create(dai.node.EdgeDetector)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto edgeDetector = pipeline.create<dai::node::EdgeDetector>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  inputImage     │                   │
  ──────────────►│                   │
                 │                   │ outputImage
                 │   EdgeDetector    ├───────────►
  inputConfig    │                   │
  ──────────────►│                   │
                 │                   │
                 └───────────────────┘

**Message types**

- :code:`inputImage` - :ref:`ImgFrame`
- :code:`inputConfig` - :ref:`EdgeDetectorConfig`
- :code:`outputImage` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      edgeDetector = pipeline.create(dai.node.EdgeDetector)

      sobelHorizontalKernel = [[1, 0, -1], [2, 0, -2], [1, 0, -1]]
      sobelVerticalKernel = [[1, 2, 1], [0, 0, 0], [-1, -2, -1]]
      edgeDetector.initialConfig.setSobelFilterKernels(sobelHorizontalKernel, sobelVerticalKernel)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto edgeDetector = pipeline.create<dai::node::EdgeDetector>();

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
