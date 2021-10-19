VideoEncoder
============

VideoEncoder node is used to encode :ref:`image frames <ImgFrame>` into H264/H265/JPEG.

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

- :code:`input` - :ref:`ImgFrame`
- :code:`bitstream` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

    # Create ColorCamera beforehand
    # Set H265 encoding for the ColorCamera video output
    videoEncoder = pipeline.create(dai.node.VideoEncoder)
    videoEncoder.setDefaultProfilePreset(cam.getVideoSize(), cam.getFps(), dai.VideoEncoderProperties.Profile.H265_MAIN)

    # Create MJPEG encoding for still images
    stillEncoder = pipeline.create(dai.node.VideoEncoder)
    stillEncoder.setDefaultProfilePreset(cam.getStillSize(), 1, dai.VideoEncoderProperties.Profile.MJPEG)

    cam.still.link(stillEncoder.input)
    cam.video.link(videoEncoder.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;

    // Create ColorCamera beforehand
    // Set H265 encoding for the ColorCamera video output
    auto videoEncoder = pipeline.create<dai::node::VideoEncoder>();
    videoEncoder->setDefaultProfilePreset(cam->getVideoSize(), cam->getFps(), dai::VideoEncoderProperties::Profile::H265_MAIN);

    // Create MJPEG encoding for still images
    stillEncoder = pipeline.create(dai.node.VideoEncoder);
    stillEncoder->setDefaultProfilePreset(cam->getStillSize(), 1, dai::VideoEncoderProperties::Profile::MJPEG);

    cam->still.link(stillEncoder->input);
    cam->video.link(videoEncoder->input);

Limitations
###########

- HW limit for the encoder is: 3840x2160 pixels at 30FPS or :code:`248 million pixels/second`. The resolution and frame rate can be divided into multiple streams - but the sum of all the pixels/second needs to be below 248 million.
- Due to a HW constraint, video encoding can be done only on frames whose width values are multiples of 32.

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
