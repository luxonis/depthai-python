ColorCamera
===========

ColorCamera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`InputControl` and :code:`InputConfig`.

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

                 ┌───────────────────┐   Still
                 │                   ├───────────►
  InputConfig    │                   │
  ──────────────►│                   │   Preview
                 │    ColorCamera    ├───────────►
  InputControl   │                   │
  ──────────────►│                   │   Video
                 │                   ├───────────►
                 └───────────────────┘

Message types
#############

- :code:`InputConfig` - :ref:`ImageManipConfig`
- :code:`InputControl` - :ref:`CameraControl`
- :code:`Still` - :ref:`ImgFrame`
- :code:`Preview` - :ref:`ImgFrame`
- :code:`Video` - :ref:`ImgFrame`

:code:`Preview` is RGB (or BGR planar/interleaved if configured) and is mostly suited for small size previews and to feed the image
into :ref:`NeuralNetwork`. :code:`Video` and :code:`Still` are both NV12, so are suitable for bigger sizes. :code:`Still` image gets created when
a capture event is sent to the ColorCamera, so it's like taking a photo.

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      cam = pipeline.createColorCamera()
      cam.setPreviewSize(300, 300)
      cam.setBoardSocket(dai.CameraBoardSocket.RGB)
      cam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
      cam.setInterleaved(False)
      cam.setColorOrder(dai.ColorCameraProperties.ColorOrder.RGB)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto cam = pipeline.create<dai::node::createColorCamera>();

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

.. include::  ../../includes/footer-short.rst
