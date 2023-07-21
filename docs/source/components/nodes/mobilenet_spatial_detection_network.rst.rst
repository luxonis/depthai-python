MobileNetSpatialDetectionNetwork
================================

Spatial detection for the MobileNet NN. It is similar to a combination of the :ref:`MobileNetDetectionNetwork` and :ref:`SpatialLocationCalculator`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mobilenetSpatial = pipeline.create(dai.node.MobileNetSpatialDetectionNetwork)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetSpatial = pipeline.create<dai::node::MobileNetSpatialDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  input          │                   │       passthrough
  ──────────────►│-------------------├─────────────────►
                 │     MobileNet     │               out
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
    mobilenetSpatial = pipeline.create(dai.node.MobileNetSpatialDetectionNetwork)

    mobilenetSpatial.setBlobPath(nnBlobPath)
    # Will ingore all detections whose confidence is below 50%
    mobilenetSpatial.setConfidenceThreshold(0.5)
    mobilenetSpatial.input.setBlocking(False)
    # How big the ROI will be (smaller value can provide a more stable reading)
    mobilenetSpatial.setBoundingBoxScaleFactor(0.5)
    # Min/Max threshold. Values out of range will be set to 0 (invalid)
    mobilenetSpatial.setDepthLowerThreshold(100)
    mobilenetSpatial.setDepthUpperThreshold(5000)

    # Link depth from the StereoDepth node
    stereo.depth.link(mobilenetSpatial.inputDepth)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetSpatial = pipeline.create<dai::node::MobileNetSpatialDetectionNetwork>();

    mobilenetSpatial->setBlobPath(nnBlobPath);
    // Will ingore all detections whose confidence is below 50%
    mobilenetSpatial->setConfidenceThreshold(0.5f);
    mobilenetSpatial->input.setBlocking(false);
    // How big the ROI will be (smaller value can provide a more stable reading)
    mobilenetSpatial->setBoundingBoxScaleFactor(0.5f);
    // Min/Max threshold. Values out of range will be set to 0 (invalid)
    mobilenetSpatial->setDepthLowerThreshold(100);
    mobilenetSpatial->setDepthUpperThreshold(5000);

    // Link depth from the StereoDepth node
    stereo->depth.link(mobilenetSpatial->inputDepth);

Examples of functionality
#########################

- :ref:`RGB & MobilenetSSD with spatial data`
- :ref:`Mono & MobilenetSSD with spatial data`

.. include::  /includes/spatial-coords.rst

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.MobileNetSpatialDetectionNetwork
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::MobileNetSpatialDetectionNetwork
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
