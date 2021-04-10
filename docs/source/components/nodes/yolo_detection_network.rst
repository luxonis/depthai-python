Yolo Detection Network node
===========================

Yolo detection network node is very similar to :ref:`Neural Network node` (in fact it extends it). The only difference is that this node
is specifically for the (tiny) Yolo (v3/v4) NN and it decodes the result of the NN on device. This means that :code:`Out` of this node is not a
:ref:`NNData message` (a byte array) but a :ref:`ImgDetections message` that can easily be used in your code.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    manip = pipeline.createImageManip()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto imageManip = pipeline.create<dai::node::ImageManip>();


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

Usage
#####

An example for the various transformations one can do with the manip and what needs to be kept in mind with regards to grabbing from
different streams with their different data formats (color cam, depth) would be great!

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      manip = pipeline.createImageManip()

      manip.initialConfig.setResize(300, 300)
      manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto imageManip = pipeline.create<dai::node::ImageManip>();

      imageManip->initialConfig.setCenterCrop(0.7f);
      imageManip->initialConfig.setResizeThumbnail(300, 400);

Examples of functionality
#########################

- :ref:`22.1 - RGB & TinyYoloV3 decoding on device`
- :ref:`22.2 - RGB & TinyYoloV4 decoding on device`

Reference
#########

.. autoclass:: depthai.YoloDetectionNetwork
  :members:
  :inherited-members: