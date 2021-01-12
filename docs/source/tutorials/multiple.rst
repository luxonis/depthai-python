Multiple DepthAI per Host
=========================

Learn how to use the DepthAI :code:`-dev` option to discover the DepthAI connected to your system, and use them individually.

.. image:: /_static/images/tutorials/multiple/setup.jpg
  :alt: face

Shown on the left is Luxonis `uAI (BW1093) <https://shop.luxonis.com/products/bw1093>`__ which is actually plugged into
a `Raspberry Pi Compute Module Edition (BW1097) <https://shop.luxonis.com/products/depthai-rpi-compute-module-edition>`__.

So in this case, everything is running on the (single) Raspberry Pi 3B+ which is in the back of the BW1097.

Dependencies
############

You have already set up the Python API on your system (if you have a Raspberry Pi Compute Module it came pre-setup).
See :ref:`here <Python API>` if you have not yet installed the DepthAI Python API on your system.

Discover DepthAI-USB Port Mapping
#################################

The DepthAI multi-device support is currently done by selecting the USB port into which the DepthAI is plugged in.

If you'd like to associate a given DepthAI device with specific code (e.g. neural model) to be run on it, it is recommended
to plug in one device at a time, and then use the following command to determine which device is on which port:

.. code-block:: bash

  python3 depthai_demo.py -dev list

Example results for 2x DepthAI on a system:

.. code-block::

  ...
  XLink initialized.
  Detected 2 device(s):
    2-ma2480     on USB port: 1
    1.1-ma2480   on USB port: 2.1

Selecting a Specific DepthAI device to be used.
###############################################

From the Detected devices(s) above, use the following command to select the device you would like to use with your code.
For example, if the first device is desirable from above (the device on USB port 1), use the following command:

.. code-block:: bash

  python3 depthai_demo.py -dev 1

And then similarly, to run the same script again on the other second device, run it with:

.. code-block::

  python3 depthai_demo.py -dev 2.1

And you can use these scripts as a basis for your own modified versions, such that you can run differing neural models
on different DepthAI/uAI models.

Summary and Overview of Host-Side Burden
########################################

Now use as many DepthAI devices as you need!

And since DepthAI does all the heavy lifting, you can usually use quite a
few of them with very little burden to the host.

And it's worth noting that you can always disable the video stream by only only requesting :code:`metaout`
`here <https://github.com/luxonis/depthai-python-extras/blob/232d1e7529e0278b75192d0870a969b6c0e2d1ae/depthai.py#L104>`__.

So if you're using the metadata to say, drive a robot or make decisions with code, and the video isn't needed,
you can do this to substantially reduce the burden on the host - as since all the neural inference work is done on
DepthAI before getting to the host - almost all the host burden is just from displaying the video.

So with the video disabled, the host only has to handle a couple kilobytes a second in terms of metadata.

.. include::  /pages/includes/footer-short.rst
