RGB & Mono Encoding
===================

This example shows how to set up the encoder node to encode the RGB camera and both grayscale cameras
(of DepthAI/OAK-D) at the same time. The RGB is set to 1920x1080 and the grayscale are set to 1280x720
each, all at 30FPS. Each encoded video stream is transferred over XLINK and saved to a respective file.

Pressing Ctrl+C will stop the recording and then convert it using ffmpeg into an mp4 to make it
playable. Note that ffmpeg will need to be installed and runnable for the conversion to mp4 to succeed.

Be careful, this example saves encoded video to your host storage. So if you leave it running,
you could fill up your storage on your host.

.. rubric:: Similar samples:

- :ref:`RGB Encoding`
- :ref:`Encoding Max Limit`
- :ref:`RGB Encoding & MobilenetSSD`
- :ref:`RGB Encoding & Mono & MobilenetSSD`
- :ref:`RGB Encoding & Mono with MobilenetSSD & Depth`

.. include::  /includes/container-encoding.rst

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/DWlkMjjH68M" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/VideoEncoder/rgb_mono_encoding.py>`__

        .. literalinclude:: ../../../../examples/VideoEncoder/rgb_mono_encoding.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/VideoEncoder/rgb_mono_encoding.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/VideoEncoder/rgb_mono_encoding.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
