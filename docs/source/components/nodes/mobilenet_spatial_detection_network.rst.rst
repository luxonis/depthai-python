MobileNetSpatialDetectionNetwork
================================

Spatial detection for the MobileNet NN. It is similar to a combination of the :ref:`MobileNetDetectionNetwork` and :ref:`SpatialLocationCalculator`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mobilenet_spatial = pipeline.createMobileNetSpatialDetectionNetwork()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetSpatial = pipeline.create<dai::node::MobileNetSpatialDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  Input          │                   │       Passthrough
  ──────────────►│-------------------├─────────────────►
                 │     MobileNet     │               Out
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
    mobilenet_spatial = pipeline.createMobileNetSpatialDetectionNetwork()

    mobilenet_spatial.setBlobPath(nnBlobPath)
    # Will ingore all detections whose confidence is below 50%
    mobilenet_spatial.setConfidenceThreshold(0.5)
    mobilenet_spatial.input.setBlocking(False)
    # How big the ROI will be (smaller value can provide a more stable reading)
    mobilenet_spatial.setBoundingBoxScaleFactor(0.5)
    # Min/Max threshold. Values out of range will be set to 0 (invalid)
    mobilenet_spatial.setDepthLowerThreshold(100)
    mobilenet_spatial.setDepthUpperThreshold(5000)

    # Link depth from the StereoDepth node
    stereo.depth.link(mobilenet_spatial.inputDepth)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetSpatial = pipeline.create<dai::node::MobileNetSpatialDetectionNetwork>();

Examples of functionality
#########################

- :ref:`26.1 - RGB & MobilenetSSD with spatial data`
- :ref:`26.2 - MONO & MobilenetSSD with spatial data`

Reference
#########

.. autoclass:: depthai.MobileNetSpatialDetectionNetwork
  :members:
  :inherited-members: