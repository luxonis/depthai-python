Demux
=====


The Demux (Demultiplexer) node is used to separate a `MessageGroup` into individual outputs. It currently serves as way to demultiplex the output of :ref:`Sync` node. 

How to Place It
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    demux = pipeline.create(dai.node.MessageDemux)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto demux = pipeline.create<dai::node::MessageDemux>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  input          │                   │
  ──────────────►│                   │
                 │       Demux       │        output1
                 │                   ├───────────►
                 │                   │        output2
                 │                   ├───────────►
                 │                   │        ...
                 └───────────────────┘

**Message types**

- :code:`input` - :ref:`MessageGroup`
- :code:`output1`, :code:`output2`, ... - Individual output messages

Usage
#####

The Demux node is particularly useful for handling different types of data coming from a single source. 
For example, when the :ref:`Sync` node is used to synchronize the outputs of multiple nodes, the output of the Sync node is a :ref:`MessageGroup` containing all the messages from the synchronized nodes. The Demux node can be used to separate the messages into individual streams.

.. tabs::

  .. code-tab:: py

      # Create sync node and set sync threshold
      sync = pipeline.create(dai.node.Sync)
      sync.setSyncThresholdMs(timedelta(milliseconds=100))

      # Create demux node
      demux = pipeline.create(dai.node.MessageDemux)

      # Sync the outputs of multiple nodes
      rgb.preview.link(sync.inputs["rgb"])
      stereo.depth.link(sync.inputs["depth"])
      script.outputs["out"].link(sync.inputs["script"])

      sync.out.link(demux.input) # Sync output is a MessageGroup containing all the messages from the synchronized nodes

      # Demux the MessageGroup into individual messages
      demux.outputs["rgb"].link(xout1.input)
      demux.outputs["depth"].link(xout2.input)
      demux.outputs["script"].link(xout3.input)




  .. code-tab:: c++

      // Create sync node and set sync threshold
      auto sync = pipeline.create<dai::node::Sync>();
      sync->setSyncThreshold(std::chrono::milliseconds(100));

      // Create demux node
      auto demux = pipeline.create<dai::node::MessageDemux>();

      // Sync the outputs of multiple nodes
      rgb.preview.link(sync->input["rgb"]);
      stereo.depth.link(sync->input["depth"]);
      script.outputs["out"].link(sync->input["script"]);

      sync->out.link(demux->input); // Sync output is a MessageGroup containing all the messages from the synchronized nodes

      // Demux the MessageGroup into individual messages
      demux->outputs["rgb"].link(xout1.input);
      demux->outputs["depth"].link(xout2.input);
      demux->outputs["script"].link(xout3.input);


Examples of Functionality
##########################

- :ref:`Demuxing Synchronized Script Outputs`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.MessageDemux
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++
      
    .. doxygenclass:: dai::node::MessageDemux
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
