Script UART communication
=========================

This example uses :ref:`Script` node for `UART communication <https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter>`__. Note that OAK
cameras don't have UART pins easily disposed, and we soldered wires on `OAK-FFC-4P <https://docs.luxonis.com/projects/hardware/en/latest/pages/DD2090.html>`__
to expose UART pins.

.. note::

    This should only be run on OAK-FFC-4P, as other OAK cameras might have different GPIO configuration.

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/nbS1BczO5sE" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>


.. figure:: https://user-images.githubusercontent.com/18037362/232458809-a36dc418-6bb5-411f-9172-5130a926191d.jpg

    Oscilloscope connected to the OAK-FFC-4P UART pins

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/Script/script_uart.py>`__

        .. literalinclude:: ../../../../examples/Script/script_uart.py
           :language: python
           :linenos:

    .. tab:: C++

        Not yet available


.. include::  /includes/footer-short.rst
