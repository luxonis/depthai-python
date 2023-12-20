Standalone mode
===============

**Standalone / Hostless / On-The-Edge mode** means that the camera starts the (:ref:`flashed <Flash the pipeline>`) application as soon as it gets power, without being connected to any particular host computer.
This is useful for applications where camera is stationary and just inspecting the world and providing analytics (eg. people/vehicle counting, LPR, fall detection, etc.)

Usually, this mode is also more robust to any instabilities (eg. networking issues, where connection between camera and host computer would drop), as application will restart automatically.

Standalone mode is **only possible on OAKs that have on-board flash** memory, which are currently
`OAK POE <https://docs.luxonis.com/projects/hardware/en/latest/#poe-designs>`__ and OAK IOT camera models.

**Scenarios** when standalone mode is particularly useful:

- People/vehicle/object tracking and counting (People counting `demo here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-people-tracker>`__). Each camera can do its own counting (inside Script node), and only send final count (eg. every hour) to some server via MQTT/HTTP.
- License Plate Recognition (LPR) camera (`demo here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-license-plate-recognition>`__). Each camera does vehicle detection, license plate detection and LPR, and only reports license plate (in string) to a server.
- Fall detection for elderly people (`demo here <https://www.youtube.com/watch?v=npiG-Dy7yQ4>`__). Each camera tracks people and checks their poses for any anomalies (eg. person falling down). If anomaly is detected, it sends an alert to a server.

In case you already have a computer on-board (eg. a robot/drone), standalone mode isn't as useful, and just adds extra complexity.

Communication with the camera
#############################

To "communicate" with the outside world (eg. a server), POE cameras can use :ref:`Script` node to send/receive networking packets (HTTP/TCP/UDP...). Here are a few examples:

- `TCP streaming <https://github.com/luxonis/depthai-experiments/tree/master/gen2-poe-tcp-streaming>`__ (camera being either server or client)
- `HTTP server <https://docs.luxonis.com/projects/api/en/latest/samples/Script/script_mjpeg_server/#script-mjpeg-server>`__
- `HTTP client <https://docs.luxonis.com/projects/api/en/latest/samples/Script/script_http_client/>`__
- `MQTT client <https://github.com/luxonis/depthai-experiments/tree/master/gen2-poe-mqtt>`__


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

To run the application on the camera, a `Bootloader` is required. Note that bootloader is already flashed on all POE cameras, as it's also required for network booting.
The :ref:`Bootloader` is packaged together with the depthai, so if you have the latest depthai version, you can flash the
latest bootloader version. To flash the latest bootloader, we suggest using the :ref:`Device Manager`. To view the API code behind
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
        print('Successfully cleared bootloader flash')

Factory reset
#############

In case you have soft-bricked your device, or just want to clear everything (flashed pipeline/assets and bootloader config),
we recommend using the :ref:`Device Manager`. Factory reset will also flash the latest bootloader.

.. include::  ../includes/footer-short.rst
