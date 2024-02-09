Flash Bootloader
================

This script will flash :ref:`factory bootloader <Factory and User bootloader>` to the connected OAK camera. It will
also clear the :ref:`user bootloader <Factory and User bootloader>` (if it exists).
:ref:`Bootloader` can only be flashed to devices that have on-board flash memory.

.. note::
    We suggest using :ref:`Device Manager`, a GUI tool for interfacing with the bootloader and its configurations.

Demo
####

Example script output

.. code-block:: bash

    ~/depthai-python/examples$ python3 flash_bootloader.py
    [0] 1844301041C83D0E00 [X_LINK_USB_VSC]
    Which DepthAI device to flash bootloader for [0..0]: 0
    Booting latest bootloader first, will take a tad longer...
    Bootloader version to flash: 0.0.26
    No config found, skipping erasing user bootloader
    Flashing USB bootloader...
    Flashing progress: 0.0%
    Flashing progress: 18.8%
    Flashing progress: 31.2%
    Flashing progress: 48.2%
    Flashing progress: 94.2%
    Flashing progress: 100.0%
    Flashing successful. Took 7.55600329185836 seconds

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/bootloader/flash_bootloader.py>`__

        .. literalinclude:: ../../../../examples/bootloader/flash_bootloader.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/bootloader/flash_bootloader.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/bootloader/flash_bootloader.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
