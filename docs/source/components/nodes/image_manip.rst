ImageManip
==========

ImageManip node can be used to crop, rotate rectangle area or perform various image transforms: rotate, mirror, flip, perspective transform.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    manip = pipeline.createImageManip()

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
      manip = pipeline.createImageManip()

      manip.initialConfig.setResize(300, 300)
      manip.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto manip = pipeline.create<dai::node::ImageManip>();

      manip->initialConfig.setResize(300, 300);
      manip->initialConfig.setFrameType(dai::ImgFrame::Type::BGR888p);

Examples of functionality
#########################

- :ref:`Mono & MobilenetSSD`
- :ref:`RGB Encoding & Mono & MobilenetSSD`
- :ref:`RGB Camera Control`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.ImageManip
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
