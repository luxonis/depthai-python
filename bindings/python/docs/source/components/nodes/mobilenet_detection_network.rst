MobileNetDetectionNetwork
=========================

MobileNet detection network node is very similar to :ref:`NeuralNetwork` (in fact it extends it). The only difference is that this node
is specifically for the MobileNet NN and it decodes the result of the NN on device. This means that :code:`out` of this node is not a byte array
but a :ref:`ImgDetections` that can easily be used in your code.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mobilenetDet = pipeline.create(dai.node.MobileNetDetectionNetwork)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetDet = pipeline.create<dai::node::MobileNetDetectionNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       out
              │                   ├───────────►
              │     MobileNet     │
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
    mobilenetDet = pipeline.create(dai.node.MobileNetDetectionNetwork)

    mobilenetDet.setConfidenceThreshold(0.5)
    mobilenetDet.setBlobPath(nnBlobPath)
    mobilenetDet.setNumInferenceThreads(2)
    mobilenetDet.input.setBlocking(False)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mobilenetDet = pipeline.create<dai::node::MobileNetDetectionNetwork>();

    mobilenetDet->setConfidenceThreshold(0.5f);
    mobilenetDet->setBlobPath(nnBlobPath);
    mobilenetDet->setNumInferenceThreads(2);
    mobilenetDet->input.setBlocking(false);

Examples of functionality
#########################

- :ref:`RGB & MobilenetSSD`
- :ref:`Mono & MobilenetSSD`
- :ref:`Mono & MobilenetSSD & Depth`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.MobileNetDetectionNetwork
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::MobileNetDetectionNetwork
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
