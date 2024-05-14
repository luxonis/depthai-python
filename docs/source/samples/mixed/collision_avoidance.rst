Collision avoidance
===================

This example demonstrates how to use DepthAI to implement a collision avoidance system with the OAK-D camera. The script measures objects distance from the camera in real-time, displaying warnings based on predefined distance thresholds.

The script uses stereo cameras to calculate the distance of objects from the camera. The depth map is then aligned to center (color) camera in order to overlay the distance information on the color frame. 

User-defined constants **`WARNING`** and **`CRITICAL`** are used to define distance thresholds for orange and red alerts respectively.

Similar examples
################ 

- :ref:`Spatial Location Calculator`
- :ref:`RGB Depth Alignment`

Demo
####

.. image:: ../../_static/images/examples/collision_avoidance.gif
    :width: 100%
    :alt: Collision Avoidance

Setup
#####

.. include::  /includes/install_from_pypi.rst

.. include:: /includes/install_req.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/collision_avoidance.py>`__

        .. literalinclude:: ../../../../examples/collision_avoidance.py
           :language: python
           :linenos:

.. include::  /includes/footer-short.rst
