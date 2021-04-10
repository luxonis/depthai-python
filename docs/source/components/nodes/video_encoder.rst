VideoEncoder
============

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

            ┌──────────────┐
            │              │
   Input    │              │Bitstream
  ─────────►│ VideoEncoder ├────────►
            │              │
            │              │
            └──────────────┘

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

- :ref:`04 - RGB Encoding`
- :ref:`13 - Encoding Max Limit`
- :ref:`18 - RGB Encoding with MobilenetSSD`

Reference
#########

.. autoclass:: depthai.VideoEncoder
  :members:
  :inherited-members: