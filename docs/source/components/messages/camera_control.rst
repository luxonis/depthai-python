CameraControl
=============

This message is used for controlling the :ref:`color camera <ColorCamera>` as well as the :ref:`mono camera <MonoCamera>`.
The message handles things like capturing still images, configuring auto focus, anti banding, white balance,
scenes, effects etc.

Examples of functionality
#########################

- :ref:`RGB Camera Control`
- :ref:`Mono Camera Control`
- :ref:`Auto Exposure on ROI`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.CameraControl
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::CameraControl
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
