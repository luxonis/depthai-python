UVC
===

The DepthAI UVC (`USB Video Class <https://en.wikipedia.org/wiki/USB_video_device_class>`__) node allows OAK devices to function as standard webcams. This feature is particularly useful for integrating OAK devices into applications that require video input, such as video conferencing tools or custom video processing applications.

What is UVC?
############

UVC refers to the USB Video Class standard, which is a USB device class that describes devices capable of streaming video. This standard allows video devices to interface with computers and other devices without needing specific drivers, making them immediately compatible with a wide range of systems and software.

How Does the UVC Node Work?
###########################

The UVC node in DepthAI leverages this standard to stream video from OAK devices. When the UVC node is enabled, the OAK device is recognized as a standard webcam by the host system. This allows the device to be used in any application that supports webcam input, such as Zoom, Skype, or custom video processing software.

The UVC node streams video data over a USB connection. It is important to use a USB3 cable for this purpose, as USB2 may not provide the necessary bandwidth for stable video streaming.

.. note::

    The UVC node can currently handle NV12 video streams from OAK devices. For streams in other formats, conversion to NV12 is necessary, which can be achieved using the :ref:`ImageManip` node. It's important to note that streams incompatible with NV12 conversion, like depth streams, are not supported by the UVC node.

Examples of UVC Node Usage
##########################

1. **DepthAI Demo Script**: The DepthAI demo script includes a UVC application that can be run to enable the UVC node on an OAK device.

   .. code-block:: bash

       python3 depthai_demo.py --app uvc

2. **Custom Python Script**: A custom Python script can be written to enable the UVC node and configure the video stream parameters. Here are some pre-written examples:

   - :ref:`UVC & Color Camera`
   - :ref:`UVC & Mono Camera`
   - :ref:`UVC & Disparity`


3. **OBS Forwarding**: For applications where direct UVC node usage is not possible, OBS Studio can be used to forward the UVC stream.


Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.UVC
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::UVC
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:


.. include:: ../../includes/footer-short.rst
