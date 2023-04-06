ImageManipConfig
================

This message can is used for cropping, warping, rotating, resizing, etc. an image in runtime.
It can be sent from host/:ref:`Script` node to either :ref:`ColorCamera` or :ref:`ImageManip`.

.. note:: This message will reconfigure the whole config of the node, meaning you need to set all settings, not just the setting you want to change.

Examples of functionality
#########################

- :ref:`RGB Camera Control`
- :ref:`RGB Rotate Warp`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.ImageManipConfig
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::ImageManipConfig
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
