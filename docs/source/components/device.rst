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

After initializing the device, one has to initialize the input/output queues as well.

.. code-block:: python

  outputQueue = device.getOutputQueue("output_name")
  inputQueue = device.getInputQueue("input_name")

When you define an output queue, the device can push new messages to it at any point in time, and the host can read from it at any point in time.
Usually, when the host is reading very fast from the queue, the queue (regardless of its size) will stay empty most of
the time. But as we add things on the host side (additional processing, analysis, etc), it may happen that the device will be writing to
the queue faster than the host can read from it. And then the messages in the queue will start to add up - and both maxSize and blocking
flags determine the behavior of the queue in this case. You can set these flags with:

.. code-block:: python

  # When initializing the queue
  queue = device.getOutputQueue(name="name", maxSize=5, blocking=False)

  # Or afterwards
  queue.setMaxSize(10)
  queue.setBlocking(True)

Blocking behaviour
******************

By default, queues are **blocking** and their size is **30**, so when the device fills up a queue and when the limit is
reached, any additional messages from the device will be blocked and the library will wait until it can add new messages to the queue.
It will wait for the host to consume (eg. :code:`queue.get()`) a message before putting a new one into the queue.

.. note::
    After the host queue gets filled up, the XLinkOut.input queue on the device will start filling up. If that queue is
    set to blocking, other nodes that are sending messages to it will have to wait as well. This is a usual cause for a
    blocked pipeline, where one of the queues isn't emptied in timely manner and the rest of the pipeline waits for it
    to be empty again.

Non-Blocking behaviour
**********************
Making the queue non-blocking will change the behavior in the situation described above - instead of waiting, the library will discard
the oldest message and add the new one to the queue, and then continue its processing loop (so it won't get blocked).
:code:`maxSize` determines the size of the queue and it also helps to control memory usage.

For example, if a message has 5MB of data, and the queue size is 30, this queue can effectively store
up to 150MB of data in the memory on the host (the messages can also get really big, for instance, a single 4K NV12 encoded frame takes about ~12MB).

Some additional information
***************************

- Decreasing the queue size to 1 and setting non-blocking behavior will effectively mean "I only want the latest packet from the queue".
- Queues are thread-safe - they can be accessed from any thread.
- Queues are created such that each queue is its own thread which takes care of receiving, serializing/deserializing, and sending the messages forward (same for input/output queues).
- The :code:`Device` object isn't fully thread-safe. Some RPC calls (eg. :code:`getLogLevel`, :code:`setLogLevel`, :code:`getDdrMemoryUsage`) will get thread-safe once the mutex is set in place (right now there could be races).


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
