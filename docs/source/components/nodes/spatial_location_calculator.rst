SpatialLocationCalculator
=========================

SpatialLocationCalculator will calculate the depth based on the :code:`depth` map from the :code:`inputDepth` and ROI (region-of-interest)
provided from the :code:`inputConfig`. It will average the depth values in the ROI and remove the ones out of range.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spatialCalc = pipeline.SpatialLocationCalculator()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spatialCalc = pipeline.create<dai::node::SpatialLocationCalculator>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
                 │                   │
  inputConfig    │                   │       out
  ──────────────►│     Spatial       ├───────────►
                 │     location      │
                 │     calculator    │
  inputDepth     │                   │passthroughDepth
  ──────────────►│-------------------├───────────►
                 └───────────────────┘

**Message types**

- :code:`inputConfig` - :ref:`SpatialLocationCalculatorConfig`
- :code:`inputDepth` - :ref:`ImgFrame`
- :code:`out` - :ref:`SpatialLocationCalculatorData`
- :code:`passthroughDepth` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spatialCalc = pipeline.SpatialLocationCalculator()
    spatialCalc.setWaitForConfigInput(False)

    # Set initial config
    config = dai.SpatialLocationCalculatorConfigData()
    config.depthThresholds.lowerThreshold = 100
    config.depthThresholds.upperThreshold = 10000

    topLeft = dai.Point2f(0.4, 0.4)
    bottomRight = dai.Point2f(0.6, 0.6)
    config.roi = dai.Rect(topLeft, bottomRight)

    spatial_calc.initialConfig.addROI(config)

    # You can later send configs from the host (XLinkIn) / scripting node to the InputConfig


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spatialCalc = pipeline.create<dai::node::SpatialLocationCalculator>();
    spatialCalc->setWaitForConfigInput(false);

    // Set initial config
    dai::SpatialLocationCalculatorConfigData config;
    config.depthThresholds.lowerThreshold = 100;
    config.depthThresholds.upperThreshold = 10000;

    dai::Point2f topLeft(0.4f, 0.4f);
    dai::Point2f bottomRight(0.6f, 0.6f);
    config.roi = dai::Rect(topLeft, bottomRight);

    spatialCalc->initialConfig.addROI(config);

    // You can later send configs from the host (XLinkIn) / scripting node to the InputConfig

Examples of functionality
#########################

- :ref:`Spatial location calculator`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.SpatialLocationCalculator
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::SpatialLocationCalculator
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
