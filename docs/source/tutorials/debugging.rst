Debugging DepthAI pipeline
##########################

Currently, tools for debugging the DepthAI pipeline are limited. We plan on creating a software that would track all messages and queues,
which would allow users to debug a "frozen" pipeline much easier, which is usually caused by a filled up :ref:`blocking queue <Blocking behaviour>`.

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

Debugging can be enabled either **in code**:

.. code-block:: python

    with dai.Device() as device: # Initialize device
        # Set debugging level
        device.setLogLevel(dai.LogLevel.DEBUG)
        device.setLogOutputLevel(dai.LogLevel.DEBUG)

Where :code:`setLogLevel` sets verbosity which filters messages that get sent from the device to the host and :code:`setLogOutputLevel` sets
verbosity which filters messages that get printed on the host (stdout). This difference allows to capture the log messages internally and
not print them to stdout, and use those to eg. display them somewhere else or analyze them.

You can also enable debugging using an **environmental variable DEPTHAI_LEVEL**:

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

.. include::  /includes/footer-short.rst