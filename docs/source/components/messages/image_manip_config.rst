ImageManipConfig
================

This message can is used for cropping, warping, rotating, resizing, etc. an image in runtime.
It is sent either from the host to :ref:`ColorCamera` or :ref:`ImageManip`.

..
  It is sent either from the host or from the :ref:`Script` node to :ref:`ColorCamera` or :ref:`ImageManip`.

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
