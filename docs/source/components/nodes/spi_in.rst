SPIIn
=====

SPIIn node is used for **receiving data** that was sent **from a MCU** (via SPI).
You can find demos `here <https://github.com/luxonis/esp32-spi-message-demo>`__.

This allows you for example to control eg. :ref:`ColorCamera` or :ref:`ImageManip` from the MCU or send a :ref:`Buffer` of data from the MCU to a :ref:`Script` node.

:ref:`SPIOut` is used for sending data from the VPU to a MCU (via SPI).

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spi = pipeline.create(dai.node.SPIIn)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spi = pipeline.create<dai::node::SPIIn>();


Inputs and Outputs
##################

.. code-block::

             ┌─────────────┐
  SPI        │             │
  (from MCU) |             │      out
  ----------►│    SPIIn    ├─────────►
             │             │
             │             │
             └─────────────┘

**Message types**

- :code:`out` - :code:`Any`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spi = pipeline.create(dai.node.SPIIn)

    spi.setStreamName("control")
    spi.setBusId(0)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spi = pipeline.create<dai::node::SPIIn>();

    spi->setStreamName("control");
    spi->setBusId(0);

Examples of functionality
#########################

- `SPI code demos with ESP32 <https://github.com/luxonis/esp32-spi-message-demo>`__

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.SPIIn
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::SPIIn
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
