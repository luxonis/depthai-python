NNData
======

This message carries tensors and its data. You can recieve this message from the :ref:`NeuralNetwork` node or you could create this message
on the host, populate the tensor with the data and send the message to the :code:`input` of the :ref:`NeuralNetwork` node.

Creating NNData message
#######################

If you would like to run inference on a model that has multiple inputs, you would currently need to create a :code:`NNData` message (either on the host or in the :ref:`Script` node), populate it and send it to the :ref:`NeuralNetwork` node. In the case below, our model has two inputs, one named "input_layer_float" and second named "input_layer_int8".

.. code-block:: python

    nndata = NNData(150)
    nndata.setLayer("input_layer_float", [1.0, 1.2, 3.9, 5.5])
    nndata.setLayer("input_layer_int8", [6, 9, 4, 2, 0])

    # Send the message to the NN node

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
