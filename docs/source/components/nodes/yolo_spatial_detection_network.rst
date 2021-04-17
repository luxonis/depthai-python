YoloSpatialDetectionNetwork
===========================

Spatial detection for the Yolo NN. It is similar to a combination of the :ref:`YoloDetectionNetwork` and :ref:`SpatialLocationCalculator`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yolo_spatial = pipeline.createYoloSpatialDetectionNetwork()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloSpatial = pipeline.create<dai::node::YoloSpatialDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  Input          │                   │       Passthrough
  ──────────────►│-------------------├─────────────────►
                 │     Yolo          │               Out
                 │     Spatial       ├─────────────────►
                 │     Detection     │BoundingBoxMapping
                 │     Network       ├─────────────────►
  InputDepth     │                   │  PassthroughDepth
  ──────────────►│-------------------├─────────────────►
                 └───────────────────┘

Message types
#############

- :code:`Input` - :ref:`ImgFrame`
- :code:`InputDepth` - :ref:`ImgFrame`
- :code:`Passthrough` - :ref:`ImgFrame`
- :code:`Out` - :ref:`SpatialImgDetections`
- :code:`BoundingBoxMapping` - :ref:`SpatialLocationCalculatorConfig`
- :code:`PassthroughDepth` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yolo_spatial = pipeline.createYoloSpatialDetectionNetwork()
    yolo_spatial.setBlobPath(nnBlobPath)

    # Spatial detection specific parameters
    yolo_spatial.setConfidenceThreshold(0.5)
    yolo_spatial.input.setBlocking(False)
    yolo_spatial.setBoundingBoxScaleFactor(0.5)
    yolo_spatial.setDepthLowerThreshold(100) # Min 10 centimeters
    yolo_spatial.setDepthUpperThreshold(5000) # Max 5 meters

    # Yolo specific parameters
    yolo_spatial.setNumClasses(80)
    yolo_spatial.setCoordinateSize(4)
    yolo_spatial.setAnchors(np.array([10,14, 23,27, 37,58, 81,82, 135,169, 344,319]))
    yolo_spatial.setAnchorMasks({ "side26": np.array([1,2,3]), "side13": np.array([3,4,5]) })
    yolo_spatial.setIouThreshold(0.5)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloSpatial = pipeline.create<dai::node::YoloSpatialDetectionNetwork>();

Examples of functionality
#########################

- :ref:`26.3 - RGB & TinyYolo with spatial data`

Reference
#########

.. autoclass:: depthai.YoloSpatialDetectionNetwork
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst