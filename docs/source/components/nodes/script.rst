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

Available modules and libraries
###############################

- **Modules** available on **LEON_CSS and LEON_MSS**

.. code-block::

  "posix", "errno", "pwd", "_sre", "_codecs", "_weakref", "_functools", "_operator",
  "_collections", "_abc", "itertools", "atexit", "_stat", "time", "_datetime", "math",
  "_thread", "_io", "_symtable", "marshal", "_ast", "gc", "_warnings", "_string", "_struct"

- **Modules** available on **LEON_CSS**

.. code-block::

  "binascii", "_random", "_socket", "_md5", "_sha1", "_sha256", "_sha512", "select",
  "array", "unicodedata"

- **Libraries** available on **LEON_CSS and LEON_MSS**

.. code-block::

  "__main__", "_collections_abc", "_frozen_importlib", "_frozen_importlib_external",
  "_sitebuiltins", "abc", "codecs", "datetime", "encodings", "encodings.aliases",
  "encodings.ascii", "encodings.latin_1", "encodings.mbcs", "encodings.utf_8", "genericpath",
  "io", "os", "posixpath", "site", "stat", "threading", "types", "struct", "copyreg",
  "reprlib", "operator", "keyword", "heapq", "collections", "functools", "sre_constants",
  "sre_parse", "sre_compile", "enum", "re", "json", "json.decoder", "json.encoder",
  "json.scanner", "textwrap"

- **Libraries** available on **LEON_CSS**

.. code-block::

  "http", "http.client", "http.server", "html", "mimetypes", "copy", "shutil", "fnmatch",
  "socketserver", "contextlib", "email", "email._encoded_words", "email._header_value_parser",
  "email._parseaddr", "email._policybase", "email.base64mime", "email.charset",
  "email.contentmanager",  "email.encoders", "email.errors", "email.feedparser",
  "email.generator", "email.header", "email.headerregistry", "email.iterators", "email.message",
  "email.parser", "email.policy", "email.quoprimime", "email.utils", "string", "base64",
  "quopri", "random", "warnings", "bisect", "hashlib", "logging", "traceback", "linecache",
  "socket", "token", "tokenize", "weakref", "_weakrefset", "collections.abc", "selectors",
  "urllib", "urllib.parse", "calendar", "locale", "uu", "encodings.idna", "stringprep"

The **difference between module and library** is that module is a precompiled C source with Python bindings, whereas library is Python source code 
packed into a library and precompiled into Python bytecode (before loaded into our Firmware).

Networking/protocol modules/libraries that are available on the LEON_CSS can be used on any `OAK POE device <https://docs.luxonis.com/projects/hardware/en/latest/#poe-designs>`__.
You can specify on which processor the script will run, eg. for LEON_CSS:

.. code-block:: py

  script = pipeline.create(dai.node.Script)
  script.setProcessor(dai.ProcessorType.LEON_CSS)

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
