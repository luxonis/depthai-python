Camera
======

Camera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`InputControl` and :code:`InputConfig`.
It aims to unify the :ref:`ColorCamera` and :ref:`MonoCamera` into one node.

Compared to :ref:`ColorCamera` node, Camera node:

- Supports **cam.setSize()**, which replaces both ``cam.setResolution()`` and ``cam.setIspScale()``. Camera node will automatically find resolution that fits best, and apply correct scaling to achieve user-selected size
- Supports **cam.setCalibrationAlpha()**, example here: :ref:`Undistort camera stream`
- Supports **cam.loadMeshData()** and **cam.setMeshStep()**, which can be used for custom image warping (undistortion, perspective correction, etc.)
- Automatically undistorts camera stream if HFOV of the camera is greater than 85°. You can disable this with: ``cam.setMeshSource(dai.CameraProperties.WarpMeshSource.NONE)``.

Besides points above, compared to :ref:`MonoCamera` node, Camera node:

- Doesn't have ``out`` output, as it has the same outputs as :ref:`ColorCamera` (``raw``, ``isp``, ``still``, ``preview``, ``video``). This means that ``preview`` will output 3 planes of the same grayscale frame (3x overhead), and ``isp`` / ``video`` / ``still``  will output luma (useful grayscale information) + chroma (all values are 128), which will result in 1.5x bandwidth overhead

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    cam = pipeline.create(dai.node.Camera)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto cam = pipeline.create<dai::node::Camera>();


Inputs and Outputs
##################

.. code-block::

                            Camera node
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
- :code:`still` - :ref:`ImgFrame` - NV12, suitable for bigger size frames. The image gets created when a capture event is sent to the Camera, so it's like taking a photo
- :code:`preview` - :ref:`ImgFrame` - RGB (or BGR planar/interleaved if configured), mostly suited for small size previews and to feed the image into :ref:`NeuralNetwork`
- :code:`video` - :ref:`ImgFrame` - NV12, suitable for bigger size frames

**ISP** (image signal processor) is used for bayer transformation, demosaicing, noise reduction, and other image enhancements.
It interacts with the 3A algorithms: **auto-focus**, **auto-exposure**, and **auto-white-balance**, which are handling image sensor
adjustments such as exposure time, sensitivity (ISO), and lens position (if the camera module has a motorized lens) at runtime.
Click `here <https://en.wikipedia.org/wiki/Image_processor>`__ for more information.

**Image Post-Processing** converts YUV420 planar frames from the **ISP** into :code:`video`/:code:`preview`/:code:`still` frames.

``still`` (when a capture is triggered) and ``isp`` work at the max camera resolution, while ``video`` and ``preview`` are
limited to max 4K (3840 x 2160) resolution, which is cropped from ``isp``.
For IMX378 (12MP), the **post-processing** works like this:

.. code-block::

    ┌─────┐   Cropping to   ┌─────────┐  Downscaling   ┌──────────┐
    │ ISP ├────────────────►│  video  ├───────────────►│ preview  │
    └─────┘  max 3840x2160  └─────────┘  and cropping  └──────────┘

.. image:: /_static/images/tutorials/isp.jpg

The image above is the ``isp`` output from the Camera (12MP resolution from IMX378). If you aren't downscaling ISP,
the ``video`` output is cropped to 4k (max 3840x2160 due to the limitation of the ``video`` output) as represented by
the blue rectangle. The Yellow rectangle represents a cropped ``preview`` output when the preview size is set to a 1:1 aspect
ratio (eg. when using a 300x300 preview size for the MobileNet-SSD NN model) because the ``preview`` output is derived from
the ``video`` output.

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      cam = pipeline.create(dai.node.Camera)
      cam.setPreviewSize(300, 300)
      cam.setBoardSocket(dai.CameraBoardSocket.CAM_A)
      # Instead of setting the resolution, user can specify size, which will set
      # sensor resolution to best fit, and also apply scaling
      cam.setSize(1280, 720)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto cam = pipeline.create<dai::node::Camera>();
      cam->setPreviewSize(300, 300);
      cam->setBoardSocket(dai::CameraBoardSocket::CAM_A);
      // Instead of setting the resolution, user can specify size, which will set
      // sensor resolution to best fit, and also apply scaling
      cam->setSize(1280, 720);

Limitations
###########

Here are known camera limitations for the `RVC2 <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#rvc2>`__:

- **ISP can process about 600 MP/s**, and about **500 MP/s** when the pipeline is also running NNs and video encoder in parallel
- **3A algorithms** can process about **200..250 FPS overall** (for all camera streams). This is a current limitation of our implementation, and we have plans for a workaround to run 3A algorithms on every Xth frame, no ETA yet

Examples of functionality
#########################

- :ref:`Undistort camera stream`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.Camera
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::Camera
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
