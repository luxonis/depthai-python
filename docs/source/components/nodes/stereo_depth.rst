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
                 │                   │rectifiedLeft
                 │                   ├─────────────►
  left           │                   │   syncedLeft
  ──────────────►│                   ├─────────────►
                 │                   │        depth
                 │                   ├─────────────►
                 │    StereoDepth    │    disparity
                 │                   ├─────────────►
  right          │                   │rectifiedRight
  ──────────────►│                   ├─────────────►
                 │                   │   syncedRight
                 │                   ├─────────────►
                 └───────────────────┘

**Message types**

- :code:`left` - :ref:`ImgFrame` from the left :ref:`MonoCamera`
- :code:`right` - :ref:`ImgFrame` from the right :ref:`MonoCamera`
- :code:`rectifiedLeft` - :ref:`ImgFrame`
- :code:`syncedLeft` - :ref:`ImgFrame`
- :code:`depth` - :ref:`ImgFrame`
- :code:`disparity` - :ref:`ImgFrame`
- :code:`rectifiedRight` - :ref:`ImgFrame`
- :code:`syncedRight` - :ref:`ImgFrame`

Disparity
#########

When calculating the disparity, each pixel in the disparity map gets assigned a confidence value 0..255 by the stereo matching algorithm, as:
- 0 - maximum confidence that it holds a valid value
- 255 - minimum confidence, so there are chances the value is incorrect
(this confidence score is kind-of inverted, if say comparing with NN)

For the final disparity map, a filtering is applied based on the confidence threshold value: the pixels that have their confidence score larger than
the threshold get invalidated, i.e. their disparity value is set to zero.

Current limitations
###################

If one or more of the additional depth modes (lrcheck, extended, subpixel) are enabled, then:

- :code:`depth` output is FP16.
- median filtering is disabled on device.
- with subpixel, either depth or disparity has valid data.

Otherwise, depth output is U16 (in milimeters) and median is functional.

Like on Gen1, either :code:`depth` or :code:`disparity` has valid data.

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

    pipeline = dai.Pipeline()
    stereo = pipeline.createStereoDepth()

    # Better handling for occlusions:
    stereo.setLeftRightCheck(False)
    # Closer-in minimum depth, disparity range is doubled:
    stereo.setExtendedDisparity(False)
    # Better accuracy for longer distance, fractional disparity 32-levels:
    stereo.setSubpixel(False)

    # Define and configure MonoCamera nodes beforehand
    left.out.link(stereo.left)
    right.out.link(stereo.right)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();

    // Better handling for occlusions:
    stereo->setLeftRightCheck(false);
    // Closer-in minimum depth, disparity range is doubled:
    stereo->setExtendedDisparity(false);
    // Better accuracy for longer distance, fractional disparity 32-levels:
    stereo->setSubpixel(false);

    // Define and configure MonoCamera nodes beforehand
    left->out.link(stereo->left);
    right->out.link(stereo->right);

Examples of functionality
#########################

- :ref:`03 - Depth Preview`
- :ref:`10 - Mono & MobilenetSSD & Depth`
- :ref:`26.1 - RGB & MobilenetSSD with spatial data`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.StereoDepth
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::StereoDepth
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
