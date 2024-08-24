Demuxing Synchronized Script Outputs
====================================

This example demonstrates the use of the DepthAI Sync node in conjunction with the Demux node to synchronize and then demux outputs from two separate script nodes. Each script node generates data buffers at different intervals, which are first synchronized by the Sync node and then demultiplexed by the MessageDemux node.

.. rubric:: Similar samples:

- :ref:`Multiple Scripts Sync`
- :ref:`Depth and Video Sync`
- :ref:`IMU and Video Sync`

Demo
####


.. code-block::

    ~/depthai-python/examples/Sync $ python3 demux_message_group.py
    Start
    Buffer 1 timestamp: 0:00:03.581073
    Buffer 2 timestamp: 0:00:03.591084
    ----------
    Buffer 1 timestamp: 0:00:04.583100
    Buffer 2 timestamp: 0:00:04.497079
    ----------
    Buffer 1 timestamp: 0:00:06.587174
    Buffer 2 timestamp: 0:00:06.611154
    ----------
    Buffer 1 timestamp: 0:00:07.589147
    Buffer 2 timestamp: 0:00:07.517125
    ----------
    Buffer 1 timestamp: 0:00:09.593076
    Buffer 2 timestamp: 0:00:09.631089
    ----------
    Buffer 1 timestamp: 0:00:10.595106
    Buffer 2 timestamp: 0:00:10.537082


Setup
#####

.. include::  /includes/install_from_pypi.rst


Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/examples/demux_message_group.py>`__

        .. literalinclude:: ../../../../examples/Sync/demux_message_group.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/Sync/demux_message_group.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/Sync/demux_message_group.cpp
           :language: cpp
           :linenos:

How it Works
############

#. Initialize a DepthAI pipeline.
#. Create two Script nodes, with each script generating and sending data buffers at different intervals.
#. Set up a Sync node with a synchronization threshold.
#. Integrate a MessageDemux node to separate the synchronized data streams.
#. Link the outputs of the Script nodes to the Sync node, and then from the Sync node to the MessageDemux node.
#. Start the pipeline and continuously receive demultiplexed data from the MessageDemux node.
#. Print the timestamps of the demultiplexed data for comparison.


.. include::  /includes/footer-short.rst
