Latency measurement
===================

This example shows how to :ref:`ImgFrame`'s ``.getTimestamp()`` function in combination with ``dai.Clock.now()`` to measure
the latency since image was captured (more accurately since it was processed by ISP and timestamp was attached to it) until
the frame was received on the host computer.

If you would like to **learn more about low-latency, see** :ref:`documentation page here <Low latency>`.

Demo
####

This example measures latency of ``isp`` 1080P output (YUV420 encoded frame) from :ref:`ColorCamera` running at 60FPS. We get
about 33ms, which is what was measured in :ref:`Low latency` docs page as well.

.. code-block::

    UsbSpeed.SUPER
    Latency: 34.16 ms, Average latency: 34.16 ms
    Latency: 34.92 ms, Average latency: 34.54 ms
    Latency: 32.36 ms, Average latency: 33.81 ms
    Latency: 31.70 ms, Average latency: 33.28 ms
    Latency: 32.95 ms, Average latency: 33.22 ms
    Latency: 31.74 ms, Average latency: 32.97 ms
    Latency: 32.83 ms, Average latency: 32.95 ms
    Latency: 37.01 ms, Average latency: 33.46 ms
    Latency: 32.93 ms, Average latency: 33.40 ms
    Latency: 31.73 ms, Average latency: 33.23 ms


Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/host_side/latency_measurement.py>`__

        .. literalinclude:: ../../../../examples/host_side/latency_measurement.py
           :language: python
           :linenos:

    .. tab:: C++

        (Work in progress)