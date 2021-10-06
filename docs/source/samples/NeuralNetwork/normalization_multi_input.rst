Multi-Input Frame Normalization
===============================

This example shows how you can normalize image before sending it to another NN. Most neural network models
require frames in values in range between :code:`-1.0` to :code:`1.0`. :ref:`ColorCamera`'s preview outputs
values between :code:`0` and :code:`255`. Simple custom NN, created with PyTorch (`link here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-custom-models/generate_model/pytorch_normalize.py>`__, `tutorial here <https://docs.luxonis.com/en/latest/pages/tutorials/creating-custom-nn-models/>`__),
allows users to specify multiplication factor and addent value for all frame values.

On the host, values are converted back to :code:`0`-:code:`255`, so they can be displayed by OpenCV.

.. note::
    This is just a demo, for normalization you should use OpenVINO's `model optimizer <https://docs.openvinotoolkit.org/latest/openvino_docs_MO_DG_prepare_model_convert_model_Converting_Model_General.html>`__ arguments :code:`--mean_values` and :code:`--scale_values`.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/NeuralNetwork/normalization_multiple_input.py>`__

        .. literalinclude:: ../../../../examples/NeuralNetwork/normalization_multi_input.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/normalization_multiple_input.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/src/normalization_multi_input.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
