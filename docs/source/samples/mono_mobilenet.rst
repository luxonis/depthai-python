Mono & MobilenetSSD
===================

This example shows how to run MobileNetv2SSD on the right grayscale camera and how to display the
neural network results on a preview of the right camera stream.

.. rubric:: Similiar samples:

- :ref:`RGB & MobilenetSSD`
- :ref:`RGB & MobileNetSSD @ 4K`
- :ref:`Video & MobilenetSSD`
- :ref:`Mono & MobilenetSSD & Depth`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/8Jih63NWFdI" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires MobilenetSDD blob (:code:`mobilenet-ssd_openvino_2021.2_6shave.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet-ssd_openvino_2021.2_6shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/mono_mobilenet.py>`__

        .. literalinclude:: ../../../examples/mono_mobilenet.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/mono_mobilenet.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/mono_mobilenet.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
