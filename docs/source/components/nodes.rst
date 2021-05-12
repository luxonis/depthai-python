.. _components_nodes:

Nodes
=====

Nodes are the building blocks when populating the :ref:`Pipeline`. Each node provides a specific functionality on the DepthaI, a set of configurable
properties and inputs/outputs. After you create a node on a pipeline, you can also configure it as desired and link it to other nodes.

.. rubric:: Outputs and inputs

Each node can have zero, one or multiple inputs and outputs. For example :ref:`SystemLogger` node has no inputs and 1 output and :ref:`StereoDepth` has
2 inputs and 6 outputs.

.. rubric:: Node input

Node input queue is a queue for the :ref:`Messages`. It can be linked with other node's output (that's how you link up nodes). Node inputs are
configurable - with :code:`input.setBlocking(bool)` and :code:`input.setQueueSize(num)`. Default behaviour is blocking and a queue size of 8 messages.
If the input queue fills up, behavior of the input depends on blocking attribute.
If blocking is enabled, new messages will be discarded until user gets the old messages. If blocking is disabled, new messages will push out old messages.

.. rubric:: Node output

Node outputs :ref:`Messages`. There is no output queue per se, but some nodes do have a configurable output message pool.
Output message pool is a reserved memory region (to reduce memory fragmentation) that holds output messages.
After the node creates an output message (for example :ref:`ImgFrame`), it will send it to other nodes as specified when linking the inputs/outputs of the node.
Currently, some nodes (:ref:`VideoEncoder`, :ref:`NeuralNetwork`, :ref:`ImageManip`, :ref:`XLinkIn`) can have the pool size configured.
The size of the pool specifies how many messages can be created and sent out while other messages are already
somewhere in the pipeline. When all the messages from pool are sent out and none yet returned, that is when the node will block and
wait until a message is returned to the pool (not used by any node in the pipeline anymore)

On the table of contents (left side of the page) all nodes are listed under the :code:`Node` entry. You can click on them to find out more.

.. toctree::
   :maxdepth: 0
   :hidden:
   :glob:

   nodes/*

.. include::  ../includes/footer-short.rst
