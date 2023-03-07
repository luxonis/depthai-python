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

  focal_length_in_pixels = image_width_in_pixels * 0.5 / tan(HFOV * 0.5 * PI/180)

  // With 400P mono camera resolution where HFOV=71.9 degrees
  focal_length_in_pixels = 640 * 0.5 / tan(71.9 * 0.5 * PI / 180) = 441.25

  // With 800P mono camera resolution where HFOV=71.9 degrees
  focal_length_in_pixels = 1280 * 0.5 / tan(71.9 * 0.5 * PI / 180) = 882.5

Examples for calculating the depth value, using the OAK-D (7.5cm baseline):

.. math::

  # For OAK-D @ 400P resolution and disparity of eg. 50 pixels
  depth = 441.25 * 7.5 / 50 = 66.19 # cm

  # For OAK-D @ 800P resolution and disparity of eg. 10 pixels
  depth = 882.5 * 7.5 / 10 = 661.88 # cm


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
