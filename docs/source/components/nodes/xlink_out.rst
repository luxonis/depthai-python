XLinkOut
========

XLinkOut node is used to send data from the device to the host via XLink.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlink_out = pipeline.createXLinkOut()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkOut = pipeline.create<dai::node::XLinkOut>();


Inputs and Outputs
##################

.. code-block::

            ┌──────────────┐
            │              │
  Input     │              │   (to host)
  ─────────►│   XLinkOut   ├------------►
            │              │
            │              │
            └──────────────┘

Message types
#############

- :code:`Input` - :ref:`Buffer`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlink_out = pipeline.createXLinkOut()
    xlink_out.setStreamName("cam_out")

    # Here we will send camera preview (ImgFrame) to the host via XLink. Host can then display the frame to the user
    cam.preview.link(xlink_out.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkOut = pipeline.create<dai::node::XLinkOut>();

Examples of functionality
#########################

- :ref:`01 - RGB Preview`
- :ref:`09 - Mono & MobilenetSSD`
- :ref:`10 - Mono & MobilenetSSD & Encoding`


Reference
#########

.. autoclass:: depthai.XLinkOut
  :members:
  :inherited-members: