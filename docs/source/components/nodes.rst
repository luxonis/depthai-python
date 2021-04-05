Nodes
=====

List of all nodes, ref links to the node documentation page where the node is further described in detail

Every node in has an input queue whose default size (8) can be changed with `setQueueSize()`, and default behaviour (blocking) can be changed with 
`setBlocking()`. If queue size fills up, behavior depends on blocking attribute.

Q:
- In addition some type of nodes have an output queue. When full, the node stops and waits for the results to be consumed before
producing new ones. The size of the output queue can be changed with setNumPoolFrames for NeuralNetwork nodes, with setNumFramesPool for
ImageManip and VideoEncoder nodes. I don't know if the "same-same but different" function names is relevant or not
A:
This isn't an output queue per se, but the size of the pool of how many frames can be created and sent out while other frames are already
somewhere in the pipeline. When all the frames (messages from pool) were sent out and none yet returned, that is when the node will block and
wait until a frame is returned to the pool (not used by any node in the pipeline anymore)

- On the host side, we have output data queues, and sometimes input data queues, whose size and behaviour are set at creation 
(device.getOutputQueue and device.getInputQueue).
Correct, the size and behavior can be modified by calling device.getOutputQueue again (or directly on the returned object 
DataInputQueue / DataOutputQueue)


ImageManip
##########

ImageManip node can be used to crop, rotate rectangle area or perform various image transforms: rotate, mirror, flip, perspective transform.

For more details, click :ref:`here <ImageManip node>`


Reference
*********

.. autoclass:: depthai.Node
  :members:
  :inherited-members: