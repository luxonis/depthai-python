XLinkIn
=======

XLinkIn node is used to send data from the host to the device via XLink.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlink_in = pipeline.createXLinkIn()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkIn = pipeline.create<dai::node::XLinkIn>();


Inputs and Outputs
##################

.. code-block::

              ┌──────────────┐
              │              │
  (From host) │              │    Out
  ----------->│   XLinkIn    ├────────►
              │              │
              │              │
              └──────────────┘

Message types
#############

- :code:`Out` - :ref:`Buffer`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlink_in = pipeline.createXLinkIn()
    xlink_in.setStreamName("cam_control")

    # Create ColorCamera beforehand
    xlink_in.out.link(cam.inputControl)

    with dai.Device(pipeline) as device:
      q_cam_control = device.getInputQueue("cam_control")

      # Send a message to the ColorCamera to capture a still image
      ctrl = dai.CameraControl()
      ctrl.setCaptureStill(True)
      q_cam_control.send(ctrl)


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkIn = pipeline.create<dai::node::XLinkIn>();


Examples of functionality
#########################

- :ref:`14 - Color Camera Control`
- :ref:`17 - Video & MobilenetSSD`
- :ref:`20 - Color Rotate Warp`

Reference
#########

.. autoclass:: depthai.XLinkIn
  :members:
  :inherited-members: