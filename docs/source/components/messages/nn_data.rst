NNData
======

This message carries tensors and its data. :ref:`NeuralNetwork` node outputs NNData.

Creating NNData
###############

You can also create this message on the host (`example here <https://github.com/luxonis/depthai-experiments/blob/cd4a1e2be64a7c0319870cf80b4be3bd0561e9a0/gen2-efficientnet-classification/main.py#L125-L127>`__)
or :ref:`Script` node (:ref:`example here <Script NNData example>`), populate the tensor with the data,
and send the message to the ``input`` of the :ref:`NeuralNetwork` node.

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
