Stereo Depth from host
======================

This example shows depth map from host using stereo images. There are 3 depth modes which you can select inside the code:
#. lr_check: computes disparity with sub-pixel interpolation (5 fractional bits), suitable for long range
#. extended_disparity: mirror rectified frames: true to have disparity/depth normal (non-mirrored)
#. subpixel: suitable for short range objects
Otherwise a median with kernel_7x7 is activated.

.. rubric:: Similiar samples:

- :ref:`Stereo Depth Video`

Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires dataset folder - you can download it from
`here <https://drive.google.com/file/d/1mPyghc_0odGtSU2cROS1uLD-qwRU8aug>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/stereo_depth_from_host.py>`__

        .. literalinclude:: ../../../examples/stereo_depth_from_host.py
           :language: python
           :linenos:

.. include::  /includes/footer-short.rst
