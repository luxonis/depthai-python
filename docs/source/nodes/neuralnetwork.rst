NeuralNetwork node
===============

Runs a neural inference on input data.

Passthrough mechanism
*********************

The passthrough mechanism is very useful when a node specifies its input to be non-blocking, where messages can be overwritten.
There we don't know on which message the node performed its operation (eg NN, was inference done on frame 25 or skipped 25 and performed
inference on 26). At the same time means that if: xlink and host input queues are blocking, and we receive both say passthrough and
output we can do a blocking get on both of those queues and be sure to always get matching frames. They might not arrive at the same time,
but both of them will arrive, and be in queue in correct spot to be taken out together.


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

  InputImage  +-------------------+
  +---------->+                   |
              |                   |        out
              |  ImageManip node  +---------->
  InputConfig |                   |
  +---------->+                   |
              +-------------------+

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

- :ref:`09 - Mono & MobilenetSSD`
- :ref:`10 - Mono & MobilenetSSD & Encoding`
- :ref:`11 - RGB & Encoding & Mono & MobilenetSSD`
- :ref:`12 - RGB Encoding & Mono with MobilenetSSD & Depth`
- :ref:`14 - Color Camera Control`
- :ref:`26.2 - MONO & MobilenetSSD with spatial data`

Reference
#########

.. autoclass:: depthai.ImageManip
  :members:
  :inherited-members: