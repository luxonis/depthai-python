YoloDetectionNetwork
====================

Yolo detection network node is very similar to :ref:`Neural Network` (in fact it extends it). The only difference is that this node
is specifically for the (tiny) Yolo (v3/v4) NN and it decodes the result of the NN on device. This means that :code:`Out` of this node is not a
:ref:`NNData` (a byte array) but a :ref:`ImgDetections` that can easily be used in your code.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yolo_det = pipeline.createYoloDetectionNetwork()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloDet = pipeline.create<dai::node::YoloDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       Out
              │                   ├───────────►
              │     Yolo          │
              │     Detection     │
  Input       │     Network       │ Passthrough
  ───────────►│-------------------├───────────►
              │                   │
              └───────────────────┘

Message types
#############

- :code:`Input` - :ref:`ImgFrame`
- :code:`Out` - :ref:`ImgDetections`
- :code:`Passthrough` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yolo_det = pipeline.createYoloDetectionNetwork()

    # Yolo specific parameters
    yolo_det.setConfidenceThreshold(0.5)
    yolo_det.setNumClasses(80)
    yolo_det.setCoordinateSize(4)
    yolo_det.setAnchors(np.array([10,14, 23,27, 37,58, 81,82, 135,169, 344,319]))
    yolo_det.setAnchorMasks({"side26": np.array([1, 2, 3]), "side13": np.array([3, 4, 5])})
    yolo_det.setIouThreshold(0.5)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloDet = pipeline.create<dai::node::YoloDetectionNetwork>();

Examples of functionality
#########################

- :ref:`22.1 - RGB & TinyYoloV3 decoding on device`
- :ref:`22.2 - RGB & TinyYoloV4 decoding on device`

Reference
#########

.. autoclass:: depthai.YoloDetectionNetwork
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
