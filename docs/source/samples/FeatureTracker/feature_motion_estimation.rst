Feature Tracker with Motion Estimation
======================================

This example demonstrates the capabilities of the :ref:`FeatureTracker` combined with motion estimation. It detects and tracks features between consecutive frames using optical flow. 
Each feature is assigned a unique ID. The motion of the camera is estimated based on the tracked features, and the estimated motion (e.g., Up, Down, Left, Right, Rotating) is displayed on screen. 

The :ref:`Feature Detector` example only detects features without estimating motion.

Demo
####

.. image:: ../../../../docs/source/_static/images/examples/feature_motion_estimation.gif
    :alt: Feature Tracker with Motion Estimation Demo
    :width: 100%
    :align: center


Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/FeatureTracker/feature_motion_estimation.py>`__

        .. literalinclude:: ../../../../examples/FeatureTracker/feature_motion_estimation.py
           :language: python
           :linenos:


.. include::  /includes/footer-short.rst