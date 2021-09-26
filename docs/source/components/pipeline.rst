.. _components_pipeline:

Pipeline
========

Pipeline is a collection of :ref:`nodes <Nodes>` and links between them. This flow provides extensive flexibility that users get for their
DepthAI device.


Pipeline first steps
####################

To get DepthAI up and running, one has to define a pipeline, populate it with nodes, configure the nodes and link them together. After that, the pipeline
can be loaded onto the :ref:`Device` and be started.

.. code-block:: python

  pipeline = depthai.Pipeline()

  # Create nodes, configure them and link them together

  # Upload the pipeline to the device
  with depthai.Device(pipeline) as device:
    # Start the pipeline that is now on the device
    device.startPipeline()

    # Set input/output queues to configure device/host communication through the XLink...

Using multiple devices
######################

If user has multiple DepthAI devices, each device can run a separate pipeline or the same pipeline
(`demo here <https://github.com/luxonis/depthai-experiments/tree/master/gen2-multiple-devices>`__). To use different pipeline for each device,
you can create multiple pipelines and pass the desired pipeline to the desired device on initialization.

Specifying OpenVINO version
###########################

When using a NN blob that was not compiled with the latest OpenVINO (that DepthAI supports), you have to specify the OpenVINO version of the pipeline.
The reason behind this is that OpenVINO doesn't provide version inside the blob.

.. code-block:: python

  pipeline = depthai.Pipeline()
  # Set the correct version:
  pipeline.setOpenVINOVersion(depthai.OpenVINO.Version.VERSION_2021_4)

Specifying arguments for :code:`getOutputQueue` method
######################################################

When obtaining the output queue (example code below), the :code:`maxSize` and :code:`blocking` arguments should be set depending on how
the messages are intended to be used, where :code:`name` is the name of the outputting stream.

.. code-block:: python

  with dai.Device(pipeline) as device:
    queueLeft = device.getOutputQueue(name="manip_left", maxSize=8, blocking=False)

If only the latest results are relevant and previous do not matter, one can set :code:`maxSize = 1` and :code:`blocking = False`.
That way only latest message will be kept (:code:`maxSize = 1`) and it might also be overwritten in order to avoid waiting for
the host to process every frame, thus providing only the latest data (:code:`blocking = False`).
However, if there are a lot of dropped/overwritten frames, because the host isn't able to process them fast enough 
(eg. one-threaded environment which does some heavy computing), the :code:`maxSize` could be set to a higher
number, which would increase the queue size and reduce the number of dropped frames.
Specifically, at 30 FPS, a new frame is recieved every ~33ms, so if your host is able to process a frame in that time, the :code:`maxSize`
should be set to :code:`1`, otherwise to :code:`2` for processing times up to 66ms and so on.

If, however, there is a need to have some intervals of wait between retrieving messages, one could specify that differently.
An example would be checking the results of :code:`DetectionNetwork` for the last 1 second based on some other event,
in which case one could set :code:`maxSize = 30` and :code:`blocking = False`
(assuming :code:`DetectionNetwork` produces messages at ~30FPS).

The :code:`blocking = True` option is mostly used when correct order of messages is needed.
Two examples would be:

- matching passthrough frames and their original frames (eg. full 4K frames and smaller preview frames that went into NN),
- encoding (most prominently H264/H265 as frame drops can lead to artifacts).

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()

  .. code-tab:: c++

    dai::Pipeline pipeline;

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.Pipeline
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::Pipeline
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../includes/footer-short.rst
