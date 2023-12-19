Multiple Scripts Sync
=====================

This example illustrates the use of the DepthAI Sync node to synchronize outputs from two separate script nodes. Each script generates and sends data buffers at different intervals, and the Sync node aligns these outputs based on their timestamps.

.. rubric:: Similar samples:

- :ref:`Depth and Video Sync`
- :ref:`IMU and Video Sync`

Demo
####

TODO: Add demo

Setup
#####

.. include::  /includes/install_from_pypi.rst

Synchronized Script Outputs
###########################

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/examples/sync_scripts.py>`__

        .. literalinclude:: ../../../../depthai-python/examples/sync_scripts.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/Sync/sync_scripts.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/Sync/sync_scripts.cpp
           :language: cpp
           :linenos:


How it Works
############

#. Initialize a DepthAI pipeline.
#. Create two Script nodes, each generating and sending data buffers at different intervals.
#. Set up a Sync node with a synchronization threshold.
#. Link the outputs of the Script nodes to the Sync node.
#. Start the pipeline and continuously receive synchronized data from the Script nodes.
#. Print the received data along with timestamps and the interval between messages.


.. include::  /includes/footer-short.rst
