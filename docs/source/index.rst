.. Luxonis Docs documentation master file, created by
   sphinx-quickstart on Tue Nov  3 14:34:56 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

DepthAI API Documentation
=========================

.. image:: https://github.com/luxonis/depthai-python/workflows/Python%20Wheel%20CI/badge.svg?branch=gen2_develop
   :target: https://github.com/luxonis/depthai-python/actions?query=workflow%3A%22Python+Wheel+CI%22+branch%3A%22gen2_develop%22

DepthAI API allows users to connect to, configure and communicate with their OAK devices.
We support both :ref:`Python API <Python API Reference>` and :ref:`C++ API <C++ API Reference>`.

.. image:: /_static/images/api_diagram.png

- **Host side** is a computer, like PC or RPi, to which an OAK device is connected.
- **Device side** is the OAK device itself. If something is happening on the device side, it means that it's running on the `Myriad X VPU <https://www.intel.com/content/www/us/en/products/details/processors/movidius-vpu/movidius-myriad-x.html>`__. More :ref:`information here <components_device>`.
- **Pipeline** is a complete workflow on the device side, consisting of :ref:`nodes <Nodes>` and connections between them. More :ref:`information here <components_device>`.
- **Node** is a single functionality of the DepthAI. :ref:`Nodes` have inputs or outputs, and have configurable properties (like resolution on the camera node).
- **Connection** is a link between one node's output and another one's input. In order to define the pipeline dataflow, the connections define where to send :ref:`messages <Messages>` in order to achieve an expected result
- **XLink** is a middleware that is capable to exchange data between device and host. :ref:`XLinkIn` node allows sending the data from the host to a device, while :ref:`XLinkOut` does the opposite.
- **Messages** are transferred between nodes, as defined by a connection. More :ref:`information here <components_messages>`.

Getting started
---------------

First, you need to :ref:`install the DepthAI <Installation>` library and its dependencies.

After installation, you can continue with an insightful :ref:`Hello World tutorial <Hello World>`, or with :ref:`code examples <Code Samples>`, where different
node functionalities are presented with code.

.. include::  /includes/footer-short.rst

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: Content:

   Home <self>
   install.rst

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Components:

   components/device.rst
   components/bootloader.rst
   components/pipeline.rst
   components/nodes.rst
   components/messages.rst

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Code:

   tutorials/code_samples.rst
   Experiments (GitHub) <https://github.com/luxonis/depthai-experiments>

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Tutorials:

   tutorials/hello_world.rst
   tutorials/standalone_mode.rst
   tutorials/multiple.rst
   tutorials/maximize_fov.rst
   tutorials/debugging.rst
   tutorials/ram_usage.rst
   tutorials/dispaying_detections.rst
   tutorials/camera_tuning.rst

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: References:

   references/python.rst
   references/cpp.rst
