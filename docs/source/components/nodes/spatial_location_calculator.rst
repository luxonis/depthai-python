SpatialLocationCalculator
=========================

**SpatialLocationCalculator node** calculates the spatial coordinates of the ROI (region-of-interest) based on the
:code:`depth` map from the :code:`inputDepth`. It will average the depth values in the ROI and remove the ones out of range.

You can also calculate spatial coordiantes on host side, `demo here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-calc-spatials-on-host>`__.
The demo also has the same logic that's performed on the device (:code:`calc.py` file).

.. image:: https://user-images.githubusercontent.com/18037362/146296930-9e7071f5-33b9-45f9-af21-cace7ffffc0f.gif

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spatialCalc = pipeline.create(dai.node.SpatialLocationCalculator)

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
    spatialCalc = pipeline.create(dai.node.SpatialLocationCalculator)
    spatialCalc.setWaitForConfigInput(False)

    # Set initial config
    config = dai.SpatialLocationCalculatorConfigData()
    config.depthThresholds.lowerThreshold = 100
    config.depthThresholds.upperThreshold = 10000

    topLeft = dai.Point2f(0.4, 0.4)
    bottomRight = dai.Point2f(0.6, 0.6)
    config.roi = dai.Rect(topLeft, bottomRight)

    spatial_calc.initialConfig.addROI(config)

    # You can later send configs from the host (XLinkIn) / Script node to the InputConfig


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

.. include::  /includes/spatial-coords.rst

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.SpatialLocationCalculator
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
