IMU
===

IMU (`inertial measurement unit <https://en.wikipedia.org/wiki/Inertial_measurement_unit>`__) node can be used to receive data
from the IMU chip on the device. Our OAK devices use either:

- `BNO085 <https://www.ceva-dsp.com/product/bno080-085/>`__ (`datasheet here <https://www.ceva-dsp.com/wp-content/uploads/2019/10/BNO080_085-Datasheet.pdf>`__) 9-axis sensor, combining accelerometer, gyroscope, and magnetometer. It also does sensor fusion on the (IMU) chip itself. We have efficiently integrated `this driver <https://github.com/hcrest/bno080-driver>`__ into the DepthAI.
- `BMI270 <https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi270/>`__ 6-axis sensor, combining accelerometer and gyroscope.

The IMU chip is connected to the `RVC <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html#rvc2>`__
over SPI. See `OAK Hardware documentation <https://docs.luxonis.com/projects/hardware/en/latest/>`__ to check whether your OAK camera has IMU integrated.


How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    imu = pipeline.create(dai.node.IMU)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto imu = pipeline.create<dai::node::IMU>();


Inputs and Outputs
##################

.. code-block::

  ┌──────────────┐
  │              │
  │              │      out
  │     IMU      ├─────────►
  │              │
  │              │
  └──────────────┘

**Message types**

- :code:`out` - :ref:`IMUData`

Limitations
###########

- For BNO086, gyroscope frequency above 400Hz can produce some jitter from time to time due to sensor HW limitation.

IMU sensor frequencies
######################

Below are the discrete **stable frequencies** available for each (raw) IMU sensor. Some maximum IMU frequencies are higher, eg.
for BNO086, maximum frequency for gyroscope is 1000Hz, but up to 400Hz is stable (due to driver limitation).

**BNO086:**

Note that BNO IMU "rounds up" the input frequency to the next available frequency. For example, if you set the frequency to 101 it will round it to 200Hz.

- Accelerometer: 15Hz, 31Hz, 62Hz, 125Hz, 250Hz 500Hz
- Gyroscope: 25Hz, 33Hz, 50Hz, 100Hz, 200Hz, 400Hz
- Magnetometer: 100Hz

**BNO086 max frequency:**

.. list-table::
   :header-rows: 1

   * - BNO086 Sensor
     - Max Frequency
   * - ``ACCELEROMETER_RAW``
     - 512 Hz
   * - ``ACCELEROMETER``
     - 512 Hz
   * - ``LINEAR_ACCELERATION``
     - 400 Hz
   * - ``GRAVITY``
     - 400 Hz
   * - ``GYROSCOPE_RAW``
     - 1000 Hz
   * - ``GYROSCOPE_CALIBRATED`` / ``GYROSCOPE_UNCALIBRATED``
     - 100 Hz
   * - ``MAGNETOMETER_RAW``
     - 100 Hz
   * - ``MAGNETOMETER_CALIBRATED`` / ``MAGNETOMETER_UNCALIBRATED``
     - 100 Hz
   * - ``ROTATION_VECTOR``
     - 400 Hz
   * - ``GAME_ROTATION_VECTOR``
     - 400 Hz
   * - ``GEOMAGNETIC_ROTATION_VECTOR``
     - 100 Hz
   * - ``ARVR_STABILIZED_ROTATION_VECTOR``
     - 100 Hz
   * - ``ARVR_STABILIZED_GAME_ROTATION_VECTOR``
     - 100 Hz

**BMI270:**

Note that BMI279 "rounds down" the input frequency to the next available frequency. For example, if you set the frequency to 99 it will round it to 50Hz.
Additionally, the current max frequency of ~250 Hz is set when the input is >400Hz.

- Accelerometer: 25Hz, 50Hz, 100Hz, 200Hz, 250Hz
- Gyroscope: 25Hz, 50Hz, 100Hz, 200Hz, 250Hz

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    imu = pipeline.create(dai.node.IMU)

    # enable ACCELEROMETER_RAW and GYROSCOPE_RAW at 100 hz rate
    imu.enableIMUSensor([dai.IMUSensor.ACCELEROMETER_RAW, dai.IMUSensor.GYROSCOPE_RAW], 100)
    # above this threshold packets will be sent in batch of X, if the host is not blocked and USB bandwidth is available
    imu.setBatchReportThreshold(1)
    # maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
    # if lower or equal to batchReportThreshold then the sending is always blocking on device
    # useful to reduce device's CPU load  and number of lost packets, if CPU load is high on device side due to multiple nodes
    imu.setMaxBatchReports(10)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto imu = pipeline.create<dai::node::IMU>();

    // enable ACCELEROMETER_RAW and GYROSCOPE_RAW at 100 hz rate
    imu->enableIMUSensor({dai::IMUSensor::ACCELEROMETER_RAW, dai::IMUSensor::GYROSCOPE_RAW}, 100);
    // above this threshold packets will be sent in batch of X, if the host is not blocked and USB bandwidth is available
    imu->setBatchReportThreshold(1);
    // maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
    // if lower or equal to batchReportThreshold then the sending is always blocking on device
    // useful to reduce device's CPU load  and number of lost packets, if CPU load is high on device side due to multiple nodes
    imu->setMaxBatchReports(10);

IMU sensors
###########

When enabling the IMU sensors (:code:`imu.enableIMUSensor()`), you can select between the following sensors:

- :code:`ACCELEROMETER_RAW`
- :code:`ACCELEROMETER`
- :code:`LINEAR_ACCELERATION`
- :code:`GRAVITY`
- :code:`GYROSCOPE_RAW`
- :code:`GYROSCOPE_CALIBRATED`
- :code:`GYROSCOPE_UNCALIBRATED`
- :code:`MAGNETOMETER_RAW`
- :code:`MAGNETOMETER_CALIBRATED`
- :code:`MAGNETOMETER_UNCALIBRATED`
- :code:`ROTATION_VECTOR`
- :code:`GAME_ROTATION_VECTOR`
- :code:`GEOMAGNETIC_ROTATION_VECTOR`
- :code:`ARVR_STABILIZED_ROTATION_VECTOR`
- :code:`ARVR_STABILIZED_GAME_ROTATION_VECTOR`

Here are **descriptions of all sensors**:

.. autoclass:: depthai.IMUSensor
  :noindex:

Examples of functionality
#########################

- :ref:`IMU Accelerometer & Gyroscope`
- :ref:`IMU Rotation Vector`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.IMU
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::IMU
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
