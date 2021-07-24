MonoCamera
==========

MonoCamera node is a source of :ref:`image frames <ImgFrame>`. You can control in at runtime with the :code:`inputControl`. Some DepthAI modules don't
have mono camera(s). Two mono cameras are used to calculate stereo depth (with :ref:`StereoDepth` node).

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mono = pipeline.create(dai.node.MonoCamera)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mono = pipeline.create<dai::node::MonoCamera>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
                 │                   │
                 │                   │
  inputControl   │                   │       out
  ──────────────►│    MonoCamera     ├───────────►
                 │                   │
                 │                   │
                 │                   │
                 └───────────────────┘

**Message types**

- :code:`inputControl` - :ref:`CameraControl`
- :code:`out` - :ref:`ImgFrame`

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    mono = pipeline.create(dai.node.MonoCamera)
    mono.setBoardSocket(dai.CameraBoardSocket.RIGHT)
    mono.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto mono = pipeline.create<dai::node::MonoCamera>();
    mono->setBoardSocket(dai::CameraBoardSocket::RIGHT);
    mono->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);

Examples of functionality
#########################

- :ref:`Mono Preview`
- :ref:`Mono & MobilenetSSD`
- :ref:`Mono Camera Control`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.MonoCamera
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::MonoCamera
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
