RAM usage
=========

All OAK devices have 512 MiB (4 Gbit) on-board RAM, which is used for firmware (about 15MB), assets (a few KB up to 100MB+, eg. NN models), and other
resources, such as message pools where messages are stored.

If you enable :code:`info` :ref:`logging <depthai_logging>`, you will see how RAM is used:

.. code-block:: bash

    [info] Memory Usage - DDR: 41.23 / 358.82 MiB, CMX: 2.17 / 2.50 MiB,
    LeonOS Heap: 20.70 / 78.63 MiB, LeonRT Heap: 3.51 / 23.84 MiB

As you can see, RAM is split between the two LEON (CPU) cores, :code:`CMX` (used for image manipulation), and :code:`DDR` (everything else).
If :code:`DDR` usage is close to the max (in this example, 358 MiB), you might get an error such as:

.. code-block:: bash

    [error] Neural network executor '0' out of '2' error: OUT_OF_MEMORY

This means you should decrease RAM consumption, and we will take a look at a few ways on how to do this.

Decreasing RAM consumption
##########################

- **Large frames**

    If we change the resolution from 1080P to 4K in the :ref:`RGB video` example, DDR usage will increase from 41 MiB to 161 MiB. That's because
    4K uses 4x more RAM compared to 1080P frame. An easy way to decrease RAM consumption is to use lower resolution / smaller frames.

- **VideoEncoder**

    :ref:`VideoEncoder` nodes can consume a lot of RAM, especially at high resolutions. For example, :ref:`RGB Encoding` example consumes
    259 MiB. If we change the resolution from 4K to 1080P, we decrease DDR consumption to only 65 MiB.

- **ImageManip**

    Each :ref:`ImageManip` node will have it's own (output) pool of 4 frames (by default), so having multiple ImageManips that are manipulating
    high resolution frames will consume a lot of DDR RAM. You can change the pool size with :code:`manip.setNumFramesPool(4)`. By default,
    each pool "spot" will consume 1 MiB, even if it's a small 300x300 RGB frame (which is 270kB). Specifying the output frame size
    can therefore decrease the RAM as well, eg. for a 300x300 RGB frame, you can set :code:`manip.setMaxOutputFrameSize(270000)`.

- **XLinkIn**

    Just like :ref:`ImageManip`, each :ref:`XLinkIn` node has it's own message pool as well. By default, each XLinkIn will consume 40 MiB, as
    each pool "spot" has 5 MiB reserved, and there are 8 "spots" in the pool. If you are sending 300x300 RGB frames from the host to the device,
    you can set :code:`xin.setMaxDataSize(270000)`, and also limit number of messages per pool :code:`xin.setNumFrames(4)`. This will decrease
    DDR RAM consumption from 40 MiB to about 1 MiB.

    If you are just sending control/config from the host, you can
    set :code:`xin.setMaxDataSize(1)`, as :ref:`CameraControl` and :ref:`ImageManipConfig` don't have any extra data
    (like :ref:`NNData`/:ref:`ImgFrame`/:ref:`Buffer`).

.. include::  ../includes/footer-short.rst