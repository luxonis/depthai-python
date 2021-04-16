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
    auto imageManip = pipeline.create<dai::node::ImageManip>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  InputImage     │                   │
  ──────────────►│                   │
                 │                   │        Out
                 │    ImageManip     ├───────────►
  InputConfig    │                   │
  ──────────────►│                   │
                 │                   │
                 └───────────────────┘

Message types
#############

- :code:`InputImage` - :ref:`ImgFrame`
- :code:`InputConfig` - :ref:`ImageManipConfig`
- :code:`Out` - :ref:`ImgFrame`

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
      manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto imageManip = pipeline.create<dai::node::ImageManip>();

      imageManip->initialConfig.setCenterCrop(0.7f);
      imageManip->initialConfig.setResizeThumbnail(300, 400);

Examples of functionality
#########################

- :ref:`09 - Mono & MobilenetSSD`
- :ref:`11 - RGB & Encoding & Mono & MobilenetSSD`
- :ref:`14 - Color Camera Control`

Reference
#########

.. autoclass:: depthai.ImageManip
  :members:
  :inherited-members: