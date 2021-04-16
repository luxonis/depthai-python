VideoEncoder
============

VideoEncoder node is used to encode :ref:`image frames <ImfFrame>` into H264/H265/JPEG.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    encoder = pipeline.createVideoEncoder()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto encoder = pipeline.create<dai::node::VideoEncoder>();


Inputs and Outputs
##################

.. code-block::

            ┌──────────────┐
            │              │
   Input    │              │Bitstream
  ─────────►│ VideoEncoder ├────────►
            │              │
            │              │
            └──────────────┘

Message types
#############

- :code:`Input` - :ref:`ImgFrame`
- :code:`Bitstream` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    encoder_video = pipeline.createVideoEncoder()
    # Set H265 encoding for 4k, 30FPS video
    encoder_video.setDefaultProfilePreset(3840, 2160, 30, dai.VideoEncoderProperties.Profile.H265_MAIN)

    # Or create MJPEG encoding for still images
    encoder_still = pipeline.createVideoEncoder()
    encoder_still.setDefaultProfilePreset(cam.getStillSize(), 1, dai.VideoEncoderProperties.Profile.MJPEG)

    # Create ColorCamera beforehand
    cam.still.link(encoder_still.input)
    cam.video.link(encoder_video.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto encoder = pipeline.create<dai::node::VideoEncoder>();

Examples of functionality
#########################

- :ref:`04 - RGB Encoding`
- :ref:`13 - Encoding Max Limit`
- :ref:`18 - RGB Encoding with MobilenetSSD`

Reference
#########

.. autoclass:: depthai.VideoEncoder
  :members:
  :inherited-members: