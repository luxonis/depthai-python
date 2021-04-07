30 - Stereo Depth from host
===========================

This example shows depth map from host using stereo images. There are 3 depth modes which you can select inside the code: left-right check, extended (for closer distance), subpixel (for longer distance). Otherwise a median with kernel_7x7 is activated.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/30_stereo_depth_from_host.py>`__

.. literalinclude:: ../../../examples/30_stereo_depth_from_host.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
