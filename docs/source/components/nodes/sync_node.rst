Sync
====

The Sync node is used for synchronizing multiple input streams based on their timestamps. It outputs a grouped message containing synchronized frames from the input streams.
The output message is a :ref:`MessageGroup` containing synchronized messages from all the input streams. These can be demultiplexed using the :ref:`Demux` node.

How to Use
##########

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    sync = pipeline.create(dai.node.MessageDemux)

    # Configure threshold for timestamp alignment
    sync.setSyncThreshold(timedelta(milliseconds=50))

    # Configure inputs to be synchronized
    sync.inputs["input1"]
    sync.inputs["input2"]

    # ...

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto sync = pipeline.create<dai::node::MessageDemux>();

    // Configure threshold for timestamp alignment
    sync->setSyncThreshold(std::chrono::milliseconds(50));

    // Configure inputs to be synchronized
    sync->inputs["input1"];
    sync->inputs["input2"];

    // ...


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  input1         │                   │
  ──────────────►│                   │
  input2         │                   │        out
  ──────────────►│      Sync         ├───────────►
                 │                   │
  ...            │                   │
  ──────────────►│                   │
                 └───────────────────┘

**Message types**

- :code:`input1`, :code:`input2`, ... - any message type from :ref:`Messages`
- :code:`out` - :ref:`MessageGroup`


Message Synchronization
########################

The Sync node aligns incoming messages based on their timestamps. The synchronization criteria and behavior can be configured using the :code:`depthai.node.Sync.setSyncThreshold` and :code:`depthai.node.Sync.setSyncAttempts`  method. More info in the :ref:`API Reference <reference>`.


Examples of Functionality
##########################

- :ref:`Depth and Video Sync`
- :ref:`Multiple Scripts Sync`
- :ref:`IMU and Video Sync`
- :ref:`Demuxing Synchronized Script Outputs`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.Sync
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::Sync
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
