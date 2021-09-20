XLinkOut
========

XLinkOut node is used to send data from the device to the host via XLink.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xlinkOut = pipeline.create(dai.node.XLinkOut)

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

- :code:`input` - :code:`Any`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    xOut = pipeline.create(dai.node.XLinkOut)
    xOut.setStreamName("camOut")

    # Here we will send camera preview (ImgFrame) to the host via XLink.
    # Host can then display the frame to the user
    cam.preview.link(xOut.input)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto xOut = pipeline.create<dai::node::XLinkOut>();
    xOut->setStreamName("camOut");

    // Here we will send camera preview (ImgFrame) to the host via XLink.
    // Host can then display the frame to the user
    cam->preview.link(xOut->input);

Examples of functionality
#########################

- :ref:`RGB Preview`
- :ref:`Mono & MobilenetSSD`
- :ref:`Mono & MobilenetSSD & Depth`


Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.XLinkOut
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::XLinkOut
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
