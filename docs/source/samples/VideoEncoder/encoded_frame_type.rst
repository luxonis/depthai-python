Video Encoded Frame Type
========================

This example showcases how to compress and print the sequence of encoded frame types (I, P, B) encountered during the encoding process.


Example script output
#####################

.. code-block::

    ~/depthai-python/examples$ python3 video_encoding.py
    Press Ctrl+C to stop encoding...
    To view the encoded data, convert the stream file (.h265) into a video file (.mp4) using a command below:
    ffmpeg -framerate 30 -i video.h265 -c copy video.mp4
    
    1I,29P,1I,29P,1I,29P,1I,29P,1I,29P,1I,29P,1I,29P,1I,29P,1I,29P,1I,18P

This output indicates the script's real-time video encoding process, and it compresses the sequence of frame types encountered. After stopping the script, it provides instructions to convert the raw H.265 stream into a playable MP4 video file. The final line shows a compressed representation of the frame types sequence, indicating the count of consecutive P, B, and I frames.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/VideoEncoder/rgb_encoding_encodedframe.py>`__

        .. literalinclude:: ../../../../examples/VideoEncoder/rgb_encoding_encodedframe.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/VideoEncoder/rgb_encoding_encodedframe.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/VideoEncoder/rgb_encoding_encodedframe.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst