ImageManip
==========

ImageManip node can be used to crop, rotate rectangle area or perform various image transforms: rotate, mirror, flip, perspective transform.

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

ImageManip node supports the following image formats:

- for RGB input/output types only :code:`RGB888p` / :code:`BGR888p` (planar, not interleaved)
- :code:`RAW16` input, but can only for cropping (use-case: depth map), outputting same format
- can output :code:`GRAY8` (either as single plane, or 3-plane RGB as set with setFrameType()), but only from these inputs: :code:`RAW8`, :code:`GRAY8`, :code:`YUV420p` (:ref:`ColorCamera` isp), :code:`NV12` (:ref:`ColorCamera` video / still)

Limitations
###########

Besides limitations mentioned above (unsupported frame formats), there are other limitations:

- Due to HW warp constraint, rotating/warping can be done only on frames whose width values are multiples of 16. 

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
