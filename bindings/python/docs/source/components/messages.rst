.. _components_messages:

Messages
========

Messages are sent between linked :ref:`Nodes`. The only way nodes communicate with each other is by sending messages from one to another. On the
table of contents (left side of the page) **all DepthAI messages are listed** under the :code:`Messages` entry. You can click on them to find out more.

.. rubric:: Creating a message in Script node

A DepthAI message can be created either on the device, by a node automatically or manually inside the :ref:`Script` node. In below example,
the code is taken from the :ref:`Script camera control` example, where :ref:`CameraControl` is created inside the Script node every second
and sent to the :ref:`ColorCamera`'s input (:code:`cam.inputControl`).

.. code-block:: python

   script = pipeline.create(dai.node.Script)
   script.setScript("""
      # Create a message
      ctrl = CameraControl()
      # Configure the message
      ctrl.setCaptureStill(True)
      # Send the message from the Script node
      node.io['out'].send(ctrl)
   """)

.. rubric:: Creating a message on a Host

It can also be created on a host computer and sent to the device via :ref:`XLinkIn` node. :ref:`RGB Camera Control`, :ref:`Video & MobilenetSSD`
and :ref:`Stereo Depth from host` code examples demonstrate this functionality perfectly. In the example below, we have removed all the code
that isn't relevant to showcase how a message can be created on the host and sent to the device via XLink.

.. code-block:: python

   # Create XLinkIn node and configure it
   xin = pipeline.create(dai.node.XLinkIn)
   xin.setStreamName("frameIn")
   xin.out.link(nn.input) # Connect it to NeuralNetwork's input

   with dai.Device(pipeline) as device:
      # Create input queue, which allows you to send messages to the device
      qIn = device.getInputQueue("frameIn")
      # Create ImgFrame message
      img = dai.ImgFrame()
      img.setData(frame)
      img.setWidth(300)
      img.setHeight(300)
      qIn.send(img) # Send the message to the device

.. rubric:: Creating a message on an external MCU

A message can also be created on an external MCU and sent to the device via :ref:`SPIIn` node. An demo of such functionality is the
`spi_in_landmark <https://github.com/luxonis/esp32-spi-message-demo/tree/main/spi_in_landmark>`__ example.

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:

   messages/*

.. include::  ../includes/footer-short.rst
