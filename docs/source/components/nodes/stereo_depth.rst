StereoDepth
===========

Stereo depth node calculates the dispartiy/depth from two :ref:`mono cameras <MonoCamera>`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    stereo = pipeline.createStereoDepth()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();


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

Message types
#############

- :code:`Left` - :ref:`ImgFrame` from the left :ref:`MonoCamera`
- :code:`Right` - :ref:`ImgFrame` from the right :ref:`MonoCamera`
- :code:`RectifiedLeft` - :ref:`ImgFrame`
- :code:`SyncedLeft` - :ref:`ImgFrame`
- :code:`Depth` - :ref:`ImgFrame`
- :code:`Disparity` - :ref:`ImgFrame`
- :code:`RectifiedRight` - :ref:`ImgFrame`
- :code:`SyncedRight` - :ref:`ImgFrame`

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

..
  COnfigure different:
  -median
  -raw_depth
  -rectified
  -lr_check
  -ext_disparity
  -subpixel

.. tabs::

  .. code-tab:: py
    '''
    If one or more of the additional depth modes (lrcheck, extended, subpixel)
    are enabled, then:
    - depth output is FP16. TODO enable U16.
    - median filtering is disabled on device. TODO enable.
    - with subpixel, either depth or disparity has valid data.
    Otherwise, depth output is U16 (mm) and median is functional.
    But like on Gen1, either depth or disparity has valid data. TODO enable both.
    '''

    pipeline = dai.Pipeline()
    stereo = pipeline.createStereoDepth()

    # Better handling for occlusions:
    stereo.setLeftRightCheck(False)
    # Closer-in minimum depth, disparity range is doubled:
    stereo.setExtendedDisparity(False)
    # Better accuracy for longer distance, fractional disparity 32-levels:
    stereo.setSubpixel(False)

    # Define and configure MonoCamera nodes
    left_mono.out.link(depth.left)
    right_mono.out.link(depth.right)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();

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