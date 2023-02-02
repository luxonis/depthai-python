ImageManip
==========

ImageManip node can be used to crop, rotate rectangle area or perform various image transforms: rotate, mirror, flip, perspective transform.

For downscaling, ImageManip uses the bilinear/bicubic interpolation.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    manip = pipeline.create(dai.node.ImageManip)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto manip = pipeline.create<dai::node::ImageManip>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  inputImage     │                   │
  ──────────────►│                   │
                 │                   │        out
                 │    ImageManip     ├───────────►
  inputConfig    │                   │
  ──────────────►│                   │
                 │                   │
                 └───────────────────┘

**Message types**

- :code:`inputImage` - :ref:`ImgFrame`
- :code:`inputConfig` - :ref:`ImageManipConfig`
- :code:`out` - :ref:`ImgFrame`

Usage
#####

..
  TODO Add example of how to perform various transformations with the manip and what needs to be kept in mind with regards to grabbing from
  different streams with their different data formats (color cam, depth)

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      manip = pipeline.create(dai.node.ImageManip)

      manip.initialConfig.setResize(300, 300)
      manip.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto manip = pipeline.create<dai::node::ImageManip>();

      manip->initialConfig.setResize(300, 300);
      manip->initialConfig.setFrameType(dai::ImgFrame::Type::BGR888p);

Image formats supported
#######################

ImageManip node supports the following image formats (more info `in PR here <https://github.com/luxonis/depthai-core/pull/444>`__):

- Input formats supported: RGB/BGR, planar/interleaved, YUV/NV12, RAW8 and GRAY8
- Convert format to any other format mentioned above
- RAW16 (uint16, ``depth`` output)  supported, but without color conversion capability

Note that planar formats are faster to process, so interleaved images should be avoided (eg. NV12 input).

Limitations
###########

Besides limitations mentioned above (unsupported frame formats), there are other limitations:

- Due to HW warp constraint, rotating/warping can be done only on frames whose width values are multiples of 16
- Maximum output width of a frame is 4056 pixels

Examples of functionality
#########################

- :ref:`Mono & MobilenetSSD`
- :ref:`RGB Encoding & Mono & MobilenetSSD`
- :ref:`RGB Camera Control`
- :ref:`ImageManip tiling` - Using ImageManip for frame tiling
- :ref:`ImageManip rotate` - Using ImageManip to rotate color/mono frames

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.ImageManip
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::ImageManip
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
