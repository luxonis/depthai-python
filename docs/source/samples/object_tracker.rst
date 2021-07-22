Object tracker on RGB
=====================

This example shows how to run MobileNetv2SSD on the RGB input frame, and perform object tracking on persons.

.. rubric:: Similiar samples:

- :ref:`Object tracker on video`
- :ref:`Spatial object tracker on RGB`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/Oor-8O_jZm8" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>


Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires MobilenetSDD blob (:code:`mobilenet.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_6shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/object_tracker.py>`__

        .. literalinclude:: ../../../examples/object_tracker.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/object_tracker.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/object_tracker.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
