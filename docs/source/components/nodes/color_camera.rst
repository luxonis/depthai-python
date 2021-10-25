ColorCamera
===========

ColorCamera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`InputControl` and :code:`InputConfig`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    cam = pipeline.create(dai.node.ColorCamera)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto cam = pipeline.create<dai::node::ColorCamera>();


Inputs and Outputs
##################

.. code-block::

                         ColorCamera node
                 ┌──────────────────────────────┐
                 │   ┌─────────────┐            │
                 │   │    Image    │ raw        │     raw
                 │   │    Sensor   │---┬--------├────────►
                 │   └────▲────────┘   |        │
                 │        │   ┌--------┘        │
                 │      ┌─┴───▼─┐               │     isp
  inputControl   │      │       │-------┬-------├────────►
  ──────────────►│------│  ISP  │ ┌─────▼────┐  │   video
                 │      │       │ |          |--├────────►
                 │      └───────┘ │   Image  │  │   still
  inputConfig    │                │   Post-  │--├────────►
  ──────────────►│----------------|Processing│  │ preview
                 │                │          │--├────────►
                 │                └──────────┘  │
                 └──────────────────────────────┘

**Message types**

- :code:`inputConfig` - :ref:`ImageManipConfig`
- :code:`inputControl` - :ref:`CameraControl`
- :code:`raw` - :ref:`ImgFrame` - RAW10 bayer data. Demo code for unpacking `here <https://github.com/luxonis/depthai-experiments/blob/3f1b2b2/gen2-color-isp-raw/main.py#L13-L32>`__
- :code:`isp` - :ref:`ImgFrame` - YUV420 planar (same as YU12/IYUV/I420)
- :code:`still` - :ref:`ImgFrame` - NV12, suitable for bigger size frames. The image gets created when a capture event is sent to the ColorCamera, so it's like taking a photo
- :code:`preview` - :ref:`ImgFrame` - RGB (or BGR planar/interleaved if configured), mostly suited for small size previews and to feed the image into :ref:`NeuralNetwork`
- :code:`video` - :ref:`ImgFrame` - NV12, suitable for bigger size frames

**ISP** (image signal processor) is used for bayer transformation, demosaicing, noise reduction, and other image enhancements.
It interacts with the 3A algorithms: **auto-focus**, **auto-exposure**, and **auto-white-balance**, which are handling image sensor
adjustments such as exposure time, sensitivity (ISO), and lens position (if the camera module has a motorized lens) at runtime.
Click `here <https://en.wikipedia.org/wiki/Image_processor>`__ for more information.

**Image Post-Processing** converts YUV420 planar frames from the **ISP** into :code:`video`/:code:`preview`/:code:`still` frames.

When setting sensor resolution to 12MP and using :code:`video`, you will get 4K video output. 4K frames are cropped from 12MP frames (not downsampled).

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      cam = pipeline.create(dai.node.ColorCamera)
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

- :ref:`RGB Preview`
- :ref:`RGB Camera Control`
- :ref:`RGB video`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.ColorCamera
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
