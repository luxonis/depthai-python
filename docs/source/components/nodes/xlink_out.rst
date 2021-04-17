XLinkOut
========

XLinkOut node is used to send data from the device to the host via XLink.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlinkOut = pipeline.createXLinkOut()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xlinkOut = pipeline.create<dai::node::XLinkOut>();


Inputs and Outputs
##################

.. code-block::

            ┌──────────────┐
            │              │
  input     │              │   (to host)
  ─────────►│   XLinkOut   ├------------►
            │              │
            │              │
            └──────────────┘

**Message types**

- :code:`input` - :ref:`Buffer`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xOut = pipeline.createXLinkOut()
    xOut.setStreamName("camOut")

    # Here we will send camera preview (ImgFrame) to the host via XLink. Host can then display the frame to the user
    cam.preview.link(xOut.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xOut = pipeline.create<dai::node::XLinkOut>();
    xOut->setStreamName("camOut");

    # Here we will send camera preview (ImgFrame) to the host via XLink. Host can then display the frame to the user
    cam->preview.link(xOut->input);

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

.. include::  ../../includes/footer-short.rst
