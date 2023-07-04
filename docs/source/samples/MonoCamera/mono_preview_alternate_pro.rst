Mono preview - Alternate between dot projector and illumination LED
===================================================================

This example will alternate between the IR illumination LED and IR dot projector. By default, example script will run
both left and right monochrome camera sensors at 30FPS, and it will switch between the IR LED and dot projector
every frame - meaning you will get LED-illuminated frames at 15FPS, and dot projector-illuminated frames at 15FPS.

LED-illuminated frames can be used for your `AI vision tasks <https://docs.luxonis.com/en/latest/pages/ai_ml_nn/#ai-vision-tasks>`__
and CV algorithms (eg. :ref:`Feature Tracker`) in low-light environments. Dot projector-illuminated frames are used for `active stereo depth <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/oak-s2/#pro-version>`__.

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/LkW50C4N-R8" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

On the video, we disabled both projector and LED for about a second, just to demonstrate how the scene looks
in almost-complete darkness.

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/MonoCamera/mono_preview_alternate_pro.py>`__

        .. literalinclude:: ../../../../examples/MonoCamera/mono_preview_alternate_pro.py
           :language: python
           :linenos:

    .. tab:: C++

        Not yet implemented.

..
            Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/MonoCamera/mono_preview_alternate_pro.cpp>`__

            .. literalinclude:: ../../../../depthai-core/examples/MonoCamera/mono_preview_alternate_pro.cpp
            :language: cpp
            :linenos:

.. include::  /includes/footer-short.rst
