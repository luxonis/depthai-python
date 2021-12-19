NNData
======

This message carries tensors and its data. You can receive this message from the :ref:`NeuralNetwork` node or you could create this message
on the host, populate the tensor with the data and send the message to the :code:`input` of the :ref:`NeuralNetwork` node.

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.NNData
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::NNData
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
