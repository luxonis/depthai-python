24 - OpenCV support
=========================

This example shows API which exposes both numpy and OpenCV compatible image types for eaiser usage.
It uses ColorCamera node to retrieve both BGR interleaved 'preview' and NV12 encoded 'video' frames.
Both are displayed using functions `getFrame` and `getCvFrame`.

Setup
#####

Please run the following command to install the required dependencies


.. code-block:: python
   :substitutions:

   python3 -m pip install -U depthai numpy opencv-python


For additional information, please follow :ref:`Python API installation guide <Installation - Python>`

This example also requires MobilenetSDD blob (:code:`mobilenet.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet.blob>`__

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/24_opencv_support.py>`__

.. literalinclude:: ../../../examples/24_opencv_support.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
