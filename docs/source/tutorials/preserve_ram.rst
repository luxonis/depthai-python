How to preserve RAM
===================

The amount of memory consumed can be measured in debug mode with the following lines of code.

.. code-block:: python
    
    device.setLogLevel(dai.LogLevel.DEBUG)
    device.setLogOutputLevel(dai.LogLevel.DEBUG)

There might be some memory issues when creating a larger number (around 10) of image manip, config and manip output nodes.

A simple way to reduce the memory consumed is to reduce the video resolution to 1080P, however there are methods to reduce it further.

Each ImageManip instance creates an output pool of 4 frames by default, this number can be controller with :code:`setNumFramesPool`. Each frame has 1 MiB of size, but that can also be controlled using the :code:`setMaxOutputFrameSize`, eg. if the output is 300x300 RGB, only 300x300x3=270000 bytes are needed.

Furthermore, you can lower the data size for the XLinkIn connections using the :code:`setMaxDataSize` to set the number of bytes.
If XLinkIn is used to send config message, then 5*1024 is more than enough, if it's used to send image data then it should be the number of bytes that the image takes, e.g. for 1920x1080 RGB it would be 1920*1080*3.
