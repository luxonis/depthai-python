Overview
========

..
  Section which described mental model, flow / pipeline programming,
  messages that carry data, nodes that compute upon them

.. code-block::

              DepthAI device (eg. OAK-D)                                  Host (eg. RaspberryPi)
  ┌───────────────────────────────────────────────┐                   ┌─────────────────────────┐
  │                                               │                   │                         │
  │       Node                            Node    │                   │ # Your python code that │
  │  ┌─────────────┐                   ┌──────────┤                   │ # runs on the host      │
  │  │             │                   │          │                   │                         │
  │  │             │preview       input│          │   XLink protocol  │ # Get the frame         │
  │  │ ColorCamera ├───────────────────┤ XLinkOut ├──────────────────►│ data=q_preview.get()    │
  │  │             │     ImgFrame      │          │(USB/Ethernet/PCIe)│ frame=data.getCvFrame() │
  │  │             │     Message       │          │                   │ # Show the frame        │
  │  └─────────────┘                   └──────────┤                   │ cv2.imshow("rgb",frame) │
  │ inputControl ▲                                │                   │                         │
  │              │                                │                   │                         │
  │              │                        Node    │                   │                         │
  │              │                      ┌─────────┤                   │ # Control the camera    │
  │              │                      │         │                   │ cc=dai.CameraControl()  │
  │              │                  out │         │   XLink protocol  │ cc.setManualFocus(100)  │
  │              └──────────────────────┤ XLinkIn │◄──────────────────┤ q_cam_control.send(cc)  │
  │                  CameraControl      │         │(USB/Ethernet/PCIe)│                         │
  │                     Message         │         │                   │                         │
  │                                     └─────────┤                   │                         │
  │                                               │                   │                         │
  └───────────────────────────────────────────────┘                   └─────────────────────────┘
                                  A simple pipeline visualzied

Device
######

Device is the `DepthAI module <https://docs.luxonis.com/en/latest/pages/products/>`__ itself. On the device there is a powerful vision processing unit
(:code:`VPU`) from Intel, called `Myriad X <https://www.intel.com/content/www/us/en/products/details/processors/movidius-vpu.html>`__ (MX for short).
The VPU is optimized for performing AI inference algorithms and for processing sensory inputs (eg. calculating stereo disparity from two cameras).

For more details, click :ref:`here <components_device>`

Pipeline
########

The upper flowchart is a simple pipeline visualized. So a **pipeline is collection of nodes and links** between them.

For more details, click :ref:`here <components_pipeline>`

Nodes
#####

Each node provides a specific functionality on the DepthAI, a set of configurable properties and inputs/outputs. On the flowchart above, we have 3 nodes;
:code:`ColorCamera`, :code:`XLinkOut` and :code:`XLinkIn`.

For more details, click :ref:`here <components_nodes>`

Messages
########

Messages are sent between linked nodes. On the flowchart above, there are two links - visualized as arrows that are inside the device. There are a few
different types of messages, on the chart we have :code:`ImgFrame` and :code:`CameraControl`

For more details, click :ref:`here <components_messages>`

.. include::  ../includes/footer-short.rst