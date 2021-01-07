.. Luxonis Docs documentation master file, created by
   sphinx-quickstart on Tue Nov  3 14:34:56 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to DepthAI Gen2 API Documentation
=========================================

On this page you can find the details regarding DepthAI API that will allow you to interact with the DepthAI device.
We support both :ref:`Python API <Python API Reference>` and C++ API

What is Gen2?
-------------

Gen2 is a step forward in DepthAI integration, allowing users to define their own flow of data using pipelines, nodes
and connections. Gen2 was created based on user's feedback from Gen1 and from raising capabilities of both DepthAI and
supporting software like OpenVINO.

Basic glossary
--------------

- **Host side** is the device, like PC or RPi, to which the DepthAI is connected to. If something is happening on the host side, it means that this device is involved in it, not DepthAI itself

- **Device side** is the DepthAI itself. If something is happening on the device side, it means that the DepthAI is responsible for it

- **Pipeline** is a complete workflow on the device side, consisting of nodes and connections between them - these cannot exist outside of pipeline.

- **Node** is a single functionality of the DepthAI. It have either inputs or outputs or both, together with properties to be defined (like resolution on the camera node or blob path in neural network node)

- **Connection** is a link between one node's output and another one's input. In order to define the pipeline dataflow, the connections define where to send data in order to achieve an expected result

- **XLink** is a middleware that is capable to exchange data between device and host. XLinkIn node allows to send the data from host to device, XLinkOut does the opposite.

Getting started
---------------

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: References:

   references/python.rst
