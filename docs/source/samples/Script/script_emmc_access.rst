Script EMMC access
==================

.. note::

    This example requires a device with onboard EMMC memory (e.g. OAK-1-POE). 
    To check whether your device has EMMC memory, run the bootloader version script at :ref:`Bootloader Version` and check whether the output contains ``Memory.EMMC``.



This example shows how to use :ref:`Script` node to access EMMC memory of the device. Default location for EMMC memory is ``/media/mmcsd-0-0/``. The first script in the pipeline works by writing an image to EMMC memory. 
The second script starts a webserver on ::code:`/media/mmcsd-0-0/` directory and serves the image from EMMC memory.


Setup
#####

.. include::  /includes/install_from_pypi.rst


Prerequisites
#############

We first need to enable the EMMC memory as storage on the device. To do so, we need to flash the device with an application that has EMMC enabled. 

Example application:

.. code-block:: python

    import depthai as dai

    # Create pipeline
    pipeline = dai.Pipeline()

    # Set board config
    board = dai.BoardConfig()
    board.emmc = True
    config = dai.Device.Config()
    config.board = board
    pipeline.setBoardConfig(board)

    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    bootloader = dai.DeviceBootloader(bl)
    progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
    (r, errmsg) = bootloader.flash(progress, pipeline, memory=dai.DeviceBootloader.Memory.EMMC)
    if r: print("Flash OK")


The above code will flash the device with the application that enables the script node to access EMMC memory. Now we should be able to access EMMC memory even when the device is in standard mode (connected to the host PC).

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/Script/script_emmc_access.py>`__

        .. literalinclude:: ../../../../examples/Script/script_emmc_access.py
           :language: python
           :linenos:

.. include::  /includes/footer-short.rst
