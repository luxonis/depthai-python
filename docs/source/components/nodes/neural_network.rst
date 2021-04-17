NeuralNetwork
=============

Runs a neural inference on input data. Neural network has to be a :code:`.blob` type. Instructions on how to compile your neural network (NN) to :code:`.blob`
can be found at :ref:`Local OpenVINO Model Conversion`.


How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    nn = pipeline.createNeuralNetwork()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto nn = pipeline.create<dai::node::NeuralNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       Out
              │                   ├───────────►
              │                   │
              │   NeuralNetwork   │
  Input       │                   │ Passthrough
  ───────────►│-------------------├───────────►
              │                   │
              └───────────────────┘

Message types
#############

- :code:`Input` - :ref:`ImgFrame`
- :code:`Out` - :ref:`NNData`
- :code:`Passthrough` - :ref:`ImgFrame`

Passthrough mechanism
#####################

The passthrough mechanism is very useful when a node specifies its input to be non-blocking, where messages can be overwritten.
There we don't know on which message the node performed its operation (eg NN, was inference done on frame 25 or skipped 25 and performed
inference on 26). At the same time means that if: xlink and host input queues are blocking, and we receive both say passthrough and
output we can do a blocking get on both of those queues and be sure to always get matching frames. They might not arrive at the same time,
but both of them will arrive, and be in queue in correct spot to be taken out together.


Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    nn = pipeline.createNeuralNetwork()
    nn.setBlobPath(model_path_absolute)
    cam.out.link(nn.input)

    # Send NN out to the host via XLink
    nn_xout = pipeline.createXLinkOut()
    nn_xout.setStreamName("nn")
    nn.out.link(nn_xout.input)

    with dai.Device(pipeline) as device:
      q_nn = device.getOutputQueue("nn")
      # You have to decode the output of the NN

      nn_data = q_nn.get() # Blocking

      # NN can output from multiple layers. Print all layer names:
      print(nn_data.getAllLayerNames())

      # Get layer named "Layer1_FP16" as FP16
      layer1_data = nn_data.getLayerFp16("Layer1_FP16")

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto nn = pipeline.create<dai::node::NeuralNetwork>();

Examples of functionality
#########################

- :ref:``

Reference
#########

.. autoclass:: depthai.NeuralNetwork
  :members:
  :inherited-members:

.. include::  ../../includes/footer-short.rst
