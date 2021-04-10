StereoDepth
===========

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
                 │                   │RectifiedLeft
                 │                   ├─────────────►
  Left           │                   │   SyncedLeft
  ──────────────►│                   ├─────────────►
                 │                   │        Depth
                 │                   ├─────────────►
                 │    StereoDepth    │    Disparity
                 │                   ├─────────────►
  Right          │                   │RectifiedRight
  ──────────────►│                   ├─────────────►
                 │                   │   SyncedRight
                 │                   ├─────────────►
                 └───────────────────┘

Configuration
#############
-median
-raw_depth
-rectified
-lr_check
-ext_disparity
-subpixel


Disparity
#########

When calculating the disparity, each pixel in the disparity map gets assigned a confidence value 0..255 by the stereo matching algorithm, as:
- 0 - maximum confidence that it holds a valid value
- 255 - minimum confidence, so there are chances the value is incorrect
(this confidence score is kind-of inverted, if say comparing with NN)

For the final disparity map, a filtering is applied based on the confidence threshold value: the pixels that have their confidence score larger than
the threshold get invalidated, i.e. their disparity value is set to zero.

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

- :ref:`03 - Depth Preview`
- :ref:`10 - Mono & MobilenetSSD & Encoding`
- :ref:`26.1 - RGB & MobilenetSSD with spatial data`

Reference
#########

.. autoclass:: depthai.StereoDepth
  :members:
  :inherited-members: