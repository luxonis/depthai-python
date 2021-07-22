RGB & TinyYolo with spatial data
================================

This example shows how to run TinyYoloV3 and v4 on the RGB input frame, and how to display both the RGB
preview, detections, depth map and spatial information (X,Y,Z). It's similar to example
:ref:`RGB & MobilenetSSD with spatial data` except it is running TinyYolo network.
X,Y,Z coordinates are relative to the center of depth map.


setNumClasses - number of YOLO classes
setCoordinateSize - size of coordinate
setAnchors - yolo anchors
setAnchorMasks - anchorMasks26, anchorMasks13 (anchorMasks52 - additionally for full YOLOv4)
setIouThreshold - intersection over union threshold
setConfidenceThreshold - confidence threshold above which objects are detected

.. rubric:: Similiar samples:

- :ref:`Spatial location calculator`
- :ref:`Spatial object tracker on RGB`
- :ref:`RGB & MobilenetSSD with spatial data`
- :ref:`Mono & MobilenetSSD with spatial data`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/93tG_qBweN8" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

This example also requires YOLOv4-tiny blob (:code:`tiny-yolo-v4_openvino_2021.2_6shave.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v4_openvino_2021.2_6shave.blob>`__

YOLOv3-tiny blob (:code:`tiny-yolo-v3_openvino_2021.2_6shave.blob` file) can be used too - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/tiny-yolo-v3_openvino_2021.2_6shave.blob>`__

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/spatial_tiny_yolo.py>`__

        .. literalinclude:: ../../../examples/spatial_tiny_yolo.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/spatial_tiny_yolo.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/spatial_tiny_yolo.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
