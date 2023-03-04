StereoDepth
###########

StereoDepth node calculates the disparity/depth from the stereo camera pair (2x :ref:`MonoCamera <MonoCamera>`/:ref:`ColorCamera`).

How to place it
===============

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    stereo = pipeline.create(dai.node.StereoDepth)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();


Inputs and Outputs
==================

.. code-block::

                 ┌───────────────────┐
                 │                   │ confidenceMap
                 │                   ├─────────────►
                 │                   │rectifiedLeft
                 │                   ├─────────────►
  left           │                   │   syncedLeft
  ──────────────►│-------------------├─────────────►
                 │                   │   depth [mm]
                 │                   ├─────────────►
                 │    StereoDepth    │    disparity
                 │                   ├─────────────►
  right          │                   │   syncedRight
  ──────────────►│-------------------├─────────────►
                 │                   │rectifiedRight
                 │                   ├─────────────►
  inputConfig    │                   |     outConfig
  ──────────────►│-------------------├─────────────►
                 └───────────────────┘

.. tabs::

  .. tab:: **Inputs**

    - ``left`` - :ref:`ImgFrame` from the left stereo camera
    - ``right`` - :ref:`ImgFrame` from the right stereo camera
    - ``inputConfig`` - :ref:`StereoDepthConfig`

  .. tab:: **Outputs**

    - ``confidenceMap`` - :ref:`ImgFrame`
    - ``rectifiedLeft`` - :ref:`ImgFrame`
    - ``syncedLeft`` - :ref:`ImgFrame`
    - ``depth`` - :ref:`ImgFrame`: UINT16 values - depth in depth units (millimeter by default)
    - ``disparity`` - :ref:`ImgFrame`: UINT8 or UINT16 if Subpixel mode
    - ``rectifiedRight`` - :ref:`ImgFrame`
    - ``syncedRight`` - :ref:`ImgFrame`
    - ``outConfig`` - :ref:`StereoDepthConfig`

  .. tab:: **Debug outputs**

    - ``debugDispLrCheckIt1`` - :ref:`ImgFrame`
    - ``debugDispLrCheckIt2`` - :ref:`ImgFrame`
    - ``debugExtDispLrCheckIt1`` - :ref:`ImgFrame`
    - ``debugExtDispLrCheckIt2`` - :ref:`ImgFrame`
    - ``debugDispCostDump`` - :ref:`ImgFrame`
    - ``confidenceMap`` - :ref:`ImgFrame`

Internal block diagram of StereoDepth node
==========================================

.. image:: /_static/images/components/depth_diagram.png
   :target: https://whimsical.com/stereo-node-EKcfcXGjGpNL6cwRPV6NPv

On the diagram, red rectangle are firmware settings that are configurable via the API. Gray rectangles are settings that that are not yet
exposed to the API. We plan on exposing as much configurability as possible, but please inform us if you would like to see these settings
configurable sooner.

If you click on the image, you will be redirected to the webapp. Some blocks have notes that provide additional technical information.

Currently configurable blocks
-----------------------------

.. tabs::

  .. tab:: Stereo Mode

    .. tabs::

      .. tab:: Left-Right Check

        **Left-Right Check** or LR-Check is used to remove incorrectly calculated disparity pixels due to occlusions at object borders (Left and Right camera views
        are slightly different).

        #. Computes disparity by matching in R->L direction
        #. Computes disparity by matching in L->R direction
        #. Combines results from 1 and 2, running on Shave: each pixel d = disparity_LR(x,y) is compared with disparity_RL(x-d,y). If the difference is above a threshold, the pixel at (x,y) in the final disparity map is invalidated.

        You can use :code:`debugDispLrCheckIt1` and :code:`debugDispLrCheckIt2` debug outputs for debugging/fine-tuning purposes.

      .. tab:: Extended Disparity

        **Extended disparity mode** allows detecting closer distance objects for the given baseline. This increases the maximum disparity search from 96 to 191, meaning the range is now: **[0..190]**.
        So this cuts the minimum perceivable distance in half, given that the minimum distance is now :code:`focal_length * base_line_dist / 190` instead
        of :code:`focal_length * base_line_dist / 95`.

        #. Computes disparity on the original size images (e.g. 1280x720)
        #. Computes disparity on 2x downscaled images (e.g. 640x360)
        #. Combines the two level disparities on Shave, effectively covering a total disparity range of 191 pixels (in relation to the original resolution).

        You can use :code:`debugExtDispLrCheckIt1` and :code:`debugExtDispLrCheckIt2` debug outputs for debugging/fine-tuning purposes.

      .. tab:: Subpixel Disparity

        **Subpixel mode** improves the precision and is especially useful for long range measurements. It also helps for better estimating surface normals.

        Besides the integer disparity output, the Stereo engine is programmed to dump to memory the cost volume, that is 96 levels (disparities) per pixel,
        then software interpolation is done on Shave, resulting a final disparity with 3 fractional bits, resulting in significantly more granular depth
        steps (8 additional steps between the integer-pixel depth steps), and also theoretically, longer-distance depth viewing - as the maximum depth
        is no longer limited by a feature being a full integer pixel-step apart, but rather 1/8 of a pixel. In this mode, stereo cameras perform: :code:`94 depth steps * 8 subpixel depth steps + 2 (min/max values) = 754 depth steps`

        For comparison of normal disparity vs. subpixel disparity images, click `here <https://github.com/luxonis/depthai/issues/184>`__.

  .. tab:: Depth Filters

    **Depth Filtering** / **Depth Post-Processing** is performed at the end of the depth pipeline. It helps with noise reduction and overall depth quality.

    .. include::  ../../includes/depth-filters.rst

  .. tab:: Mesh files

    Mesh files (homography matrix) are generated using the camera intrinsics, distortion coeffs, and rectification rotations.
    These files helps in overcoming the distortions in the camera increasing the accuracy and also help in when `wide FOV <https://docs.luxonis.com/projects/hardware/en/latest/pages/arducam.html#arducam-compatible-cameras>`__ lens are used.

    .. note::
      Currently mesh files are generated only for stereo cameras on the host during calibration. The generated mesh files are stored in `depthai/resources <https://github.com/luxonis/depthai/tree/main/resources>`__ which users can load to the device. This process will be moved to on device in the upcoming releases.

    .. doxygenfunction:: dai::node::StereoDepth::loadMeshFiles
        :project: depthai-core
        :no-link:

    .. doxygenfunction:: dai::node::StereoDepth::loadMeshData
        :project: depthai-core
        :no-link:

    .. doxygenfunction:: dai::node::StereoDepth::setMeshStep
        :project: depthai-core
        :no-link:

  .. tab:: Confidence Threshold

    - **Confidence threshold**: Stereo depth algorithm searches for the matching feature from right camera point to the left image (along the 96 disparity levels). During this process it computes the cost for each disparity level and chooses the minimal cost between two disparities and uses it to compute the confidence at each pixel. Stereo node will output disparity/depth pixels only where depth confidence is below the **confidence threshold** (lower the confidence value means better depth accuracy).
    - **LR check threshold**: Disparity is considered for the output when the difference between LR and RL disparities is smaller than the LR check threshold.

    .. doxygenfunction:: dai::StereoDepthConfig::setConfidenceThreshold
        :project: depthai-core
        :no-link:

    .. doxygenfunction:: dai::StereoDepthConfig::setLeftRightCheckThreshold
        :project: depthai-core
        :no-link:

Limitations
===========

- Median filtering is disabled when subpixel mode is set to 4 or 5 bits.
- For RGB-depth alignment the RGB camera has to be placed on the same horizontal line as the stereo camera pair.

Stereo depth FPS
================

.. list-table::
   :header-rows: 1

   * - Stereo depth mode
     - FPS for 1280x720
     - FPS for 640x400
   * - Standard mode
     - 60
     - 110
   * - Left-Right Check
     - 55
     - 105
   * - Subpixel Disparity
     - 45
     - 105
   * - Extended Disparity
     - 54
     - 105
   * - Subpixel + LR check
     - 34
     - 96
   * - Extended + LR check
     - 26
     - 62

All stereo modes were measured for :code:`depth` output with **5x5 median filter** enabled. For 720P, mono cameras were set
to **60 FPS** and for 400P mono cameras were set to **110 FPS**.

Usage
=====

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    stereo = pipeline.create(dai.node.StereoDepth)

    # Better handling for occlusions:
    stereo.setLeftRightCheck(False)
    # Closer-in minimum depth, disparity range is doubled:
    stereo.setExtendedDisparity(False)
    # Better accuracy for longer distance, fractional disparity 32-levels:
    stereo.setSubpixel(False)

    # Define and configure MonoCamera nodes beforehand
    left.out.link(stereo.left)
    right.out.link(stereo.right)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();

    // Better handling for occlusions:
    stereo->setLeftRightCheck(false);
    // Closer-in minimum depth, disparity range is doubled:
    stereo->setExtendedDisparity(false);
    // Better accuracy for longer distance, fractional disparity 32-levels:
    stereo->setSubpixel(false);

    // Define and configure MonoCamera nodes beforehand
    left->out.link(stereo->left);
    right->out.link(stereo->right);

Examples of functionality
=========================

- :ref:`Depth Preview`
- :ref:`RGB Depth alignment` - align depth to color camera
- :ref:`Mono & MobilenetSSD & Depth`
- :ref:`RGB & MobilenetSSD with spatial data`

Reference
=========

.. tabs::

  .. tab:: Python

    .. autoclass:: depthai.node.StereoDepth
      :members:
      :inherited-members:
      :noindex:

  .. tab:: C++

    .. doxygenclass:: dai::node::StereoDepth
      :project: depthai-core
      :members:
      :private-members:
      :undoc-members:

Disparity
=========

Disparity refers to the distance between two corresponding points in the left and right image of a stereo pair.

.. image:: /_static/images/components/disparity_explanation.jpeg
   :target: https://stackoverflow.com/a/17620159

When calculating the disparity, each pixel in the disparity map gets assigned a confidence value :code:`0..255` by the stereo matching algorithm,
as:

- :code:`0` - maximum confidence that it holds a valid value
- :code:`255` - minimum confidence, so there is more chance that the value is incorrect

(this confidence score is kind-of inverted, if say comparing with NN)

For the final disparity map, a filtering is applied based on the confidence threshold value: the pixels that have their confidence score larger than
the threshold get invalidated, i.e. their disparity value is set to zero. You can set the confidence threshold with :code:`stereo.initialConfig.setConfidenceThreshold()`.


Min stereo depth distance
=========================

If the depth results for close-in objects look weird, this is likely because they are below the minimum depth-perception distance of the device.

To calculate this minimum distance, use the :ref:`depth formula <Calculate depth using disparity map>` and choose the maximum value for disparity_in_pixels parameter (keep in mind it is inveresly related, so maximum value will yield the smallest result).

For example OAK-D has a baseline of **7.5cm**, focal_length_in_pixels of **882.5 pixels** and the default maximum value for disparity_in_pixels is **95**. By using the :ref:`depth formula <Calculate depth using disparity map>` we get:

.. code-block:: python

  min_distance = focal_length_in_pixels * baseline / disparity_in_pixels = 882.5 * 7.5cm / 95 = 69.67cm

or roughly 70cm.

However this distance can be cut in 1/2 (to around 35cm for the OAK-D) with the following options:

1. Changing the resolution to 640x400, instead of the standard 1280x800.

2. Enabling Extended Disparity.

Extended Disparity mode increases the levels of disparity to 191 from the standard 96 pixels, thereby 1/2-ing the minimum depth. It does so by computing the 96-pixel disparities on the original 1280x720 and on the downscaled 640x360 image, which are then merged to a 191-level disparity. For more information see the Extended Disparity tab in :ref:`this table <Currently configurable blocks>`.

Using the previous OAK-D example, disparity_in_pixels now becomes **190** and the minimum distance is:

.. code-block:: python

  min_distance = focal_length_in_pixels * baseline / disparity_in_pixels = 882.5 * 7.5cm / 190 = 34.84cm

or roughly 35cm.

.. note::

    Applying both of those options is possible, which would set the minimum depth to 1/4 of the standard settings, but at such short distances the minimum depth is limited by focal length, which is 19.6cm, since OAK-D mono cameras have fixed focus distance: 19.6cm - infinity.

See `these examples <https://github.com/luxonis/depthai-experiments/tree/master/gen2-camera-demo#real-time-depth-from-depthai-stereo-pair>`__ for how to enable Extended Disparity.

Max stereo depth distance
=========================

The maximum depth perception distance depends on the :ref:`accuracy of the depth perception <Depth perception accuracy>`. The formula used to calculate this distance is an approximation, but is as follows:

.. code-block:: python

  Dm = (baseline/2) * tan((90 - HFOV / HPixels)*pi/180)

So using this formula for existing models the *theoretical* max distance is:

.. code-block:: python

  # For OAK-D (7.5cm baseline)
  Dm = (7.5/2) * tan((90 - 71.9/1280)*pi/180) = 3825.03cm = 38.25 meters
  
  # For OAK-D-CM4 (9cm baseline)
  Dm = (9/2) * tan((90 - 71.9/1280)*pi/180) = 4590.04cm = 45.9 meters

If greater precision for long range measurements is required, consider enabling Subpixel Disparity or using a larger baseline distance between mono cameras. For a custom baseline, you could consider using `OAK-FFC <https://docs.luxonis.com/projects/hardware/en/latest/pages/DM1090.html>`__ device or design your own baseboard PCB with required baseline. For more information see Subpixel Disparity under the Stereo Mode tab in :ref:`this table <Currently configurable blocks>`.


Measuring real-world object dimensions
======================================

Because the depth map contains the Z distance, objects in parallel with the camera are measured accurately standard. For objects not in parallel, the Euclidean distance calculation can be used. Please refer to the below:

.. image:: /_static/images/components/Euclidian_distance_fig.png

When running eg. the :ref:`RGB & MobilenetSSD with spatial data` example, you could calculate the distance to the detected object from XYZ coordinates (:ref:`SpatialImgDetections`) using the code below (after code line ``143`` of the example):

.. code-block:: python

    distance = math.sqrt(detection.spatialCoordinates.x ** 2 + detection.spatialCoordinates.y ** 2 + detection.spatialCoordinates.z ** 2) # mm


.. include::  ../../includes/footer-short.rst
