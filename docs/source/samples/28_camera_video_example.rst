28 - Camera video high resolution
=================================

This example shows how to use high resolution video at low latency. Compared to :ref:`01 - RGB Preview`, this demo outputs NV12 frames whereas
preview frames are BGR and are not suited for larger resoulution (eg. 2000x1000). Preview is more suitable for either NN or visualization purposes.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/develop/examples/28_camera_video_example.py>`__

.. literalinclude:: ../../../examples/28_camera_video_example.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst