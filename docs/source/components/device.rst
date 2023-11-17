.. _components_device:

Device
======

Device represents an `OAK camera <https://docs.luxonis.com/projects/hardware/en/latest/>`__. On all of our devices there's a powerful Robotics Vision Core
(`RVC <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#rvc2>`__). The RVC is optimized for performing AI inference algorithms and
for processing sensory inputs (eg. calculating stereo disparity from two cameras).

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


Watchdog
########

Understanding the Watchdog Mechanism in POE Devices
----------------------------------------------------

The watchdog is a crucial component in the operation of POE (Power over Ethernet) devices with DepthAI. When DepthAI disconnects from a POE device, the watchdog mechanism is the first to respond, initiating a reset of the camera. This reset is followed by a complete system reboot, which includes the loading of the DepthAI bootloader and the initialization of the entire networking stack. 

.. note::
    This process is necessary to make the camera available for reconnection and typically takes about 10 seconds, which means the fastest possible reconnection time is 10 seconds.
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

Code-Based Configuration
------------------------

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



Multiple devices
################

If you want to use multiple devices on a host, check :ref:`Multiple DepthAI per Host`.

Device queues
#############

After initializing the device, one has to initialize the input/output queues as well. These queues will be located on the host computer (in RAM).

.. code-block:: python

  outputQueue = device.getOutputQueue("output_name")
  inputQueue = device.getInputQueue("input_name")

When you define an output queue, the device can push new messages to it at any point in time, and the host can read from it at any point in time.
Usually, when the host is reading very fast from the queue, the queue (regardless of its size) will stay empty most of
the time. But as we add things on the host side (additional processing, analysis, etc), it may happen that the device will be writing to
the queue faster than the host can read from it. And then the messages in the queue will start to add up - and both maxSize and blocking
flags determine the behavior of the queue in this case. You can set these flags with:

.. code-block:: python

  # When initializing the queue
  queue = device.getOutputQueue(name="name", maxSize=5, blocking=False)

  # Or afterwards
  queue.setMaxSize(10)
  queue.setBlocking(True)

Specifying arguments for :code:`getOutputQueue` method
######################################################

When obtaining the output queue (example code below), the :code:`maxSize` and :code:`blocking` arguments should be set depending on how
the messages are intended to be used, where :code:`name` is the name of the outputting stream.

Since queues are on the host computer, memory (RAM) usually isn't that scarce. But if you are using a small SBC like RPI Zero, where there's only 0.5GB RAM,
you might need to specify max queue size as well.

.. code-block:: python

  with dai.Device(pipeline) as device:
    queueLeft = device.getOutputQueue(name="manip_left", maxSize=8, blocking=False)

If only the latest results are relevant and previous do not matter, one can set :code:`maxSize = 1` and :code:`blocking = False`.
That way only latest message will be kept (:code:`maxSize = 1`) and it might also be overwritten in order to avoid waiting for
the host to process every frame, thus providing only the latest data (:code:`blocking = False`).
However, if there are a lot of dropped/overwritten frames, because the host isn't able to process them fast enough
(eg. one-threaded environment which does some heavy computing), the :code:`maxSize` could be set to a higher
number, which would increase the queue size and reduce the number of dropped frames.
Specifically, at 30 FPS, a new frame is received every ~33ms, so if your host is able to process a frame in that time, the :code:`maxSize`
could be set to :code:`1`, otherwise to :code:`2` for processing times up to 66ms and so on.

If, however, there is a need to have some intervals of wait between retrieving messages, one could specify that differently.
An example would be checking the results of :code:`DetectionNetwork` for the last 1 second based on some other event,
in which case one could set :code:`maxSize = 30` and :code:`blocking = False`
(assuming :code:`DetectionNetwork` produces messages at ~30FPS).

The :code:`blocking = True` option is mostly used when correct order of messages is needed.
Two examples would be:

- matching passthrough frames and their original frames (eg. full 4K frames and smaller preview frames that went into NN),
- encoding (most prominently H264/H265 as frame drops can lead to artifacts).

Blocking behaviour
******************

By default, queues are **blocking** and their size is **30**, so when the device fills up a queue and when the limit is
reached, any additional messages from the device will be blocked and the library will wait until it can add new messages to the queue.
It will wait for the host to consume (eg. :code:`queue.get()`) a message before putting a new one into the queue.

.. note::
    After the host queue gets filled up, the XLinkOut.input queue on the device will start filling up. If that queue is
    set to blocking, other nodes that are sending messages to it will have to wait as well. This is a usual cause for a
    blocked pipeline, where one of the queues isn't emptied in timely manner and the rest of the pipeline waits for it
    to be empty again.

Non-Blocking behaviour
**********************
Making the queue non-blocking will change the behavior in the situation described above - instead of waiting, the library will discard
the oldest message and add the new one to the queue, and then continue its processing loop (so it won't get blocked).
:code:`maxSize` determines the size of the queue and it also helps to control memory usage.

For example, if a message has 5MB of data, and the queue size is 30, this queue can effectively store
up to 150MB of data in the memory on the host (the messages can also get really big, for instance, a single 4K NV12 encoded frame takes about ~12MB).

Some additional information
***************************

- Decreasing the queue size to 1 and setting non-blocking behavior will effectively mean "I only want the latest packet from the queue".
- Queues are thread-safe - they can be accessed from any thread.
- Queues are created such that each queue is its own thread which takes care of receiving, serializing/deserializing, and sending the messages forward (same for input/output queues).
- The :code:`Device` object isn't fully thread-safe. Some RPC calls (eg. :code:`getLogLevel`, :code:`setLogLevel`, :code:`getDdrMemoryUsage`) will get thread-safe once the mutex is set in place (right now there could be races).


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
