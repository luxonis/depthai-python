NeuralNetwork
=============

This node runs neural inference on input data. Any OpenVINO neural networks can be run using this node, as long as the VPU `supports all layers <https://docs.luxonis.com/en/latest/pages/model_conversion/#supported-layers>`__.
This allows you to pick from **200+ pre-trained model** from `Open Model Zoo <https://github.com/openvinotoolkit/open_model_zoo>`__ and `DepthAI Model Zoo <https://github.com/luxonis/depthai-model-zoo>`__
and directly run it on the OAK device.

Neural network has to be in :code:`.blob` format to be compatible with the VPU. Instructions on how to compile
your neural network (NN) to :code:`.blob` can be found `here <https://docs.luxonis.com/en/latest/pages/model_conversion/>`__.


How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    nn = pipeline.create(dai.node.NeuralNetwork)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto nn = pipeline.create<dai::node::NeuralNetwork>();


Inputs and Outputs
##################

.. code-block::

              ┌───────────────────┐
              │                   │       out
              │                   ├───────────►
              │                   │
              │   NeuralNetwork   │
  input       │                   │ passthrough
  ───────────►│-------------------├───────────►
              │                   │
              └───────────────────┘

**Message types**

- :code:`input` - Any message type
- :code:`out` - :ref:`NNData`
- :code:`passthrough` - :ref:`ImgFrame`

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
    nn = pipeline.create(dai.node.NeuralNetwork)
    nn.setBlobPath(bbBlobPath)
    cam.out.link(nn.input)

    # Send NN out to the host via XLink
    nnXout = pipeline.create(dai.node.XLinkOut)
    nnXout.setStreamName("nn")
    nn.out.link(nnXout.input)

    with dai.Device(pipeline) as device:
      qNn = device.getOutputQueue("nn")

      nnData = qNn.get() # Blocking

      # NN can output from multiple layers. Print all layer names:
      print(nnData.getAllLayerNames())

      # Get layer named "Layer1_FP16" as FP16
      layer1Data = nnData.getLayerFp16("Layer1_FP16")

      # You can now decode the output of your NN

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto nn = pipeline.create<dai::node::NeuralNetwork>();
    nn->setBlobPath(bbBlobPath);
    cam->out.link(nn->input);

    // Send NN out to the host via XLink
    auto nnXout = pipeline.create<dai::node::XLinkOut>();
    nnXout->setStreamName("nn");
    nn->out.link(nnXout->input);

    dai::Device device(pipeline);
    // Start the pipeline
    device.startPipeline();

    auto qNn = device.getOutputQueue("nn");

    auto nnData = qNn->get<dai::NNData>(); // Blocking

    // NN can output from multiple layers. Print all layer names:
    cout << nnData->getAllLayerNames();

    // Get layer named "Layer1_FP16" as FP16
    auto layer1Data = nnData->getLayerFp16("Layer1_FP16");

    // You can now decode the output of your NN

Examples of functionality
#########################

- :ref:`Multi-Input Frame Concationation`
- :ref:`Frame Normalization`
- `DeeplabV3 experiment <https://github.com/luxonis/depthai-experiments/tree/master/gen2-deeplabv3_depth>`__
- `Age/gender experiment <https://github.com/luxonis/depthai-experiments/blob/master/gen2-age-gender/main.py>`__
- `EfficientDet demo <https://github.com/luxonis/depthai-experiments/blob/master/gen2-efficientDet/main.py>`__

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.NeuralNetwork
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::NeuralNetwork
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
