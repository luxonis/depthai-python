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
              │           ├────────►
  inputConfig |           | amplitude
  ───────────►│           ├────────►
              │    ToF    │    error
     input    |           ├────────►
  ───────────►│           │ intensity
              │           ├────────►
              └───────────┘



**Message types**

- ``inputConfig`` - :ref:`ToFConfig`
- ``input`` - :ref:`ImgFrame`
- ``depth`` - :ref:`ImgFrame` - Decoded depth map
- ``amplitude`` - :ref:`ImgFrame`
- ``intensity`` - :ref:`ImgFrame`
- ``phase`` - :ref:`ImgFrame` Phase image, useful for debugging (FP32)

ToF Settings
############

In :ref:`ToF depth` example we allow users to quickly configure ToF settings. These are mostly for debugging, and should be enabled:

- FPPN Correction; It's a process that corrects the fixed pattern noise (FPN) of the ToF sensor. It's enabled by default for best performance.
- Wiggle Correction: It's a process that corrects the wiggle effect of the ToF sensor. It's enabled by default for best performance.
- Temperature Correction: It's a process that corrects the temperature effect of the ToF sensor. It's enabled by default for best performance.

And these settings are up to the user:

* Optical Correction: It's a process that corrects the optical effect. When enabled, the ToF returns depth map (represented by Green Line on graph below) instead of distance, so it matches :ref:`StereoDepth` depth reporting. It does rectification and distance to depth conversion (Z-map).
* Phase Unwrapping - Process that corrects the phase wrapping effect of the ToF sensor. You can set it to [0..5 are optimized]. The higher the number, the longer the ToF range, but it also increases the noise.

  * `0` - Disabled, up to ~1.5 meters
  * `1` - Up to ~3 meters
  * `2` - Up to ~4.5 meters
  * `3` - Up to ~6 meters
  * `4` - Up to ~7.5 meters

* Burst mode: When enabled, ToF node won't reuse frames, as shown on the graph below. It's related to post-processing of the ToF frames, not the actual sensor/projector. It's disabled by default.
* Phase shuffle Temporal filter: Averages shuffled and non-shuffled frames of the same modulation frequency to reduce noise. It's enabled by default. You can disable it to reduce :ref:`ToF motion blur` and system load.

.. image:: /_static/images/components/tof-optical-correction.png

.. image:: /_static/images/components/tof-diagram.png

Phase unwrapping
################

If the time it takes for the light to travel from ToF sensor and back exceeds the period of the emitted wave (1.5m or 1.8m), the resulting measurement will "wrap" back to a lower value. This is called phase wrapping.
It's similar to how a clock resets after 12 hours. Phase unwrapping is possible as our ToF has two different modulation frequencies (80Mhz and 100MHz).

Phase unwrapping aims to correct this by allowing the sensor to interpret longer distances without confusion. It uses algorithms to keep track of how many cycles (round trips of the wave) have occurred,
thus correcting the "wrapped" phases. The downside is that the more cycles the sensor has to keep track of, the more noise it introduces into the measurement.

ToF motion blur
###############

To reduce motion blur, we recommend these settings:

- Increase camera FPS. It goes up to 160 FPS, which causes frame capture to be the fastest (6.25ms between frames). This will reduce motion blur as ToF combines multiple frames to get the depth. Note that 160FPS will increase system load significantly (see :ref:`Debugging <Debugging DepthAI pipeline>`). Note also that higher FPS -> lower exposure times, which can increase noise.
- Disable phase shuffle temporal filter. This will introduce more noise.
- Disable phase unwrapping. This will reduce max distance to 1.5 meters, so about 1 cubic meter of space will be visible.
- Enable burst mode. This is irrelevant if shuffle filter and phase unwrapping are disabled (see diagram above). When enabled, ToF node won't reuse frames (lower FPS).

Max distance
############

Maximum ToF distance depends on the phase unwrapping level and modulation frequency. The formula for calculating the maximum distance is:

.. math::
  :nowrap:

  \begin{align*}
  c & = 299792458.0 \quad \text{//! speed of light in m/s} \\
  MAX\_80MHZ\_MM & = \frac{c}{80000000 \times 2} \times 1000 \quad \text{//! convert speed of light to mm/160ns} \\
  MAX\_MM\_80MHZ & = \text{round}(MAX\_80MHZ\_MM) \quad \text{// round(1873.7) -> 1874} \\
  MAX\_DIST\_80MHZ & = (\text{phaseUnwrappingLevel} + 1) \times 1.5 \times 1000 + \frac{\text{phaseUnwrapErrorThreshold}}{2} \quad \text{//! corrected formula in mm for 80 MHz} \\
  MAX\_100MHZ\_MM & = \frac{c}{100000000 \times 2} \times 1000 \quad \text{//! convert speed of light to mm/200ns} \\
  MAX\_MM\_100MHZ & = \text{round}(MAX\_100MHZ\_MM) \quad \text{// round(1498.9636) -> 1499} \\
  MAX\_DIST\_100MHZ & = (\text{phaseUnwrappingLevel} + 1) \times 1.5 \times 1000 + \frac{\text{phaseUnwrapErrorThreshold}}{2} \quad \text{//! corrected formula in mm for 100 MHz} \\
  \end{align*}

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

    tof_cam = pipeline.create(dai.node.Camera)
    # Decoded depth FPS will be /2 due to shuffle/non-shuffle averaging
    tof_cam.setFps(60)
    # We assume the ToF camera sensor is on port CAM_A
    tof_cam.setBoardSocket(dai.CameraBoardSocket.CAM_A)

    tof = pipeline.create(dai.node.ToF)

    # Higher number => faster processing. 1 shave core can do 30FPS.
    tof.setNumShaves(1)

    # Median filter, kernel size 5x5
    tof.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_5x5)

    tofConfig = tof.initialConfig.get()
    # Temporal filter averages shuffle/non-shuffle frequencies
    tofConfig.enablePhaseShuffleTemporalFilter = True
    # Phase unwrapping, for longer range.
    tofConfig.phaseUnwrappingLevel = 4 # Up to 7.5 meters
    tofConfig.phaseUnwrapErrorThreshold = 300
    tof.initialConfig.set(tofConfig)

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
