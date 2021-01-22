10 - RGB & Encoding & Mono & MobilenetSSD & Depth
=================================================

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/Yeuzo1H24KQ" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

Setup
#####

Please run the following command to install the required dependencies

.. code-block:: bash

  python3 -m pip install --extra-index-url https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/ depthai==0.0.2.1+6ec3f3181b4e46fa6a9f9b20a5b4a3dac5e876b4 numpy==1.19.5 opencv-python==4.5.1.48

For additional information, please follow :ref:`Python API installation guide <Installation - Python>`

This example also requires MobilenetSDD blob (:code:`mobilenet.blob` file) to work - you can download it from
`here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet.blob>`__

Source code
###########

Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/gen2_develop/examples/12_rgb_encoding_mono_mobilenet_depth.py>`__

.. literalinclude:: ../../../examples/12_rgb_encoding_mono_mobilenet_depth.py
   :language: python
   :linenos:

.. include::  /includes/footer-short.rst
