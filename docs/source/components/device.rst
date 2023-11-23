.. _components_device:

Device
======

Device is an `OAK camera <https://docs.luxonis.com/projects/hardware/en/latest/>`__ or a RAE robot. On all of our devices there's a powerful Robotics Vision Core
(`RVC <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#rvc2>`__). The RVC is optimized for performing AI inference, CV operations, and
for processing sensory inputs (eg. stereo depth, video encoders, etc.).

Device API
##########

:code:`Device` object represents an OAK device. When starting the device, you have to upload a :ref:`Pipeline` to it, which will get executed on the VPU.
When you create the device in the code, firmware is uploaded together with the pipeline and other assets (such as NN blobs).

.. code-block:: python

  pipeline = depthai.Pipeline()

  # Create nodes, configure them and link them together

  # Upload the pipeline to the device
  with depthai.Device(pipeline) as device:
    # Print MxID, USB speed, and available cameras on the device
    print('MxId:',device.getDeviceInfo().getMxId())
    print('USB speed:',device.getUsbSpeed())
    print('Connected cameras:',device.getConnectedCameras())

    # Input queue, to send message from the host to the device (you can receive the message on the device with XLinkIn)
    input_q = device.getInputQueue("input_name", maxSize=4, blocking=False)

    # Output queue, to receive message on the host from the device (you can send the message on the device with XLinkOut)
    output_q = device.getOutputQueue("output_name", maxSize=4, blocking=False)

    while True:
        # Get a message that came from the queue
        output_q.get() # Or output_q.tryGet() for non-blocking

        # Send a message to the device
        cfg = depthai.ImageManipConfig()
        input_q.send(cfg)

Connect to specified device
###########################

If you have multiple devices and only want to connect to a specific one, or if your OAK PoE camera is outside of your
subnet, you can specify the device (either with MxID, IP, or USB port name) you want to connect to.

.. code-block:: python

  # Specify MXID, IP Address or USB path
  device_info = depthai.DeviceInfo("14442C108144F1D000") # MXID
  #device_info = depthai.DeviceInfo("192.168.1.44") # IP Address
  #device_info = depthai.DeviceInfo("3.3.3") # USB port name
  with depthai.Device(pipeline, device_info) as device:
      # ...

Host clock syncing
==================

When depthai library connects to a device, it automatically syncs device's timestamp to host's timestamp. Timestamp syncing happens continuously at around 5 second intervals,
and can be configured via API (example script below).

.. image:: /_static/images/components/device_timesync.jpg

Device clocks are synced at below 2.5ms accuracy for PoE cameras, and below 1ms accuracy for USB cameras at 1σ (standard deviation) with host clock.

.. image:: /_static/images/components/clock-syncing.png

A graph representing the accuracy of the device clock with respect to the host clock. We had 3 devices connected (OAK PoE cameras), all were hardware synchronized using `FSYNC Y-adapter <https://docs.luxonis.com/projects/hardware/en/latest/pages/FSYNC_Yadapter/>`__.
Raspberry Pi (the host) had an interrupt pin connected to the FSYNC line, so at the start of each frame the interrupt happened and the host clock was recorded. Then we compared frame (synced) timestamps with
host timestamps and computed the standard deviation. For the histogram above we ran this test for about 7 hours.

.. code-block:: python

    # Configure host clock syncing exmaple

    import depthai as dai
    from datetime import timedelta
    # Configure pipeline
    with dai.Device(pipeline) as device:
        # 1st value: Interval between timesync runs
        # 2nd value: Number of timesync samples per run which are used to compute a better value
        # 3rd value: If true partial timesync requests will be performed at random intervals, otherwise at fixed intervals
        device.setTimesync(timedelta(seconds=5), 10, True) # (These are default values)


Multiple devices
################

If you want to use multiple devices on a host, check :ref:`Multiple DepthAI per Host`.

Device queues
#############

After initializing the device, you can create input/output queues that match :ref:`XLinkIn`/:ref:`XLinkOut` nodes in the pipeline. These queues will be located on the host computer (in RAM).

.. code-block:: python

  pipeline = dai.Pipeline()

  xout = pipeline.createXLinkOut()
  xout.setStreamName("output_name")
  # ...
  xin = pipeline.createXLinkIn()
  xin.setStreamName("input_name")
  # ...
  with dai.Device(pipeline) as device:

    outputQueue = device.getOutputQueue("output_name", maxSize=5, blocking=False)
    inputQueue = device.getInputQueue("input_name")

    outputQueue.get() # Read from the queue, blocks until message arrives
    outputQueue.tryGet() # Read from the queue, returns None if there's no msg (doesn't block)
    if outputQueue.has(): # Check if there are any messages in the queue


When you define an output queue, the device can push new messages to it at any time, and the host can read from it at any time.


Output queue - `maxSize` and `blocking`
#######################################

When the host is reading very fast from the queue (inside `while True` loop), the queue, regardless of its size, will stay empty most of
the time. But as we add things on the host side (additional processing, analysis, etc), it may happen that the device will be pushing messages to
the queue faster than the host can read from it. And then the messages in the queue will start to increase - and both `maxSize` and `blocking`
flags determine the behavior of the queue in this case. Two common configurations are:

.. code-block:: python

  with dai.Device(pipeline) as device:
    # If you want only the latest message, and don't care about previous ones;
    # When a new msg arrives to the host, it will overwrite the previous (oldest) one if it's still in the queue
    q1 = device.getOutputQueue(name="name1", maxSize=1, blocking=False)


    # If you care about every single message (eg. H264/5 encoded video; if you miss a frame, you will get artifacts);
    # If the queue is full, the device will wait until the host reads a message from the queue
    q2 = device.getOutputQueue(name="name2", maxSize=30, blocking=True) # Also default values (maxSize=30/blocking=True)

We used `maxSize=30` just as an example, but it can be any `int16` number. Since device queues are on the host computer, memory (RAM) usually isn't that scarce, so `maxSize` wouldn't matter that much.
But if you are using a small SBC like RPI Zero (512MB RAM), and are streaming large frames (eg. 4K unencoded), you could quickly run out of memory if you set `maxSize` to a high
value (and don't read from the queue fast enough).

Some additional information
---------------------------

- Queues are thread-safe - they can be accessed from any thread.
- Queues are created such that each queue is its own thread which takes care of receiving, serializing/deserializing, and sending the messages forward (same for input/output queues).
- The :code:`Device` object isn't fully thread-safe. Some RPC calls (eg. :code:`getLogLevel`, :code:`setLogLevel`, :code:`getDdrMemoryUsage`) will get thread-safe once the mutex is set in place (right now there could be races).

Watchdog
########

The watchdog is a crucial component in the operation of POE (Power over Ethernet) devices with DepthAI. When DepthAI disconnects from a POE device, the watchdog mechanism is the first to respond,
initiating a reset of the camera. This reset is followed by a complete system reboot, which includes the loading of the DepthAI bootloader and the initialization of the entire networking stack.

The watchdog process is necessary to make the camera available for reconnection and **typically takes about 10 seconds**, which means the fastest possible reconnection time is 10 seconds.


Customizing the Watchdog Timeout
--------------------------------

.. tabs::

  .. tab:: **Linux/MacOS**

    Set the environment variables `DEPTHAI_WATCHDOG_INITIAL_DELAY` and `DEPTHAI_BOOTUP_TIMEOUT` to your desired timeout values (in milliseconds) as follows:

    .. code-block:: bash

      DEPTHAI_WATCHDOG_INITIAL_DELAY=<my_value> DEPTHAI_BOOTUP_TIMEOUT=<my_value> python3 script.py

  .. tab:: **Windows PowerShell**

    For Windows PowerShell, set the environment variables like this:

    .. code-block:: powershell

      $env:DEPTHAI_WATCHDOG_INITIAL_DELAY=<my_value>
      $env:DEPTHAI_BOOTUP_TIMEOUT=<my_value>
      python3 script.py

  .. tab:: **Windows CMD**

    In Windows CMD, you can set the environment variables as follows:

    .. code-block:: guess

      set DEPTHAI_WATCHDOG_INITIAL_DELAY=<my_value>
      set DEPTHAI_BOOTUP_TIMEOUT=<my_value>
      python3 script.py

Alternatively, you can set the timeout directly in your code:

.. code-block:: python

  pipeline = depthai.Pipeline()

  # Create a BoardConfig object
  config = depthai.BoardConfig()

  # Set the parameters
  config.watchdogInitialDelayMs = <my_value>
  config.watchdogTimeoutMs = <my_value>

  pipeline.setBoardConfig(config)

By adjusting these settings, you can tailor the watchdog functionality to better suit your specific requirements.


Environment Variables
#####################

The following table lists various environment variables used in the system, along with their descriptions:

.. list-table::
   :widths: 50 50
   :header-rows: 1

   * - Environment Variable
     - Description
   * - `DEPTHAI_LEVEL`
     - Sets logging verbosity, options: 'trace', 'debug', 'warn', 'error', 'off'
   * - `XLINK_LEVEL`
     - Sets logging verbosity of XLink library, options: 'debug', 'info', 'warn', 'error', 'fatal', 'off'
   * - `DEPTHAI_INSTALL_SIGNAL_HANDLER`
     - Set to 0 to disable installing Backward signal handler for stack trace printing
   * - `DEPTHAI_WATCHDOG`
     - Sets device watchdog timeout. Useful for debugging (DEPTHAI_WATCHDOG=0), to prevent device reset while the process is paused.
   * - `DEPTHAI_WATCHDOG_INITIAL_DELAY`
     - Specifies delay after which the device watchdog starts.
   * - `DEPTHAI_SEARCH_TIMEOUT`
     - Specifies timeout in milliseconds for device searching in blocking functions.
   * - `DEPTHAI_CONNECT_TIMEOUT`
     - Specifies timeout in milliseconds for establishing a connection to a given device.
   * - `DEPTHAI_BOOTUP_TIMEOUT`
     - Specifies timeout in milliseconds for waiting the device to boot after sending the binary.
   * - `DEPTHAI_PROTOCOL`
     - Restricts default search to the specified protocol. Options: any, usb, tcpip.
   * - `DEPTHAI_DEVICE_MXID_LIST`
     - Restricts default search to the specified MXIDs. Accepts comma separated list of MXIDs. Lists filter results in an "AND" manner and not "OR"
   * - `DEPTHAI_DEVICE_ID_LIST`
     - Alias to MXID list. Lists filter results in an "AND" manner and not "OR"
   * - `DEPTHAI_DEVICE_NAME_LIST`
     - Restricts default search to the specified NAMEs. Accepts comma separated list of NAMEs. Lists filter results in an "AND" manner and not "OR"
   * - `DEPTHAI_DEVICE_BINARY`
     - Overrides device Firmware binary. Mostly for internal debugging purposes.
   * - `DEPTHAI_BOOTLOADER_BINARY_USB`
     - Overrides device USB Bootloader binary. Mostly for internal debugging purposes.
   * - `DEPTHAI_BOOTLOADER_BINARY_ETH`
     - Overrides device Network Bootloader binary. Mostly for internal debugging purposes.

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.Device
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::Device
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../includes/footer-short.rst
