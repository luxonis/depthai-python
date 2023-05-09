Stereo Depth custom Mesh
========================

This example shows how you can load custom mesh to the device and use it for depth calculation.

By default, :ref:`StereoDepth` will use the same logic as inside the ``def getMesh()`` to calculate
mesh files whenever horizontal FOV is larger than 90°. You could also force calculate the mesh using:

.. code-block:: python

    stereo = pipeline.create(dai.node.StereoDepth)
    # Enable mesh calculation to correct distortion:
    stereo.enableDistortionCorrection(True)


StereoDepth node also allows you to load mesh files directly from a file path:

.. code-block:: python

    stereo = pipeline.create(dai.node.StereoDepth)
    stereo.loadMeshFiles('path/to/left_mesh', 'path/to/right_mesh')

Setup
#####

.. include::  /includes/install_from_pypi.rst

Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/StereoDepth/stereo_depth_custom_mesh.py>`__

        .. literalinclude:: ../../../../examples/StereoDepth/stereo_depth_custom_mesh.py
           :language: python
           :linenos:

.. include::  /includes/footer-short.rst
