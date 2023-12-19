MessageGroup
============

The MessageGroup message type is a versatile container used in DepthAI pipelines to group together a map of arbitrary DepthAI messages. It serves as the primary output of the :ref:`Sync Node`, effectively synchronizing various input streams, and acts as the input to the :ref:`Demux Node` for subsequent disaggregation and processing. 

Creating MessageGroup
#####################

MessageGroup can be created automatically by the Sync node as it aligns and groups messages from different sources based on their timestamps. Alternatively, it can be manually constructed in a host application or within a :ref:`Script` node to create custom groupings of DepthAI messages.

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.MessageGroup
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::MessageGroup
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:


.. include::  ../../includes/footer-short.rst
