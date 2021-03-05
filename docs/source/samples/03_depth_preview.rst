03 - Depth Preview
==================

This example shows how to set the SGBM (semi-global-matching) disparity-depth node, connects
over XLink to transfer the results to the host real-time, and displays the depth map in OpenCV.
Note that disparity is used in this case, as it colorizes in a more intuitive way.
Below is also a preview of using different median filters side-by-side on a depth image.

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/7oDjG-s-88Y" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

**Filtering depth using median filter**

.. raw:: html

    <div style="position: relative; padding-top: 10px;padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <img src="https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/images/depth-demo-filters.png" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;">
    </div>

Setup
#####

Please run the following command to install the required dependencies

.. code-block:: bash
  :substitutions:

  python3 -m pip install --extra-index-url https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/ depthai==|release| numpy==1.19.5 opencv-python==4.5.1.48


For additional information, please follow :ref:`Python API installation guide <Installation - Python>`

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/gen2_develop/examples/03_depth_preview.py>`__

.. literalinclude:: ../../../examples/03_depth_preview.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
