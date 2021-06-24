XLinkIn
=======

XLinkIn node is used to send data from the host to the device via XLink.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlinkIn = pipeline.create(dai.node.XLinkIn)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkIn = pipeline.create<dai::node::XLinkIn>();


Inputs and Outputs
##################

.. code-block::

              ┌──────────────┐
              │              │
  (From host) │              │    out
  ----------->│   XLinkIn    ├────────►
              │              │
              │              │
              └──────────────┘

**Message types**

- :code:`out` - :code:`Any`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xIn = pipeline.create(dai.node.XLinkIn)
    xIn.setStreamName("camControl")

    # Create ColorCamera beforehand
    xIn.out.link(cam.inputControl)

    with dai.Device(pipeline) as device:
      device.startPipeline()
      qCamControl = device.getInputQueue("camControl")

      # Send a message to the ColorCamera to capture a still image
      ctrl = dai.CameraControl()
      ctrl.setCaptureStill(True)
      qCamControl.send(ctrl)


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xIn = pipeline.create<dai::node::XLinkIn>();
    xIn->setStreamName("camControl");

    // Create ColorCamera beforehand
    xIn->out.link(cam->inputControl);

    // Connect to the device
    dai::Device device(pipeline);
    device.startPipeline();

    auto qCamControl = device.getInputQueue("camControl");

    // Send a message to the ColorCamera to capture a still image
    dai::CameraControl ctrl;
    ctrl.setCaptureStill(true);
    qCamControl->send(ctrl)


Examples of functionality
#########################

- :ref:`RGB Camera Control`
- :ref:`Video & MobilenetSSD`
- :ref:`RGB Rotate Warp`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.XLinkIn
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::XLinkIn
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
