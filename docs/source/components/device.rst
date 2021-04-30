.. _components_device:

Device
======

Device is a DepthAI `module <https://docs.luxonis.com/en/latest/pages/products/>`__. After the :ref:`Pipeline` is defined, it can be uploaded to the device.
When you create the device in the code, firmware is uploaded together with the pipeline.

.. code-block:: python

  pipeline = depthai.Pipeline()

  # Create nodes, configure them and link them together

  # Upload the pipeline to the device
  with depthai.Device(pipeline) as device:
    # Start the pipeline that is now on the device
    device.startPipeline()

    # Input queue, to send message from the host to the device (you can recieve the message on the device with XLinkIn)
    input_q = device.getInputQueue("input_name", maxSize=4, blocking=False)

    # Output queue, to recieve message on the host from the device (you can send the message on the device with XLinkOut)
    output_q = device.getOutputQueue("output_name", maxSize=4, blocking=False)

    while True:
        # Get the message from the queue
        output_q.get() # Or output_q.tryGet() for non-blocking

        # Send a message to the device
        cfg = depthai.ImageManipConfig()
        input_q.send(cfg)


Multiple devices
################

If you want to use multiple devices on a host, check :ref:`Multiple DepthAI per Host`.

Device queues
#############

After initializing the device, one has to initialize the input/output queues as well. You can create an input queue with
:code:`device.getInputQueue("input_name")` and output queue with :code:`device.getOutputQueue("output_name")`.


When you define an output queue, the device can write to it at any point in time, and the host can read from it at any point in time.
There might be a cases when the host is reading very fast from the queue, and the queue, no matter its size, will stay empty most of
the time. But as we add things on the host side (additional processing, analysis etc), it may happen that the device will be writing to
the queue faster than the host can read from it. And then the packets in the queue will start to add up - and both maxSize and blocking
flags determine the behavior of the queue in this case.

By default, the queue is blocking and its size is 30, so the device will put
30 packets at most, and when the limit is reached, it will hang on queue put call and wait until it can successfully complete this
call (so, waits for the host to consume the packet before putting a new one). Making the queue non-blocking will change its behavior
in this situation - instead of waiting, it will discard the oldest packet and add the new one, and then continue it's processing
loop (so it won't get blocked). maxSize determines the size of the queue and also
helps to control the memory usage - if the packet have 5MB of data, and the queue size is 30, this queue effectively stores
150MB of data in memory (the packets can also get really big, for instance a single 4K NV12 encoded frame takes about ~12MB).
Decreasing the queue size to 1 and setting non-blocking behavior will effectively mean "I only want the latest packet from the queue".

The size and behavior of the queue can be modified after the initialization by calling :code:`queue.setBlocking()` and :code:`queue.setMaxSize()`.

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.Device
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::Device
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../includes/footer-short.rst
