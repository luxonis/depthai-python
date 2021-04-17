SpatialLocationCalculator
=========================

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spatial_calc = pipeline.SpatialLocationCalculator()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spatialCalc = pipeline.create<dai::node::SpatialLocationCalculator>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
                 │                   │
  InputConfig    │                   │       Out
  ──────────────►│     Spatial       ├───────────►
                 │     location      │
                 │     calculator    │
  InputDepth     │                   │PassthroughDepth
  ──────────────►│-------------------├───────────►
                 └───────────────────┘

Message types
#############

- :code:`InputConfig` - :ref:`SpatialLocationCalculatorConfig`
- :code:`InputDepth` - :ref:`ImgFrame`
- :code:`Out` - :ref:`SpatialLocationCalculatorData`
- :code:`PassthroughDepth` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spatial_calc = pipeline.SpatialLocationCalculator()
    spatial_calc.setWaitForConfigInput(False)

    # Set initial config
    config = dai.SpatialLocationCalculatorConfigData()
    config.depthThresholds.lowerThreshold = 100
    config.depthThresholds.upperThreshold = 10000
    config.roi = dai.Rect(topLeft, bottomRight)
    spatial_calc.initialConfig.addROI(config)

    # You can later send configs from the host (XLinkIn) /scripting node to the InputConfig


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spatialCalc = pipeline.create<dai::node::SpatialLocationCalculator>();

Examples of functionality
#########################

- :ref:`27 - Spatial location calculator`

Reference
#########

.. autoclass:: depthai.SpatialLocationCalculator
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
