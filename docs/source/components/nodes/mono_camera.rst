MonoCamera
==========

MonoCamera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`InputControl`. Some DepthAI modules don't
have mono camera(s). Two mono cameras are used to calculate stereo depth (with :ref:`StereoDepth` node).

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mono = pipeline.createMonoCamera()

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mono = pipeline.create<dai::node::MonoCamera>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
  InitialControl │                   │
  ──────────────►│                   │
                 │                   │       Out
                 │    MonoCamera     ├───────────►
  InputControl   │                   │
  ──────────────►│                   │
                 │                   │
                 └───────────────────┘

Message types
#############

- :code:`InitialControl` - :ref:`CameraControl`
- :code:`InputControl` - :ref:`CameraControl`
- :code:`Out` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mono = pipeline.createMonoCamera()
    mono.setBoardSocket(dai.CameraBoardSocket.RIGHT)
    mono.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mono = pipeline.create<dai::node::MonoCamera>();

Examples of functionality
#########################

- :ref:`02 - Mono Preview`
- :ref:`09 - Mono & MobilenetSSD`
- :ref:`19 - Mono Camera Control`

Reference
#########

.. autoclass:: depthai.MonoCamera
  :members:
  :inherited-members: