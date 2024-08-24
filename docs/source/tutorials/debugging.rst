Debugging DepthAI pipeline
##########################

Currently, tools for debugging the DepthAI pipeline are limited. We plan on creating a software that would track all messages and queues,
which would allow users to debug a "frozen" pipeline much easier, which is usually caused by a filled up :ref:`blocking queue <Device queues>`.

.. _depthai_logging:

DepthAI debugging level
=======================

You can enable debugging by changing the debugging level. It's set to :code:`warning` by default.

================  =======
Level             Logging
================  =======
:code:`critical`  Only a critical error that stops/crashes the program.
:code:`error`     Errors will not stop the program, but won't complete the action. Examples:
                    - When :ref:`ImageManip` cropping ROI was out of bounds, error will get printed and the cropping won't take place

                    - When :ref:`NeuralNetwork` gets a frame whose shape (width/heigth/channel) isn't that of the :code:`.blob`
:code:`warn`      Warnings are printed in cases where user action could improve certain behavior/fix it. Example:
                    - When API changes, the old API style will be deprecated and warning will be shown to the user.
:code:`info`      Will print information about CPU/RAM consumption, temperature, CMX slices and SHAVE core allocation.
:code:`debug`     Useful especially on starting and stopping the pipeline. Debug will print:
                    - Information about device initialization eg. Pipeline JSON, firmware/bootloader/OpenVINO version

                    - How device/XLink is being closed/disposed
:code:`trace`     Trace will print out a :ref:`Message <components_messages>` whenever one is received from the device.
================  =======

Debugging can be enabled either:

In code
*******

.. code-block:: python

    with dai.Device() as device: # Initialize device
        # Set debugging level
        device.setLogLevel(dai.LogLevel.DEBUG)
        device.setLogOutputLevel(dai.LogLevel.DEBUG)

Where :code:`setLogLevel` sets verbosity which filters messages that get sent from the device to the host and :code:`setLogOutputLevel` sets
verbosity which filters messages that get printed on the host (stdout). This difference allows to capture the log messages internally and
not print them to stdout, and use those to eg. display them somewhere else or analyze them.


Using an environmental variable `DEPTHAI_LEVEL`
***********************************************

Using an environment variable to set the debugging level, rather than configuring it directly in code, provides additional detailed information. 
This includes metrics such as CMX and SHAVE usage, and the time taken by each node in the pipeline to process a single frame.

Example of a log message for :ref:`RGB Preview` in **INFO** mode:

.. code-block:: bash

    [184430102189660F00] [2.1] [0.675] [system] [info] SIPP (Signal Image Processing Pipeline) internal buffer size '18432'B, DMA buffer size: '16384'B
    [184430102189660F00] [2.1] [0.711] [system] [info] ImageManip internal buffer size '285440'B, shave buffer size '34816'B
    [184430102189660F00] [2.1] [0.711] [system] [info] ColorCamera allocated resources: no shaves; cmx slices: [13-15] 
    ImageManip allocated resources: shaves: [15-15] no cmx slices. 


Example of a log message for :ref:`Depth Preview` in **TRACE** mode:

.. code-block:: bash 

    [19443010513F4D1300] [0.1.2] [2.014] [MonoCamera(0)] [trace] Mono ISP took '0.866377' ms.
    [19443010513F4D1300] [0.1.2] [2.016] [MonoCamera(1)] [trace] Mono ISP took '1.272838' ms.
    [19443010513F4D1300] [0.1.2] [2.019] [StereoDepth(2)] [trace] Stereo rectification took '2.661958' ms.
    [19443010513F4D1300] [0.1.2] [2.027] [StereoDepth(2)] [trace] Stereo took '7.144515' ms.
    [19443010513F4D1300] [0.1.2] [2.028] [StereoDepth(2)] [trace] 'Median' pipeline took '0.772257' ms.
    [19443010513F4D1300] [0.1.2] [2.028] [StereoDepth(2)] [trace] Stereo post processing (total) took '0.810216' ms.
    [2024-05-16 14:27:51.294] [depthai] [trace] Received message from device (disparity) - parsing time: 11µs, data size: 256000


.. tabs::

    .. tab:: Linux/MacOS

        .. code-block:: bash

            DEPTHAI_LEVEL=debug python3 script.py

    .. tab:: Windows PowerShell

        .. code-block:: bash

            $env:DEPTHAI_LEVEL='debug'
            python3 script.py

            # Turn debugging off afterwards
            Remove-Item Env:\DEPTHAI_LEVEL

    .. tab:: Windows CMD

        .. code-block:: bash

            set DEPTHAI_LEVEL=debug
            python3 script.py

            # Turn debugging off afterwards
            set DEPTHAI_LEVEL=

.. _script_logging:

Script node logging
===================

Currently, the best way to debug a behaviour inside the :ref:`Script` node, is to use :code:`node.warn('')` logging capability. This will
send the warning back to the host and it will get printed to the user. Users can also print values, such as frame sequence numbers, which
would be valuable when debugging on-device frame-syncing logic.

.. code-block:: python

    script = pipeline.create(dai.node.Script)
    script.setScript("""
        buf = NNData(13)
        buf.setLayer("fp16", [1.0, 1.2, 3.9, 5.5])
        buf.setLayer("uint8", [6, 9, 4, 2, 0])
        # Logging
        node.warn(f"Names of layers: {buf.getAllLayerNames()}")
        node.warn(f"Number of layers: {len(buf.getAllLayerNames())}")
        node.warn(f"FP16 values: {buf.getLayerFp16('fp16')}")
        node.warn(f"UINT8 values: {buf.getLayerUInt8('uint8')}")
    """)

Code above will print the following values to the user:

.. code-block:: bash

    [Script(0)] [warning] Names of layers: ['fp16', 'uint8']
    [Script(0)] [warning] Number of layers: 2
    [Script(0)] [warning] FP16 values: [1.2001953125, 1.2001953125, 3.900390625, 5.5]
    [Script(0)] [warning] UINT8 values: [6, 9, 4, 2, 0]

Resource Debugging
==================

.. warning::

    Resource debugging in only available when setting the debug level using environmental variable `DEPTHAI_LEVEL`. It's **not** available when setting the debug level in code.

By enabling ``info`` log level (or lower), depthai will print usage of `hardware resources <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#hardware-blocks-and-accelerators>`__,
specifically SHAVE core and CMX memory usage:

.. code-block:: bash

    NeuralNetwork allocated resources: shaves: [0-11] cmx slices: [0-11] # 12 SHAVES/CMXs allocated to NN
    ColorCamera allocated resources: no shaves; cmx slices: [13-15] # 3 CMXs allocated to Color an Mono cameras (ISP)
    MonoCamera allocated resources: no shaves; cmx slices: [13-15]
    StereoDepth allocated resources: shaves: [12-12] cmx slices: [12-12] # StereoDepth node consumes 1 CMX and 1 SHAVE core
    ImageManip allocated resources: shaves: [15-15] no cmx slices. # ImageManip node(s) consume 1 SHAVE core
    SpatialLocationCalculator allocated resources: shaves: [14-14] no cmx slices. # SLC consumes 1 SHAVE core

In total, this pipeline consumes 15 SHAVE cores and 16 CMX slices. The pipeline is running an object detection model compiled for 6 SHAVE cores.

CPU usage
=========

When setting the :ref:`DepthAI debugging level` to debug (or lower), depthai will also print our CPU usage for LeonOS and LeonRT. CPU usage
at 100% (or close to it) can cause many undesirable effects, such as higher frame latency, lower FPS, and in some cases even firmware crash.

Compared to OAK USB cameras, OAK PoE cameras will have increased CPU consumption, as the networking stack is running on the LeonOS core. Besides
reducing pipeline (doing less processing), a good alternative is to reduce 3A FPS (ISP). This means that 3A algorithms (auto exposure, auto white balance
and auto focus) won't be run every frame, but every N frames. When updating DepthAI SDK's `camera_preview.py <https://github.com/luxonis/depthai/blob/main/depthai_sdk/examples/CameraComponent/camera_preview.py>`__
example (code change below), the LeonOS CPU usage decreased from 100% to ~46%:

.. code-block:: bash

    # Without 3A FPS limit on OAK PoE camera:
    Cpu Usage - LeonOS 99.99%, LeonRT: 6.91%

    # Limiting 3A to 15 FPS on OAK PoE camera:
    Cpu Usage - LeonOS 46.24%, LeonRT: 3.90%

Not having 100% CPU usage also drastically decreased frame latency, in the example for the script below it went from ~710 ms to ~110ms:

.. image:: https://github.com/luxonis/depthai-python/assets/18037362/84ec8de8-58ce-49c7-b882-048141d284e0

.. code-block:: diff

    from depthai_sdk import OakCamera

    with OakCamera() as oak:
        color = oak.create_camera('color')
        left = oak.create_camera('left')
        right = oak.create_camera('right')

  +     # Limiting 3A to 15 FPS
  +     for node in [color.node, left.node, right.node]:
  +         node.setIsp3aFps(15)

        oak.visualize([color, left, right], fps=True, scale=2/3)
        oak.start(blocking=True)

Limiting 3A FPS can be achieved by calling :code:`setIsp3aFps()` function on the camera node (either :ref:`ColorCamera` or :ref:`MonoCamera`).

.. include::  /includes/footer-short.rst