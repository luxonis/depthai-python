Bootloader Version
==================

This example shows basic bootloader interaction, retrieving the version of bootloader running on the device.

Click on :ref:`Bootloader` for more information.

.. note::
    We suggest using :ref:`Device Manager`, a GUI tool for interfacing with the bootloader and its configurations.

Demo
####

Example script output

.. code-block:: bash

    ~/depthai-python/examples$ python3 bootloader_version.py
    Found device with name: 1.1
    Version: 0.0.26
    USB Bootloader - supports only Flash memory
    Memory 'Memory.FLASH' size: 33554432, info: JEDEC ID: 01 02 19

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/bootloader/bootloader_version.py>`__

        .. literalinclude:: ../../../../examples/bootloader/bootloader_version.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/bootloader/bootloader_version.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/bootloader/bootloader_version.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
