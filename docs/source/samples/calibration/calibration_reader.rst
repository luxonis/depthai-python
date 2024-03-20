Calibration Reader
==================

This example shows how to read calibration data stored on device over XLink. This example will print camera extrinsic and instrinsic parameters, along with other calibration values written on device (EEPROM).

.. rubric:: Similar samples:

- :ref:`Calibration Flash v5`
- :ref:`Calibration Flash`
- :ref:`Calibration Load`

Camera intrinsics
~~~~~~~~~~~~~~~~~

Calibration also contains camera intrinsics and extrinsics parameters.

.. code-block:: python

  import depthai as dai

  with dai.Device() as device:
    calibData = device.readCalibration()
    intrinsics = calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT)
    print('Right mono camera focal length in pixels:', intrinsics[0][0])

Here's theoretical calculation of the focal length in pixels:

.. math::

  f_x = \text{width}_x \cdot \frac{1}{2 \cdot \tan \left(\frac{\text{HFOV}_x}{2}\frac{\pi}{180}\right)}

To get the HFOV you can use `this script <https://gist.github.com/Erol444/4aff71f4576637624d56dce4a60ad62e>`__, which also works for wide-FOV cameras and allows you to
specif alpha parameter.

With 400P (640x400) camera resolution where HFOV=71.9 degrees:

.. math::

  f_x = 640 \cdot \frac{1}{2 \cdot \tan \left(\frac{71.9}{2}\frac{\pi}{180}\right)} = 441.25

And for 800P (1280x800) camera resolution where HFOV=71.9 degrees:

.. math::

  f_x = 1280 \cdot \frac{1}{2 \cdot \tan \left(\frac{71.9}{2}\frac{\pi}{180}\right)} = 882.5




Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/calibration/calibration_reader.py>`__

        .. literalinclude:: ../../../../examples/calibration/calibration_reader.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/calibration/calibration_reader.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/calibration/calibration_reader.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
