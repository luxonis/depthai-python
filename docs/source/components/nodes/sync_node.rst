Sync Node
=========

The Sync node is used for synchronizing multiple input streams based on their timestamps. It outputs a grouped message containing synchronized frames from the input streams.

How to Use
##########

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    sync = pipeline.create(dai.node.Sync)

    # Configure threshold for timestamp alignment
    sync.setSyncThreshold(timedelta(milliseconds=50))

    # Configure inputs to be synchronized
    sync.inputs["input1"]
    sync.inputs["input2"]

    # ...

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto sync = pipeline.create<dai::node::Sync>();

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

The Sync node aligns incoming messages based on their timestamps. The synchronization criteria and behavior can be configured using the :code:`depthai.node.Sync.setSyncThreshold` method.

Limitations
###########

There are several limitations and considerations when using the Sync node:

- Timestamp precision and drift between different sources
- Handling of missed or delayed frames
- Behavior under different frame rate conditions

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
