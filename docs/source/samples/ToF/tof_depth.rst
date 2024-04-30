ToF depth
=========

This is a sample code that showcases how to use the ToF sensor. The :ref:`ToF node <ToF>` converts raw data from the ToF sensor into a depth map.

Demo
####

This demo was recorded using the `OAK-D SR PoE <https://docs.luxonis.com/projects/hardware/en/latest/pages/OAK-D-SR-POE/>`__, that's why we selected CAM_A port
on the ToF sensor.

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/D2MnnyxdsMA" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

With keyboard you can configure ToF settings:

- FPPN Correction; Turn on/off with `f`. It's a process that corrects the fixed pattern noise (FPN) of the ToF sensor. Should be enabled.
- Wiggle Correction: Turn on/off with `w`. It's a process that corrects the wiggle effect of the ToF sensor. Should be enabled.
- Temperature Correction: Turn on/off with `t`. It's a process that corrects the temperature effect of the ToF sensor. Should be enabled.
- Optical Correction: Turn on/off with `o`. It's a process that corrects the optical effect (On -> ToF returns distance represented by Green Line), so it matches stereo depth reporting.
- Phase Unwrapping - Process that corrects the phase wrapping effect of the ToF sensor. The higher the number, the longer the ToF range, but it also increases the noise.
    - `0` - Disabled.
    - `1` - Up to 1.5 meters
    - `2` - Up to 3 meters
    - `3` - Up to 4.5 meters
    - `4` - Up to 6 meters
    - `5` - Up to 6 meters

.. image:: /_static/images/components/tof-optical-correction.png

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/ToF/tof_depth.py>`__

        .. literalinclude:: ../../../../examples/ToF/tof_depth.py
           :language: python
           :linenos:

    .. tab:: C++

        ..
            Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/ToF/tof_depth.cpp>`__

            .. literalinclude:: ../../../../depthai-core/examples/ToF/tof_depth.cpp
            :language: cpp
            :linenos:

.. include::  /includes/footer-short.rst
