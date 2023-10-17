YoloDetectionNetwork
====================

Yolo detection network extends :ref:`NeuralNetwork` node by also adding **YOLO NN result decoding**, which happens on the OAK device.
This means that :code:`Out` of this node is not a :ref:`NNData` (a byte array) but a :ref:`ImgDetections` that can easily be used in your code.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yoloDet = pipeline.create(dai.node.YoloDetectionNetwork)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloDet = pipeline.create<dai::node::YoloDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       out
              │                   ├───────────►
              │     Yolo          │
              │     Detection     │
  input       │     Network       │ passthrough
  ───────────►│-------------------├───────────►
              │                   │
              └───────────────────┘

**Message types**

- :code:`input` - :ref:`ImgFrame`
- :code:`out` - :ref:`ImgDetections`
- :code:`passthrough` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    yoloDet = pipeline.create(dai.node.YoloDetectionNetwork)
    yoloDet.setBlobPath(nnBlobPath)

    # Yolo specific parameters
    yoloDet.setConfidenceThreshold(0.5)
    yoloDet.setNumClasses(80)
    yoloDet.setCoordinateSize(4)
    yoloDet.setAnchors([10,14, 23,27, 37,58, 81,82, 135,169, 344,319])
    yoloDet.setAnchorMasks({"side26": [1, 2, 3], "side13": [3, 4, 5]})
    yoloDet.setIouThreshold(0.5)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto yoloDet = pipeline.create<dai::node::YoloDetectionNetwork>();
    yoloDet->setBlobPath(nnBlobPath);

    // yolo specific parameters
    yoloDet->setConfidenceThreshold(0.5f);
    yoloDet->setNumClasses(80);
    yoloDet->setCoordinateSize(4);
    yoloDet->setAnchors({10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319});
    yoloDet->setAnchorMasks({{"side13", {3, 4, 5}}, {"side26", {1, 2, 3}}});
    yoloDet->setIouThreshold(0.5f);

Examples of functionality
#########################

- :ref:`RGB & Tiny YOLO`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.YoloDetectionNetwork
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::YoloDetectionNetwork
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
