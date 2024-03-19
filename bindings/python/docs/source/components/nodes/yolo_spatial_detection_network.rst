YoloSpatialDetectionNetwork
===========================

Spatial detection for the Yolo NN. It is similar to a combination of the :ref:`YoloDetectionNetwork` and :ref:`SpatialLocationCalculator`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yoloSpatial = pipeline.create(dai.node.YoloSpatialDetectionNetwork)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloSpatial = pipeline.create<dai::node::YoloSpatialDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  input          │                   │       passthrough
  ──────────────►│-------------------├─────────────────►
                 │     Yolo          │               out
                 │     Spatial       ├─────────────────►
                 │     Detection     │boundingBoxMapping
                 │     Network       ├─────────────────►
  inputDepth     │                   │  passthroughDepth
  ──────────────►│-------------------├─────────────────►
                 └───────────────────┘

**Message types**

- :code:`input` - :ref:`ImgFrame`
- :code:`inputDepth` - :ref:`ImgFrame`
- :code:`passthrough` - :ref:`ImgFrame`
- :code:`out` - :ref:`SpatialImgDetections`
- :code:`boundingBoxMapping` - :ref:`SpatialLocationCalculatorConfig`
- :code:`passthroughDepth` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yoloSpatial = pipeline.create(dai.node.YoloSpatialDetectionNetwork)
    yoloSpatial.setBlobPath(nnBlobPath)

    # Spatial detection specific parameters
    yoloSpatial.setConfidenceThreshold(0.5)
    yoloSpatial.input.setBlocking(False)
    yoloSpatial.setBoundingBoxScaleFactor(0.5)
    yoloSpatial.setDepthLowerThreshold(100) # Min 10 centimeters
    yoloSpatial.setDepthUpperThreshold(5000) # Max 5 meters

    # Yolo specific parameters
    yoloSpatial.setNumClasses(80)
    yoloSpatial.setCoordinateSize(4)
    yoloSpatial.setAnchors([10,14, 23,27, 37,58, 81,82, 135,169, 344,319])
    yoloSpatial.setAnchorMasks({ "side26": [1,2,3], "side13": [3,4,5] })
    yoloSpatial.setIouThreshold(0.5)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloSpatial = pipeline.create<dai::node::YoloSpatialDetectionNetwork>();
    yoloSpatial->setBlobPath(nnBlobPath);

    // Spatial detection specific parameters
    yoloSpatial->setConfidenceThreshold(0.5f);
    yoloSpatial->input.setBlocking(false);
    yoloSpatial->setBoundingBoxScaleFactor(0.5);
    yoloSpatial->setDepthLowerThreshold(100); // Min 10 centimeters
    yoloSpatial->setDepthUpperThreshold(5000); // Max 5 meters

    // yolo specific parameters
    yoloSpatial->setNumClasses(80);
    yoloSpatial->setCoordinateSize(4);
    yoloSpatial->setAnchors({10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319});
    yoloSpatial->setAnchorMasks({{"side13", {3, 4, 5}}, {"side26", {1, 2, 3}}});
    yoloSpatial->setIouThreshold(0.5f);

Examples of functionality
#########################

- :ref:`RGB & TinyYolo with spatial data`

.. include::  /includes/spatial-coords.rst

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.YoloSpatialDetectionNetwork
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::YoloSpatialDetectionNetwork
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst