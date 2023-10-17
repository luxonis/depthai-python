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
