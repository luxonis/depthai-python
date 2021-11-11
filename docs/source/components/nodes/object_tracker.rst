ObjectTracker
=============

Object tracker tracks detected objects from the :ref:`ImgDetections` using Kalman filter and hungarian algorithm.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    objectTracker = pipeline.create(dai.node.ObjectTracker)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto objectTracker = pipeline.create<dai::node::ObjectTracker>();


Inputs and Outputs
##################

.. code-block::

                      ┌───────────────────┐
  inputDetectionFrame │                   │passthroughDetectionFrame
  ───────────────────►│-------------------├─────────────────────────►
                      │                   │                      out
                      │      Object       ├─────────────────────────►
  inputTrackerFrame   │      Tracker      │  passthroughTrackerFrame
  ───────────────────►│-------------------├─────────────────────────►
  inputDetections     │                   │    passthroughDetections
  ───────────────────►│-------------------├─────────────────────────►
                      └───────────────────┘

**Message types**

- :code:`inputDetectionFrame` - :ref:`ImgFrame`
- :code:`inputTrackerFrame` - :ref:`ImgFrame`
- :code:`inputDetections` - :ref:`ImgDetections`
- :code:`out` - :ref:`Tracklets`
- :code:`passthroughDetectionFrame` - :ref:`ImgFrame`
- :code:`passthroughTrackerFrame` - :ref:`ImgFrame`
- :code:`passthroughDetections` - :ref:`ImgDetections`


Zero term tracking
##################

Zero term tracking performs object association, which means that it does not conduct prediction and tracking based on previous tracking history.
Object association would mean that detected objects from an external detector are mapped with tracked objects which has been detected and is being tracked from
previous frames.

Supported object tracker types:

- :code:`ZERO_TERM_COLOR_HISTOGRAM`: Utilizes position, shape and input image information such as RGB histogram to perform object tracking.
- :code:`ZERO_TERM_IMAGELESS`: Only utilizes rectangular shape of detected object and position information for object tracking. It does not use color information of tracking objects. It achieves higher throughput than ZERO_TERM_COLOR_HISTOGRAM. User needs to consider the trade-off between throughput and accuracy when choosing the object tracker type.

Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      objectTracker = pipeline.create(dai.node.ObjectTracker)

      objectTracker.setDetectionLabelsToTrack([15])  # Track only person
      # Possible tracking types: ZERO_TERM_COLOR_HISTOGRAM, ZERO_TERM_IMAGELESS
      objectTracker.setTrackerType(dai.TrackerType.ZERO_TERM_COLOR_HISTOGRAM)
      # Take the smallest ID when new object is tracked, possible options: SMALLEST_ID, UNIQUE_ID
      objectTracker.setTrackerIdAssigmentPolicy(dai.TrackerIdAssigmentPolicy.SMALLEST_ID)

      # You have to use Object tracker in combination with detection network
      # and an image frame source - mono/color camera or xlinkIn node

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto objectTracker = pipeline.create<dai::node::ObjectTracker>();

      objectTracker->setDetectionLabelsToTrack({15});  // Track only person
      // Possible tracking types: ZERO_TERM_COLOR_HISTOGRAM, ZERO_TERM_IMAGELESS
      objectTracker->setTrackerType(dai::TrackerType::ZERO_TERM_COLOR_HISTOGRAM);
      // Take the smallest ID when new object is tracked, possible options: SMALLEST_ID, UNIQUE_ID
      objectTracker->setTrackerIdAssigmentPolicy(dai::TrackerIdAssigmentPolicy::SMALLEST_ID);

      // You have to use Object tracker in combination with detection network
      // and an image frame source - mono/color camera or xlinkIn node

Examples of functionality
#########################

- :ref:`Object tracker on RGB`
- :ref:`Spatial object tracker on RGB`
- :ref:`Object tracker on video`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.ObjectTracker
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::ObjectTracker
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

.. include::  ../../includes/footer-short.rst
