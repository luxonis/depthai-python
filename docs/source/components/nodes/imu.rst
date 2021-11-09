IMU
===

IMU (`intertial measurement unit <https://en.wikipedia.org/wiki/Inertial_measurement_unit>`__) node can be used to receive data from the IMU chip on the device.
Our DepthAI devices use `BNO085 <https://www.ceva-dsp.com/product/bno080-085/>`__ 9-axis sensor (`datasheet here <https://www.ceva-dsp.com/wp-content/uploads/2019/10/BNO080_085-Datasheet.pdf>`__)
that supports sensor fusion on the (IMU) chip itself. The IMU chip is connected to the Myriad X (VPU) over SPI (we have integrated
`this driver <https://github.com/hcrest/bno080-driver>`__ to the DepthAI).


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

Maximum frequencies
###################

Maximum output frequencies are 500 Hz raw accelerometer, 1000 Hz raw gyroscope values individually, and 500 Hz combined (synced) output.
You can obtain the combined (synced) 500 Hz output with :code:`imu.enableIMUSensor([dai.IMUSensor.RAW_ACCELEROMETER, dai.IMUSensor.RAW_GYROSCOPE], 500)`.

Usage
#####

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    imu = pipeline.create(dai.node.IMU)

    # enable RAW_ACCELEROMETER and RAW_GYROSCOPE at 100 hz rate
    imu.enableIMUSensor([dai.IMUSensor.RAW_ACCELEROMETER, dai.IMUSensor.RAW_GYROSCOPE], 100)
    # above this threshold packets will be sent in batch of X, if the host is not blocked and USB bandwidth is available
    imu.setBatchReportThreshold(1)
    # maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
    # if lower or equal to batchReportThreshold then the sending is always blocking on device
    # useful to reduce device's CPU load  and number of lost packets, if CPU load is high on device side due to multiple nodes
    imu.setMaxBatchReports(10)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto imu = pipeline.create<dai::node::IMU>();

    // enable RAW_ACCELEROMETER and RAW_GYROSCOPE at 100 hz rate
    imu->enableIMUSensor({dai::IMUSensor::RAW_ACCELEROMETER, dai::IMUSensor::RAW_GYROSCOPE}, 100);
    // above this threshold packets will be sent in batch of X, if the host is not blocked and USB bandwidth is available
    imu->setBatchReportThreshold(1);
    // maximum number of IMU packets in a batch, if it's reached device will block sending until host can receive it
    // if lower or equal to batchReportThreshold then the sending is always blocking on device
    // useful to reduce device's CPU load  and number of lost packets, if CPU load is high on device side due to multiple nodes
    imu->setMaxBatchReports(10);

IMU devices
###########

List of devices that have an IMU sensor on-board:

* `OAK-D <https://docs.luxonis.com/projects/hardware/en/latest/pages/BW1098OAK.html>`__
* `OAK-D-IoT-40 <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM1092.html>`__
* `OAK-D-IoT-75 <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM1098OBC.html>`__
* `OAK-D-CM4 <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM1097.html>`__
* `OAK-D-PoE <https://docs.luxonis.com/projects/hardware/en/latest/pages/SJ2088POE.html>`__
* `OAK-FFC-3P <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM1090.html>`__
* OAK-D-Pro
* OAK-WD-Pro


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
