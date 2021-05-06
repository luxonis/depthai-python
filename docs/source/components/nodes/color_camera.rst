ColorCamera
===========

ColorCamera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`InputControl` and :code:`InputConfig`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    cam = pipeline.createColorCamera()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto cam = pipeline.create<dai::node::ColorCamera>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐   still
                 │                   ├───────────►
  inputConfig    │                   │
  ──────────────►│                   │   preview
                 │    ColorCamera    ├───────────►
  inputControl   │                   │
  ──────────────►│                   │   video
                 │                   ├───────────►
                 └───────────────────┘

**Message types**

- :code:`inputConfig` - :ref:`ImageManipConfig`
- :code:`inputControl` - :ref:`CameraControl`
- :code:`still` - :ref:`ImgFrame`
- :code:`preview` - :ref:`ImgFrame`
- :code:`video` - :ref:`ImgFrame`

:code:`Preview` is RGB (or BGR planar/interleaved if configured) and is mostly suited for small size previews and to feed the image
into :ref:`NeuralNetwork`. :code:`video` and :code:`still` are both NV12, so are suitable for bigger sizes. :code:`still` image gets created when
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
      auto cam = pipeline.create<dai::node::ColorCamera>();
      cam->setPreviewSize(300, 300);
      cam->setBoardSocket(dai::CameraBoardSocket::RGB);
      cam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
      cam->setInterleaved(false);
      cam->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);

Examples of functionality
#########################

- :ref:`01 - RGB Preview`
- :ref:`14.1 - Color Camera Control`
- :ref:`28 - Camera video high resolution`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.ColorCamera
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::ColorCamera
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
