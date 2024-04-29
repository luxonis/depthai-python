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

**Image Post-Processing** converts YUV420 planar frames from the **ISP** into ``video``/ ``preview``/ ``still`` frames.

``still`` (when a capture is triggered) and ``isp`` work at the max camera resolution, while ``video`` and ``preview`` are
limited to max 4K (3840 x 2160) resolution, which is cropped from ``isp``.
For IMX378 (12MP), the **post-processing** works like this:

.. code-block::

    ┌─────┐   Cropping to   ┌─────────┐  Downscaling   ┌──────────┐
    │ ISP ├────────────────►│  video  ├───────────────►│ preview  │
    └─────┘  max 3840x2160  └─────────┘  and cropping  └──────────┘

If resolution was set to 12MP, and we were to use ``video``, we'd get a 4K frame (3840x2160) cropped from the center of the 12MP frame.

Full FOV
########

Some sensors (let's take IXM378 for an example) will, by default, have 1080P resolution set, which is a crop from the full sensor resolution.
You can print sensor features to see how FOV is affected by the selected sensor resolution:

.. code-block:: python

    import depthai as dai

    with dai.Device() as device:
        for cam in dev.getConnectedCameraFeatures():
            print(cam)
            #continue  # uncomment for less verbosity
            for cfg in cam.configs:
                print("   ", cfg)

    ```
    Running on OAK-D-S2 will print:

    {socket: CAM_A, sensorName: IMX378, width: 4056, height: 3040, orientation: AUTO, supportedTypes: [COLOR], hasAutofocus: 1, hasAutofocusIC: 1, name: color}
        {width: 1920, height: 1080, minFps: 2.03, maxFps: 60, type: COLOR, fov: {x:108, y: 440, width: 3840, height: 2160}}
        {width: 3840, height: 2160, minFps: 1.42, maxFps: 42, type: COLOR, fov: {x:108, y: 440, width: 3840, height: 2160}}
        {width: 4056, height: 3040, minFps: 1.42, maxFps: 30, type: COLOR, fov: {x:0, y: 0, width: 4056, height: 3040}}
        {width: 1352, height: 1012, minFps: 1.25, maxFps: 52, type: COLOR, fov: {x:0, y: 0, width: 4056, height: 3036}}
        {width: 2024, height: 1520, minFps: 2.03, maxFps: 85, type: COLOR, fov: {x:4, y: 0, width: 4048, height: 3040}}
    {socket: CAM_B, sensorName: OV9282, width: 1280, height: 800, orientation: AUTO, supportedTypes: [MONO], hasAutofocus: 0, hasAutofocusIC: 0, name: left}
        {width: 1280, height: 720, minFps: 1.687, maxFps: 143.1, type: MONO, fov: {x:0, y: 40, width: 1280, height: 720}}
        {width: 1280, height: 800, minFps: 1.687, maxFps: 129.6, type: MONO, fov: {x:0, y: 0, width: 1280, height: 800}}
        {width: 640, height: 400, minFps: 1.687, maxFps: 255.7, type: MONO, fov: {x:0, y: 0, width: 1280, height: 800}}
    {socket: CAM_C, sensorName: OV9282, width: 1280, height: 800, orientation: AUTO, supportedTypes: [MONO], hasAutofocus: 0, hasAutofocusIC: 0, name: right}
        {width: 1280, height: 720, minFps: 1.687, maxFps: 143.1, type: MONO, fov: {x:0, y: 40, width: 1280, height: 720}}
        {width: 1280, height: 800, minFps: 1.687, maxFps: 129.6, type: MONO, fov: {x:0, y: 0, width: 1280, height: 800}}
        {width: 640, height: 400, minFps: 1.687, maxFps: 255.7, type: MONO, fov: {x:0, y: 0, width: 1280, height: 800}}
    ```

So for IMX378, if we select 4K or 1080P resolution, FOV will be cropped ~5% horizontally and ~29% vertically. So we can either select ``THE_12_MP``,
``THE_1352X1012``, or ``THE_2024X1520`` resolution to get the full sensor FOV.

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      cam = pipeline.create(dai.node.ColorCamera)
      cam.setPreviewSize(300, 300)
      cam.setBoardSocket(dai.CameraBoardSocket.CAM_A)
      cam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
      cam.setInterleaved(False)
      cam.setColorOrder(dai.ColorCameraProperties.ColorOrder.RGB)

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto cam = pipeline.create<dai::node::ColorCamera>();
      cam->setPreviewSize(300, 300);
      cam->setBoardSocket(dai::CameraBoardSocket::CAM_A);
      cam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
      cam->setInterleaved(false);
      cam->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);

Limitations
###########

Here are known camera limitations for the `RVC2 <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#rvc2>`__:

- **ISP can process about 600 MP/s**, and about **500 MP/s** when the pipeline is also running NNs and video encoder in parallel
- **3A algorithms** can process about **200..250 FPS overall** (for all camera streams). This is a current limitation of our implementation, and we have plans for a workaround to run 3A algorithms on every Xth frame, no ETA yet
- **ISP Scaling** numerator value can be 1..16 and denominator value 1..32 for both vertical and horizontal scaling. So you can downscale eg. 12MP (4056x3040) only to resolutions `calculated here <https://docs.google.com/spreadsheets/d/153yTstShkJqsPbkPOQjsVRmM8ZO3A6sCqm7uayGF-EE/edit#gid=0>`__

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
