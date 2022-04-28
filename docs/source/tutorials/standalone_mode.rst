Standalone mode
===============

**Standalone / Hostless / On-The-Edge mode** means that the OAK camera isn't connected to a host computer. This can
be achieved by first :ref:`flashing the bootloader <Flash bootloader>` and then :ref:`flashing the pipeline <Flash pipeline>`
and assets (NN models) to the OAK's flash memory.

Standalone mode is **only possible on OAKs that have on-board flash** memory, which are currently `OAK IOT <https://docs.luxonis.com/projects/hardware/en/latest/#iot-designs>`__
and `OAK POE <https://docs.luxonis.com/projects/hardware/en/latest/#poe-designs>`__ camera models.

Converting a demo to standalone mode
####################################

Since there won't be any communication between the host and the device, you first need to remove all
:ref:`XLinkOut` and :ref:`XLinkIn` nodes. This means that the device will only communicate with the "outside world"
via either SPI (:ref:`SPIOut`/:ref:`SPIIn`) or :ref:`Script` node (GPIO/UART or network protocols if you have
OAK POE mode; HTTP/TCP/UDP...).

Next thing you can also remove the host-side code, which usually looks something like this:

.. code-block:: python

    with dai.Device(pipeline) as device:
        videoQ = device.getOutputQueue("video")
        faceDetQ = device.getOutputQueue("face_det")
        nnQ = device.getOutputQueue("nn")

        while True:
            frame = videoQ.get().getCvFrame()
            # ...


After you remove all host-side code, you would only be left with the :ref:`Pipeline` definition (with nodes/links).
Since device no longer communicates with the host, you need to "route" your program's output through either SPI
or script node, as mentioned above.

Flash bootloader
################

Execute the code below to flash the :ref:`Bootloader` to the device. The bootloader is packaged together with the
depthai, so if you have the latest depthai version, you will flash the latest bootloader version. This step
is required only once.

.. code-block:: python

    import depthai as dai
    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    bootloader = dai.DeviceBootloader(bl)
    progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
    bootloader.flashBootloader(progress)

Flash pipeline
##############

After you have standalone :ref:`Pipeline` definition and :ref:`Bootloader` already flashed on the device, you
can start with flashing the pipeline. You can flash the pipeline with the following snippet:

.. code-block:: python

        import depthai as dai

        pipeline = dai.Pipeline()

        # Define standalone pipeline; add nodes and link them
        # cam = pipeline.create(dai.node.ColorCamera)
        # script = pipeline.create(dai.node.Script)
        # ...

        # Flash the pipeline
        (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
        bootloader = dai.DeviceBootloader(bl)
        progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
        bootloader.flash(progress, pipeline)

After successfully flashing the pipeline, it will get started automatically when you power up the device.
If you would like to change the flashed pipeline, simply re-flash it again.

Clear flash
###########

Since pipeline will start when powering the device, this can lead to unnecesary heating. If you would like to clear
the flashed pipeline, use the code snippet below.

.. warning::
    Code below doesn't work yet. We will be adding "flashClear" helper function to the library.

.. code-block:: python

    import depthai as dai
    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    if not f:
        print('No devices found, exiting...')
        exit(-1)

    with dai.DeviceBootloader(bl) as bootloader:
        bootloader.flashClear()

Factory reset
#############

In case you have soft-bricked your device, or just want to clear everything (flashed pipeline/assets and bootloader config),
we recommend running the factory reset script below. It will also flash the latest bootloader version.

.. code-block:: python

    import depthai as dai
    import tempfile

    blBinary = dai.DeviceBootloader.getEmbeddedBootloaderBinary(dai.DeviceBootloader.Type.NETWORK)
    blBinary = blBinary + ([0xFF] * ((8 * 1024 * 1024 + 512) - len(blBinary)))

    with tempfile.NamedTemporaryFile() as tmpBlFw:
        tmpBlFw.write(bytes(blBinary))

        (f, device_info) = dai.DeviceBootloader.getFirstAvailableDevice()
        if not f:
            print('No devices found, exiting...')
            exit(-1)

        with dai.DeviceBootloader(device_info, allowFlashingBootloader=True) as bootloader:
            progress = lambda p : print(f'Factory reset progress: {p*100:.1f}%')
            # Override SBR table, to prevent booting flashed application
            [success, msg] = bootloader.flashBootloader(progress, tmpBlFw.name)
            if success:
                print('Successfully overwritten SBR table. Device should now be reacheable through PoE')
            else:
                print(f"Couldn't overwrite SBR table to unbrick the device. Error: {msg}")

You can also **factory reset OAK POE at specific IP** if it's not reachable (not in same LAN).

.. code-block:: python

    import depthai as dai
    import tempfile

    blBinary = dai.DeviceBootloader.getEmbeddedBootloaderBinary(dai.DeviceBootloader.Type.NETWORK)
    blBinary = blBinary + ([0xFF] * ((8 * 1024 * 1024 + 512) - len(blBinary)))

    with tempfile.NamedTemporaryFile() as tmpBlFw:
        tmpBlFw.write(bytes(blBinary))

        device_info = dai.DeviceInfo()
        device_info.state = dai.XLinkDeviceState.X_LINK_BOOTLOADER
        device_info.desc.protocol = dai.XLinkProtocol.X_LINK_TCP_IP
        device_info.desc.name = "192.168.34.110" # Set IP here

        with dai.DeviceBootloader(device_info, allowFlashingBootloader=True) as bootloader:
            progress = lambda p : print(f'Factory reset progress: {p*100:.1f}%')
            # Override SBR table, to prevent booting flashed application
            [success, msg] = bootloader.flashBootloader(progress, tmpBlFw.name)
            if success:
                print('Successfully overwritten SBR table. Device should now be reacheable through PoE')
            else:
                print(f"Couldn't overwrite SBR table to unbrick the device. Error: {msg}")


.. include::  ../includes/footer-short.rst
