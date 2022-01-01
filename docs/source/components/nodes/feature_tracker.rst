FeatureTracker
==============

FeatureTracker detects key points (features) on a frame and tracks them on the next frame.
The valid features are obtained from the Harris score or Shi-Tomasi.
The default number of target features is 320 and the default maximum number of features is 480.
It supports 720p and 480p resolutions.


How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    featureTracker = pipeline.create(dai.node.FeatureTracker)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto featureTracker = pipeline.create<dai::node::FeatureTracker>();


Inputs and Outputs
##################

.. code-block::

               ┌─────────────────┐
  inputConfig  │                 │       outputFeatures
  ────────────►│     Feature     ├────────────────────►
  inputImage   │     Tracker     │passthroughInputImage
  ────────────►│-----------------├────────────────────►
               └─────────────────┘


**Message types**

- :code:`inputConfig` - :ref:`FeatureTrackerConfig`
- :code:`inputImage` - :ref:`ImgFrame`
- :code:`outputFeatures` - :ref:`TrackedFeatures`
- :code:`passthroughInputImage` - :ref:`ImgFrame`


Usage
#####

.. tabs::

  .. code-tab:: py

      pipeline = dai.Pipeline()
      featureTracker = pipeline.create(dai.node.FeatureTracker)

      # Set number of shaves and number of memory slices to maximum
      featureTracker.setHardwareResources(2, 2)
      # Specify to wait until configuration message arrives to inputConfig Input.
      featureTracker.setWaitForConfigInput(True)

      # You have to use Feature tracker in combination with
      # an image frame source - mono/color camera or xlinkIn node

  .. code-tab:: c++

      dai::Pipeline pipeline;
      auto featureTracker = pipeline.create<dai::node::FeatureTracker>();

      // Set number of shaves and number of memory slices to maximum
      featureTracker->setHardwareResources(2, 2);
      // Specify to wait until configuration message arrives to inputConfig Input.
      featureTracker->setWaitForConfigInput(true);

      // You have to use Feature tracker in combination with
      // an image frame source - mono/color camera or xlinkIn node

Examples of functionality
#########################

- :ref:`Feature Detector`
- :ref:`Feature Tracker`

Reference
#########

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.FeatureTracker
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::FeatureTracker
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

Image cells
###########

To have features all around the image, it is divided into cells which are then processed separately.
Each cell has a :code:`target feature count = frame target features / number of cells`.
The number of cells can be configured in horizontal and in vertical direction. The default number of cells is
:code:`4` (horizontal) x :code:`4` (vertical). This means that the default number of target features per cell is:
:code:`320 / (4 * 4) = 20`. Note that if an already tracked point happens to have its new coordinate in a 
full cell, it will not be removed, therefore number of features can exceed this limit.

Initial Harris Threshold
########################

This threshold controls the minimum strength of a feature which will be detected. Setting this value to 0
enables the automatic thresholds, which are used to adapt to different scenes. If there is a lot of texture,
this value needs to be increased, to limit the number of detected points. Each cell has its own threshold.

Entry conditions for new features
#################################

The entry conditions for new features are:

- features must not be too close to each other (minimum distance criteria - default value is 50, the unit of measurement being squared euclidean distance in pixels),
- Harris score of the feature is high enough,
- there is enough *room* in the cell for the feature (target feature count is not achieved).


Harris Threshold for Tracked Features
#####################################

Once a feature was detected and we started tracking it, we need to update its Harris score on each image.
This threshold defines the point where such a feature must be removed.
The goal is to track points for as long as possible, so the conditoins for entry points are higher than the ones
for already tracked points. This is why, this value is usually smaller than the detection threshold.


Feature Maintenance
###################

The algorithm has to decide which feature will be removed and which will be kept in the
subsequent frames. Note that tracked features have priority over new features.
It will remove the features which:

- have too large tracking error (wasn't tracked correctly),
- have too small Harris score (configurable threshold).

New position calculation
########################

A position of the previous features on the current frame can be calculated in two ways:

1. Using the pyramidal Lucas-Kanade optical flow method.
2. Using a dense motion estimation hardware block (Block matcher).

.. include::  ../../includes/footer-short.rst