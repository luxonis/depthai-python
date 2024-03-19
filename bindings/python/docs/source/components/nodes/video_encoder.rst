VideoEncoder
============

VideoEncoder node is used to encode :ref:`ImgFrame` into either H264, H265, or MJPEG streams. Only NV12 or GRAY8 (which gets converted to NV12) format is
supported as an input. All codecs are lossy (except lossless MJPEG), for more information please see `encoding quality docs <https://github.com/luxonis/depthai-experiments/tree/master/gen2-record-replay/encoding_quality>`__.

.. include::  /includes/container-encoding.rst

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    encoder = pipeline.create(dai.node.VideoEncoder)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto encoder = pipeline.create<dai::node::VideoEncoder>();


Inputs and Outputs
##################

.. code-block::

            ┌──────────────┐
            │              │
   input    │              │bitstream
  ─────────►│ VideoEncoder ├────────►
            │              │
            │              │
            └──────────────┘

**Message types**

- :code:`input` - :ref:`ImgFrame` (NV12/GRAY8)
- :code:`bitstream` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

    # Create ColorCamera beforehand
    # Set H265 encoding for the ColorCamera video output
    videoEncoder = pipeline.create(dai.node.VideoEncoder)
    videoEncoder.setDefaultProfilePreset(cam.getFps(), dai.VideoEncoderProperties.Profile.H265_MAIN)

    # Create MJPEG encoding for still images
    stillEncoder = pipeline.create(dai.node.VideoEncoder)
    stillEncoder.setDefaultProfilePreset(1, dai.VideoEncoderProperties.Profile.MJPEG)

    cam.still.link(stillEncoder.input)
    cam.video.link(videoEncoder.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;

    // Create ColorCamera beforehand
    // Set H265 encoding for the ColorCamera video output
    auto videoEncoder = pipeline.create<dai::node::VideoEncoder>();
    videoEncoder->setDefaultProfilePreset(cam->getFps(), dai::VideoEncoderProperties::Profile::H265_MAIN);

    // Create MJPEG encoding for still images
    stillEncoder = pipeline.create(dai.node.VideoEncoder);
    stillEncoder->setDefaultProfilePreset(1, dai::VideoEncoderProperties::Profile::MJPEG);

    cam->still.link(stillEncoder->input);
    cam->video.link(videoEncoder->input);

Limitations
###########

For **H.264 / H.265 encoding**, we have the following limits:
-  **248 million pixels/second** (4K@30) limit for the encoder. The resolution and frame rate can be divided into multiple streams - but the sum of all the pixels/second needs to be below 248 million.
- Due to a HW constraint, video encoding can be done only on frames whose width values are multiples of 32.
- 4096 pixel max width for a frame.

The **MJPEG encoder** is capable of 16384x8192 resolution at 450 MPix/sec. From our testing, we were able to encode 4K at 30FPS and 2x 800P at 55FPS.

Note the processing resources of the encoder **are shared** between H.26x and JPEG.

Examples of functionality
#########################

- :ref:`RGB Encoding`
- :ref:`Encoding Max Limit`
- :ref:`RGB Encoding & MobilenetSSD`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.VideoEncoder
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::VideoEncoder
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
