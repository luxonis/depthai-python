SPIOut
======

SPIOut node is used to send data through to a MCU via SPI. `LUX-ESP32 <https://docs.luxonis.com/en/gen2/pages/products/bw1092/>`__ module has integrated an
integrated ESP32 connected to the MyriadX via SPI. You can find demos `here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-spi>`__.


How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    spi = pipeline.createSPIOut()

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
    spi = pipeline.createSPIOut()

    spi.setStreamName("spimetaout")
    spi.setBusId(0)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto spi = pipeline.create<dai::node::SPIOut>();

    spi->setStreamName("spimetaout");
    spi->setBusId(0);

Examples of functionality
#########################

- `SPI demos (host side) <https://github.com/luxonis/depthai-experiments/tree/master/gen2-spi>`__
- `ESP32 code demos <https://github.com/luxonis/esp32-spi-message-demo>`__

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.SPIOut
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
