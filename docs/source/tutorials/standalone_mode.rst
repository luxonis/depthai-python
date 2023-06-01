Standalone mode
===============

**Standalone / Hostless / On-The-Edge mode** means that the OAK camera isn't connected to a host computer. This can
be achieved by first :ref:`flashing the bootloader <Flash the bootloader>` and then :ref:`flashing the pipeline <Flash the pipeline>`
and assets (NN models) to the OAK's flash memory.

Standalone mode is **only possible on OAKs that have on-board flash** memory, which are currently
`OAK POE <https://docs.luxonis.com/projects/hardware/en/latest/#poe-designs>`__ and OAK IOT camera models.

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

Flash the bootloader
####################

The :ref:`Bootloader` is packaged together with the depthai, so if you have the latest depthai version, you can flash the
latest bootloader version. To flash the bootloader, we suggest using the :ref:`Device Manager`. To view the API code behind
it, see :ref:`Flash Bootloader` example code.

Flash the pipeline
##################

After you have standalone :ref:`Pipeline` defined and :ref:`Bootloader` already flashed on the device, you
can flash the pipeline to the device, along with its assests (eg. AI models). You can flash the pipeline with the following snippet:

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

Alternatively, you can also flash the pipeline with the :ref:`Device Manager`. For this approach, you will need a Depthai Application Package (.dap), which you
can create with the following script:


.. code-block:: python

    import depthai as dai

    pipeline = dai.Pipeline()

    # Define standalone pipeline; add nodes and link them
    # cam = pipeline.create(dai.node.ColorCamera)
    # script = pipeline.create(dai.node.Script)
    # ...

    # Create Depthai Application Package (.dap)
    (f, bl) = dai.DeviceBootloader.getFirstAvailableDevice()
    bootloader = dai.DeviceBootloader(bl)
    bootloader.saveDepthaiApplicationPackage(pipeline=pipeline, path=<path_of_new_dap>)


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
we recommend using the :ref:`Device Manager`. Factory reset will also flash the latest bootloader.

.. include::  ../includes/footer-short.rst
