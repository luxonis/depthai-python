Multiple DepthAI per Host
=========================

You can find `Demo scripts here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-multiple-devices>`__.
Learn how to discover multiple OAK cameras connected to your system, and use them individually.

.. image:: /_static/images/tutorials/multiple/setup.jpg
  :alt: face

Shown on the left is Luxonis `OAK-1 <https://shop.luxonis.com/products/bw1093>`__ which is actually plugged into
an `OAK-D-CM3 <https://shop.luxonis.com/products/depthai-rpi-compute-module-edition>`__.

So in this case, everything is running on the (single) Raspberry Pi 3B+ host which is in the back of the OAK-D-CM3.

Discovering OAK cameras
#######################

You can use DepthAI to discover all connected OAK cameras, either via USB or through the LAN (OAK POE cameras).
The code snippet below finds all OAK cameras and prints their MxIDs (unique identifier) and their XLink state.

.. code-block:: python

  import depthai
  for device in depthai.Device.getAllAvailableDevices():
      print(f"{device.getMxId()} {device.state}")

Example results for 2x DepthAI on a system:

.. code-block::

  14442C10D13EABCE00 XLinkDeviceState.X_LINK_UNBOOTED
  14442C1071659ACD00 XLinkDeviceState.X_LINK_UNBOOTED

Selecting a Specific DepthAI device to be used
##############################################

From the Detected devices(s) above, use the following code to select the device you would like to use with your pipeline.
For example, if the first device is desirable from above use the following code:

.. code-block:: python

  # Specify MXID, IP Address or USB path
  device_info = depthai.DeviceInfo("14442C108144F1D000") # MXID
  #device_info = depthai.DeviceInfo("192.168.1.44") # IP Address
  #device_info = depthai.DeviceInfo("3.3.3") # USB port name
  with depthai.Device(pipeline, device_info) as device:
      # ...

And you can use this code as a basis for your own use cases, such that you can run differing neural models
on different OAK models.

Specifying POE device to be used
********************************

You can specify the POE device to be used by the IP address as well, as shown in the code snippet above.

Now use as many OAK cameras as you need!
And since DepthAI does all the heavy lifting, you can usually use quite a few of them with very little burden to the host.

.. include::  /includes/footer-short.rst
