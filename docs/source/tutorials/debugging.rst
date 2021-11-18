Debugging DepthAI pipeline
##########################

Currently, tools for debugging the DepthAI pipeline are limited. We plan on creating a software that would track all messages and queues,
which would allow users to debug a "frozen" pipeline much easier, which is usually caused by a filled up :ref:`blocking queue <Blocking behaviour>`.

DepthAI debugging level
=======================

You can enable debugging by changing the debugging level. It's set to :code:`error` by default.

================  =======
Level             Logging
================  =======
:code:`critical`  Only a critical error that stops/crashes the program.
:code:`error`     Errors will not stop the program, but won't complete the action. Examples:
                    - When :ref:`ImageManip` cropping ROI was out of bounds, error will get printed and the cropping won't take place

                    - When :ref:`NeuralNetwork` gets a frame whose shape (width/heigth/channel) isn't that of the :code:`.blob`
:code:`warn`      Warnings (add example here)
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

Or using **environmental variable DEPTHAI_LEVEL**:

.. tabs::

    .. tab:: Linux/MacOS

        .. code-block:: bash

            DEPTHAI_LEVEL=debug python3 script.py

    .. tab:: Windows PowerShell

        .. code-block:: bash

            $env:DEPTHAI_LEVEL='debug'
            python3 script.py

            # Turn debugging off afterwards
            $env:DEPTHAI_LEVEL='off'

    .. tab:: Windows CMD

        .. code-block:: bash

            set DEPTHAI_LEVEL=debug
            python3 script.py

            # Turn debugging off afterwards
            set DEPTHAI_LEVEL=off