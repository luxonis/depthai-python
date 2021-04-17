MobileNetDetectionNetwork
=========================

MobileNet detection network node is very similar to :ref:`NeuralNetwork` (in fact it extends it). The only difference is that this node
is specifically for the MobileNet NN and it decodes the result of the NN on device. This means that :code:`Out` of this node is not a byte array
but a :ref:`ImgDetections` that can easily be used in your code.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mobilenet_det = pipeline.createMobileNetDetectionNetwork()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetDet = pipeline.create<dai::node::MobileNetDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       Out
              │                   ├───────────►
              │     MobileNet     │
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
    mobilenet_det = pipeline.createMobileNetDetectionNetwork()
    mobilenet_det.setConfidenceThreshold(0.5)
    mobilenet_det.setBlobPath(args.nnPath)
    mobilenet_det.setNumInferenceThreads(2)
    mobilenet_det.input.setBlocking(False)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetDet = pipeline.create<dai::node::MobileNetDetectionNetwork>();

Examples of functionality
#########################

- :ref:`08 - RGB & MobilenetSSD`
- :ref:`09 - Mono & MobilenetSSD`
- :ref:`10 - Mono & MobilenetSSD & Encoding`

Reference
#########

.. autoclass:: depthai.MobileNetDetectionNetwork
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
