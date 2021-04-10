MonoCamera
==========


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
  InitialControl │                   │
  ──────────────►│                   │
                 │                   │       Out
                 │    MonoCamera     ├───────────►
  InputControl   │                   │
  ──────────────►│                   │
                 │                   │
                 └───────────────────┘

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

- :ref:`02 - Mono Preview`
- :ref:`09 - Mono & MobilenetSSD`
- :ref:`19 - Mono Camera Control`

Reference
#########

.. autoclass:: depthai.MonoCamera
  :members:
  :inherited-members: