.. Luxonis Docs documentation master file, created by
   sphinx-quickstart on Tue Nov  3 14:34:56 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to DepthAI Gen2 API Documentation
=========================================

.. image:: https://github.com/luxonis/depthai-python/workflows/Python%20Wheel%20CI/badge.svg?branch=gen2_develop
   :target: https://github.com/luxonis/depthai-python/actions?query=workflow%3A%22Python+Wheel+CI%22+branch%3A%22gen2_develop%22

On this page you can find the details regarding the Gen2 DepthAI API that will allow you to interact with the DepthAI device.
We support both :ref:`Python API <Python API Reference>` and :ref:`C++ API <C++ API Reference>`

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

To help you get started with Gen2 API, we have prepared multiple examples of it's usage, with more yet to come, together
with some insightful tutorials.

Before running the example, install the DepthAI Python library using the command below

.. code-block:: python
   :substitutions:

   python3 -m pip install -U --force-reinstall depthai


Now, pick a tutorial or code sample and start utilizing Gen2 capabilities

.. toctree::
   :maxdepth: 0
   :hidden:
   :caption: Content:

   Home <self>
   install.rst
   tutorials/overview.rst

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
   :caption: Tutorials:

   tutorials/hello_world.rst
   tutorials/multiple.rst
   tutorials/maximize_fov.rst

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Code:

   tutorials/code_samples.rst
   Experiments (GitHub) <https://github.com/luxonis/depthai-experiments>

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: References:

   references/python.rst
   references/cpp.rst
