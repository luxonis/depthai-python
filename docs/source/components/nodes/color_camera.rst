Color Camera node
=================



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

Preview is RGB (or BGR planar/interleaved if configured) and is mostly suited for small size previews and to feed the image
into :ref:`Neural Network node`. `Video` and `Still` are both NV12, so are suitable for bigger sizes. `Still` gets created when
a capture event is sent to the ColorCamera, so it's like taking a photo.

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
      manip = pipeline.createColorCamera()

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto imageManip = pipeline.create<dai::node::createColorCamera>();

Examples of functionality
#########################

- :ref:`01 - RGB Preview`
- :ref:`14 - Color Camera Control`
- :ref:`28 - Camera video high resolution`

Reference
#########

.. autoclass:: depthai.ColorCamera
  :members:
  :inherited-members: