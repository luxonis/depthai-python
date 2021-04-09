XLinkOut node
=============

ImageManip node can be used to crop, rotate rectangle area or perform various image transforms: rotate, mirror, flip, perspective transform.

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
  Input     │              │   (to host)
  ─────────►│   XLinkOut   ├------------►
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

- :ref:`09 - Mono & MobilenetSSD`
- :ref:`10 - Mono & MobilenetSSD & Encoding`
- :ref:`11 - RGB & Encoding & Mono & MobilenetSSD`
- :ref:`12 - RGB Encoding & Mono with MobilenetSSD & Depth`
- :ref:`14 - Color Camera Control`
- :ref:`26.2 - MONO & MobilenetSSD with spatial data`

Reference
#########

.. autoclass:: depthai.XLinkOut
  :members:
  :inherited-members: