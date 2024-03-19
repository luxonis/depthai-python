SPIOut
======

SPIOut node is used for **sending data to a MCU** (via SPI).
You can find demos `here <https://github.com/luxonis/esp32-spi-message-demo>`__.

:ref:`SPIIn` is used for receiving data from the MCU (via SPI).

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spi = pipeline.create(dai.node.SPIOut)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spi = pipeline.create<dai::node::SPIOut>();


Inputs and Outputs
##################

.. code-block::

            ┌──────────────┐
            │              │
  input     │              │ SPI (to MCU)
  ─────────►│    SPIOut    ├------------►
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
    spi = pipeline.create(dai.node.SPIOut)

    spi.setStreamName("spimetaout")
    spi.setBusId(0)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spi = pipeline.create<dai::node::SPIOut>();

    spi->setStreamName("spimetaout");
    spi->setBusId(0);

Examples of functionality
#########################

- `SPI code demos with ESP32 <https://github.com/luxonis/esp32-spi-message-demo>`__

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.SPIOut
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::SPIOut
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
