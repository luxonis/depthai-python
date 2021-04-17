SystemLogger
============

SystemLogger node is used to get :ref:`system information <SystemInformation>` of the device.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    logger = pipeline.createSystemLogger()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto logger = pipeline.create<dai::node::SystemLogger>();


Inputs and Outputs
##################

.. code-block::

  ┌──────────────┐
  │              │
  │              │    Out
  │ SystemLogger ├────────►
  │              │
  │              │
  └──────────────┘

Message types
#############

- :code:`Out` - :ref:`SystemInformation`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    logger = pipeline.createSystemLogger()
    logger.setRate(1)  # 1 Hz

    # Send system info to the host via XLink
    sys_xout = pipeline.createXLinkOut()
    sys_xout.setStreamName("sysinfo")
    logger.out.link(sys_xout.input)


  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto logger = pipeline.create<dai::node::SystemLogger>();

Examples of functionality
#########################

- :ref:`27 - Spatial location calculator`

Reference
#########

.. autoclass:: depthai.SystemLogger
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
