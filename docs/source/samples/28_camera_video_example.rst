28 - Camera video high resolution
=========================

This example shows how to use high resolution video at low latency. Compared to `01_rbg_preview`, this demo outputs NV12 frames whereas
preview frames are BGR and are not suited for larger resoulution (eg. 2000x1000). Prevuew is more suitable for either NN or visualization purposes.

Setup
#####

Please run the following command to install the required dependencies


.. code-block:: bash
  :substitutions:

  python3 -m pip install --extra-index-url https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/ depthai==|release|


For additional information, please follow :ref:`Python API installation guide <Installation - Python>`

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/develop/examples/28_camera_video_example.py>`__

.. literalinclude:: ../../../examples/28_camera_video_example.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst