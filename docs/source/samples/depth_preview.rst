Depth Preview
=============

This example shows how to set the SGBM (semi-global-matching) disparity-depth node, connects
over XLink to transfer the results to the host real-time, and displays the depth map in OpenCV.
Note that disparity is used in this case, as it colorizes in a more intuitive way.
Below is also a preview of using different median filters side-by-side on a depth image.
There are 3 depth modes which you can select inside the code:
#. lr_check: computes disparity with sub-pixel interpolation (5 fractional bits), suitable for long range
#. extended_disparity: mirror rectified frames: true to have disparity/depth normal (non-mirrored)
#. subpixel: suitable for short range objects

.. rubric:: Similiar samples:

- :ref:`RGB Preview`
- :ref:`Mono Preview`
- :ref:`Stereo Depth Video`

Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/4iiyxAj4TRk" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

**Filtering depth using median filter**

.. raw:: html

    <div style="position: relative; padding-top: 10px;padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <img src="https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/images/depth-demo-filters.png" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;">
    </div>

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/depth_preview.py>`__

        .. literalinclude:: ../../../examples/depth_preview.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/src/depth_preview.cpp>`__

        .. literalinclude:: ../../../depthai-core/examples/src/depth_preview.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
