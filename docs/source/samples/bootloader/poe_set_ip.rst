POE set IP
==========

This script allows you to set static or dynamic IP, or clear bootloader config on your OAK-POE device.

Demo
####

Example script output:

.. code-block:: bash

    Found device with name: 192.168.1.136
    -------------------------------------
    "1" to set a static IPv4 address
    "2" to set a dynamic IPv4 address
    "3" to clear the config
    1
    -------------------------------------
    Enter IPv4: 192.168.1.200
    Enter IPv4 Mask: 255.255.255.0
    Enter IPv4 Gateway: 255.255.255.255
    Flashing static IPv4 192.168.1.200, mask 255.255.255.0, gateway 255.255.255.255 to the POE device. Enter 'y' to confirm. y
    Flashing successful.

If you run the same example again after 10 seconds, you will see that IP changed to **192.168.1.200**:

.. code-block:: bash

    Found device with name: 192.168.1.200
    -------------------------------------
    "1" to set a static IPv4 address
    "2" to set a dynamic IPv4 address
    "3" to clear the config

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
