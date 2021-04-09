ColorCamera node
================



How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    manip = pipeline.createColorCamera()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto imageManip = pipeline.create<dai::node::ColorCamera>();


Inputs and Outputs
##################

.. code-block::

  InitialControl ┌───────────────────┐   Still
  ──────────────►│                   ├───────────►
                 │                   │
  InputConfig    │                   │   Preview
  ──────────────►│    ColorCamera    ├───────────►
                 │                   │
  InputControl   │                   │   Video
  ──────────────►│                   ├───────────►
                 └───────────────────┘

Configuration
#############


-prev_height
-resolution
-interleaved
-fps

Usage
#####

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

Reference
#########

.. autoclass:: depthai.ColorCamera
  :members:
  :inherited-members: