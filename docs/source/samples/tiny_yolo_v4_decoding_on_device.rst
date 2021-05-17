RGB & TinyYoloV4 decoding on device
===================================

This example shows how to run TinyYoloV4 on the RGB input frame, and how to display both the RGB
preview and the metadata results from the TinyYoloV4 on the preview. Decoding is done on Myriad instead on the host.

Configurable, network dependent parameters are required for correct decoding:
setNumClasses - number of YOLO classes
setCoordinateSize - size of coordinate
setAnchors - yolo anchors
setAnchorMasks - anchorMasks26, anchorMasks13 (anchorMasks52 - additionally for full YOLOv4)
setIouThreshold - intersection over union threshold
setConfidenceThreshold - confidence threshold above which objects are detected

.. rubric:: Similiar samples:

- :ref:`RGB & TinyYoloV3 decoding on device`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/l4jDLs9d8GI" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires YOLOv4-tiny blob (:code:`tiny-yolo-v4_openvino_2021.2_6shave.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v4_openvino_2021.2_6shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/tiny_yolo_v4_device_side_decoding.py>`__

        .. literalinclude:: ../../../examples/tiny_yolo_v4_device_side_decoding.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/tiny_yolo_v4_device_side_decoding.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/tiny_yolo_v4_device_side_decoding.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
