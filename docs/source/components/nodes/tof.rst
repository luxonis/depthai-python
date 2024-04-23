ToF
===

**ToF node** is used for converting the raw data from the ToF sensor into a depth map. Currently, these 2 products contain a ToF sensor:

- `OAK-D SR PoE <https://docs.luxonis.com/projects/hardware/en/latest/pages/OAK-D-SR-POE/>`__ - integrated 33D ToF sensor, together with a stereo camera pair
- `OAK-FFC ToF 33D <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM0256/>`__ - standalone FFC module with a 33D ToF sensor

ToF's ``depth`` output can be used instead of :ref:`StereoDepth`'s - so you can link ToF.depth to :ref:`MobileNetSpatialDetectionNetwork`/:ref:`YoloSpatialDetectionNetwork` or
:ref:`SpatialLocationCalculator` directly.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    warp = pipeline.create(dai.node.ToF)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto warp = pipeline.create<dai::node::ToF>();

Inputs and Outputs
##################

.. code-block::

              ┌───────────┐    depth
  inputConfig |           ├────────►
  ───────────►│           | amplitude
     input    |    ToF    ├────────►
  ───────────►│           │    error
              │           ├────────►
              └───────────┘

**Message types**

- ``inputConfig`` - :ref:`ToFConfig`
- ``input`` - :ref:`ImgFrame`
- ``depth`` - :ref:`ImgFrame`
- ``amplitude`` - :ref:`ImgFrame`
- ``error`` - :ref:`ImgFrame`

ToF Settings
############

In :ref:`ToF depth` example we allow users to quickly configure the following ToF settings:

- FFPN Correction; It's a process that corrects the fixed pattern noise (FPN) of the ToF sensor. It's enabled by default for best performance.
- Wiggle Correction: It's a process that corrects the wiggle effect of the ToF sensor. It's enabled by default for best performance.
- Temperature Correction: It's a process that corrects the temperature effect of the ToF sensor. It's enabled by default for best performance.
- Optical Correction: It's a process that corrects the optical effect (On -> ToF returns distance represented by Green Line), so it matches :ref:`StereoDepth` depth reporting.
- Phase Unwrapping - Process that corrects the phase wrapping effect of the ToF sensor. You can set it to [0..4]. The higher the number, the longer the ToF range, but it also increases the noise.
    - `0` - Disabled.
    - `1` - Up to 1.5 meters
    - `2` - Up to 3 meters
    - `3` - Up to 4.5 meters
    - `4` - Up to 6 meters

.. image:: /_static/images/components/tof-optical-correction.png

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

    tof_cam = pipeline.create(dai.node.Camera)
    # We assume the ToF camera sensor is on port CAM_A
    tof_cam.setBoardSocket(dai.CameraBoardSocket.CAM_A)

    tof = pipeline.create(dai.node.ToF)
    # ToF node converts raw sensor frames into depth
    tof_cam.raw.link(tof.input)

    # Send ToF depth output to the host, or perhaps to SLC / Spatial Detection Network
    tof.depth.link(xout.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;

    auto tofCam = pipeline.create<dai::node::MonoCamera>();
    // We assume the ToF camera sensor is on port CAM_A
    tofCam->setBoardSocket(dai::CameraBoardSocket::AUTO);

    auto tof = pipeline.create<dai::node::ToF>();
    // ToF node converts raw sensor frames into depth
    tofCam->raw.link(tof->input);

    auto xout = pipeline.create<dai::node::XLinkOut>();
    xout->setStreamName("depth");
    // Send ToF depth output to the host
    tof->depth.link(xout->input);

Examples of functionality
#########################

- :ref:`ToF depth`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.ToF
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::ToF
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
