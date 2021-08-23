Script
======

Script node allows users to run **custom Python scripts on the device**. Due to the computational resource constraints,
script node shouldn't be used for heavy computing (eg. image manipulation/CV), but for managing the flow
of the pipeline. Example use cases would be controlling nodes like :ref:`ImageManip`, :ref:`ColorCamera`, :ref:`SpatialLocationCalculator`,
decoding :ref:`NeuralNetwork` results, or interfacing with GPIOs.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    script = pipeline.create(dai.node.Script)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto script = pipeline.create<dai::node::Script>();


Inputs and Outputs
##################

.. code-block::

  inputs[]  ┌──────────────┐  outputs[]
  ---------►│              ├-----------►
  ---------►│              ├-----------►
     ...    │    Script    |    ...
     ...    │              │    ...
  ---------►│              ├-----------►
            └──────────────┘

Users can define as many inputs and outputs as they need. Inputs and outputs can be any :ref:`Message <components_messages>` type.

Usage
#####

.. tabs::

  .. code-tab:: py

      script = pipeline.create(dai.node.Script)
      script.setScript("""
          import time
          import marshal
          num = 123
          node.warn(f"Number {num}") # Print to host
          x = [1, "Hello", {"Foo": "Bar"}]
          x_serial = marshal.dumps(x)
          b = Buffer(len(x_serial))
          while True:
              time.sleep(1)
              b.getData()[:] = x_serial
              node.io['out'].send(b)
      """)
      script.outputs['out'].link(xout.input)

      # ...
      # After initializing the device, enable log levels
      device.setLogLevel(dai.LogLevel.WARN)
      device.setLogOutputLevel(dai.LogLevel.WARN)

  .. code-tab:: c++

      auto script = pipeline.create<dai::node::Script>();
      script->setScript(R"(
          import time
          import marshal
          num = 123
          node.warn(f"Number {num}") # Print to host
          x = [1, "Hello", {"Foo": "Bar"}]
          x_serial = marshal.dumps(x)
          b = Buffer(len(x_serial))
          while True:
              time.sleep(1)
              b.getData()[:] = x_serial
              node.io['out'].send(b)
      )");
      script->outputs["out"].link(xout->input);

      // ...
      // After initializing the device, enable log levels
      device.setLogLevel(dai::LogLevel.WARN);
      device.setLogOutputLevel(dai::LogLevel.WARN);

Interfacing with GPIOs
######################

In the script node you can interface with GPIOs of the VPU. Currently supported functions are:

.. code-block:: python

  import GPIO # module
  GPIO.read(pin)
  GPIO.write(pin, value)
  GPIO.setPwm(pin, highCount, lowCount, repeat=0) # repeat == 0 means indefinite
  GPIO.enablePwm(pin, enable)

Using DepthAI :ref:`Messages <components_messages>`
###################################################

The depthai module is implicitly imported to the script node. You can create new
depthai messages and assign data to it, for example:

.. code-block:: python

  buf = Buffer(100) # Assign 100 bytes to the Buffer message

  # Create CameraControl message, set manual focus
  control = CameraControl()
  control.setManualFocus(100)

  imgFrame = ImgFrame(300*300*3) # Buffer with 300x300x3 bytes


Examples of functionality
#########################

- :ref:`Script camera control`
- `Triangulation experiment <https://github.com/luxonis/depthai-experiments/blob/master/gen2-triangulation/main.py>`__
- `Movenet decoding (edge mode) <https://github.com/geaxgx/depthai_movenet/blob/main/template_processing_script.py>`__ - A bit more complex example by geaxgx

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.Script
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::Script
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
