POE set IP
==========

This script allows you to set static or dynamic IP, get the currently set IP, or clear bootloader config on your OAK-POE device.

.. warning::
    Make sure to **set mask and gateway correctly!** If they are set incorrectly you will soft-brick your
    device (you won't be able to access it), and will have to `factory reset <https://docs.luxonis.com/projects/hardware/en/latest/pages/guides/getting-started-with-poe.html#factory-reset>`__
    your OAK PoE.

.. note::
    The script currently doesn't support setting a IPv6 address.
    We suggest using :ref:`Device Manager`, a GUI tool for interfacing with the bootloader and its configurations.

Demo
####

Use cases:
.. code-block:: bash
    # Set a static IPv4
    python3 poe_set_ip.py static <static IPv4 to set> <mask>
    # Set a static IPv4 and gateway
    python3 poe_set_ip.py static <static IPv4 to set> <mask> --gateway <Gateway IPv4>
    # Set a dynamic IPv4 and gateway
    python3 poe_set_ip.py dynamic <dynamic IPv4 to set> <mask> --gateway <Gateway IPv4>
    # Get the currently set IPv4
    python3 poe_set_ip.py get

Example of how to set a static IPv4 address:

.. code-block:: bash
    python3 poe_set_ip.py static 192.168.1.200 255.255.255.0 --gateway 192.168.1.1
    -------------------------------------
    Flashing successful.

If you run the same example again after 10 seconds, you verify that the IP was changed to **192.168.1.200**:

.. code-block:: bash
    python3 poe_set_ip.py get
    -------------------------------------
    Device (IPv4): 192.168.1.200, Mask: 255.255.255.0, Gateway: 192.168.1.1

You can now also use the `Manually specify device IP <https://docs.luxonis.com/projects/hardware/en/latest/pages/guides/getting-started-with-poe.html#manually-specify-device-ip>`__
script and change the IP to :code:`192.168.1.200`.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/bootloader/poe_set_ip.py>`__

        .. literalinclude:: ../../../../examples/bootloader/poe_set_ip.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/bootloader/poe_set_ip.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/bootloader/poe_set_ip.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
