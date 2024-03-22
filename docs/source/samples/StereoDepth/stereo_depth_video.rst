Stereo Depth Video
==================

This example is an upgraded :ref:`Depth Preview`. It has higher resolution (720p), each frame can be shown
(mono left-right, rectified left-right, disparity and depth). There are 6 modes which you can select
inside the code:

#. `withDepth`: if you turn it off it will became :ref:`Mono Preview`, so it will show only the 2 mono cameras
#. `outputDepth`: if you turn it on it will show the depth
#. `lrcheck`: used for better occlusion handling. For more information `click here <https://docs.luxonis.com/en/latest/pages/faq/#left-right-check-depth-mode>`__
#. `extended`: suitable for short range objects. For more information `click here <https://docs.luxonis.com/en/latest/pages/faq/#extended-disparity-depth-mode>`__
#. `subpixel`: suitable for long range. For more information `click here <https://docs.luxonis.com/en/latest/pages/faq/#subpixel-disparity-depth-mode>`__

Stereo Alpha Param
##################

With `--alpha` you can select scaling during the rectification. Values between 0 and 1:

- 0.0 (default value) means that the rectified images are zoomed and shifted so that only valid pixels are visible (no black areas after rectification)
- 1.0 means that the rectified image is decimated and shifted so that all the pixels from the original images from the cameras are retained in the rectified images (no source image pixels are lost).

.. image:: https://github.com/luxonis/depthai-python/assets/18037362/887d489a-6bf6-41ac-9b8f-06ab7b0cd488

.. rubric:: Similar samples:

- :ref:`Depth Preview`
- :ref:`Stereo Depth from host`

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/StereoDepth/stereo_depth_video.py>`__

        .. literalinclude:: ../../../../examples/StereoDepth/stereo_depth_video.py
           :language: python
           :linenos:

    .. tab:: C++

        Also `available on GitHub <https://github.com/luxonis/depthai-core/blob/main/examples/StereoDepth/stereo_depth_video.cpp>`__

        .. literalinclude:: ../../../../depthai-core/examples/StereoDepth/stereo_depth_video.cpp
           :language: cpp
           :linenos:

.. include::  /includes/footer-short.rst
