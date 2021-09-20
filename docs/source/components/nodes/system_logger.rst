SystemLogger
============

SystemLogger node is used to get :ref:`system information <SystemInformation>` of the device.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    logger = pipeline.create(dai.node.SystemLogger)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto logger = pipeline.create<dai::node::SystemLogger>();


Inputs and Outputs
##################

.. code-block::

  ┌──────────────┐
  │              │
  │              │    out
  │ SystemLogger ├────────►
  │              │
  │              │
  └──────────────┘

**Message types**

- :code:`out` - :ref:`SystemInformation`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    logger = pipeline.create(dai.node.SystemLogger)
    logger.setRate(1)  # 1 Hz

    # Send system info to the host via XLink
    xout = pipeline.create(dai.node.XLinkOut)
    xout.setStreamName("sysinfo")
    logger.out.link(xout.input)


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto logger = pipeline.create<dai::node::SystemLogger>();\
    logger->setRate(1.0f);  // 1 Hz

    // Send system info to the host via XLink
    auto xout = pipeline.create<dai::node::XLinkOut>();
    xout->setStreamName("sysinfo");
    logger->out.link(xout->input);

Examples of functionality
#########################

- :ref:`System information`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.SystemLogger
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::SystemLogger
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
