Mono video
==========

This example shows how to use mono video at low latency. Compared to :ref:`Mono Preview`, this demo outputs NV12 frames whereas
output frames are BGR and are not suited for larger resoulution.

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/-sTQLEVvO38" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/mono_video.py>`__

.. literalinclude:: ../../../examples/mono_video.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
