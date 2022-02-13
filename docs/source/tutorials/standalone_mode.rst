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

..
    Clear flash
    ###########

    Since pipeline will start when powering the device, this can lead to unnecesary heating. If you would like to clear
    the flashed pipeline, use the code snippet below.

    .. code-block:: python

        import depthai as dai
        (ok, info) = dai.DeviceBootloader.getFirstAvailableDevice()
        if ok:
            print(f'Clearing flash of the OAK: {info.desc.name}');
            with dai.DeviceBootloader(info) as bl:
                (success, error) = bl.flashConfigClear()
                if error: print("Error:", error)
                else: print("Successfully cleared flash")
        else: print("No device found")

.. include::  ../includes/footer-short.rst
