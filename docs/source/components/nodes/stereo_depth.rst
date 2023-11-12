StereoDepth
###########

StereoDepth node calculates the disparity and/or depth from the stereo camera pair (2x :ref:`MonoCamera <MonoCamera>`/:ref:`ColorCamera`).
We suggest following :ref:`Configuring Stereo Depth` tutorial to achieve the best depth results.

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
- RGB-depth alignment doesn't work when using disparity shift.

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

Calculate depth using disparity map
===================================

Disparity and depth are inversely related. As disparity decreases, depth increases exponentially depending on baseline and focal length. Meaning, if the disparity value is close to zero, then a small change in disparity generates a large change in depth. Similarly, if the disparity value is big, then large changes in disparity do not lead to a large change in depth.

By considering this fact, depth can be calculated using this formula:

.. code-block:: python

  depth = focal_length_in_pixels * baseline / disparity_in_pixels

Where baseline is the distance between two mono cameras. Note the unit used for baseline and depth is the same.

To get focal length in pixels, you can :ref:`read camera calibration <Calibration Reader>`, as focal length in pixels is
written in camera intrinsics (``intrinsics[0][0]``):

.. code-block:: python

  import depthai as dai

  with dai.Device() as device:
    calibData = device.readCalibration()
    intrinsics = calibData.getCameraIntrinsics(dai.CameraBoardSocket.CAM_C)
    print('Right mono camera focal length in pixels:', intrinsics[0][0])

Here's theoretical calculation of the focal length in pixels:

.. code-block:: python

  focal_length_in_pixels = image_width_in_pixels * 0.5 / tan(HFOV * 0.5 * PI/180)

  # With 400P mono camera resolution where HFOV=71.9 degrees
  focal_length_in_pixels = 640 * 0.5 / tan(71.9 * 0.5 * PI / 180) = 441.25

  # With 800P mono camera resolution where HFOV=71.9 degrees
  focal_length_in_pixels = 1280 * 0.5 / tan(71.9 * 0.5 * PI / 180) = 882.5

Examples for calculating the depth value, using the OAK-D (7.5cm baseline):

.. code-block:: python

  # For OAK-D @ 400P mono cameras and disparity of eg. 50 pixels
  depth = 441.25 * 7.5 / 50 = 66.19 # cm

  # For OAK-D @ 800P mono cameras and disparity of eg. 10 pixels
  depth = 882.5 * 7.5 / 10 = 661.88 # cm

Note the value of disparity depth data is stored in :code:`uint16`, where 0 is a special value, meaning that distance is unknown.

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

Disparity shift to lower min depth perception
---------------------------------------------

Another option to perceive closer depth range is to use disparity shift. Disparity shift will shift the starting point
of the disparity search, which will significantly decrease max depth (MazZ) perception, but it will also decrease min depth (MinZ) perception.
Disparity shift can be combined with extended/subpixel/LR-check modes.

.. image:: https://user-images.githubusercontent.com/18037362/189375017-2fa137d2-ad6b-46de-8899-6304bbc6c9d7.png

**Left graph** shows min and max disparity and depth for OAK-D (7.5cm baseline, 800P resolution, ~70° HFOV) by default (disparity shift=0). See :ref:`Calculate depth using disparity map`.
Since hardware (stereo block) has a fixed 95 pixel disparity search, DepthAI will search from 0 pixels (depth=INF) to 95 pixels (depth=71cm).

**Right graph** shows the same, but at disparity shift set to 30 pixels. This means that disparity search will be from 30 pixels (depth=2.2m) to 125 pixels (depth=50cm).
This also means that depth will be very accurate at the short range (**theoretically** below 5mm depth error).

**Limitations**:

- Because of the inverse relationship between disparity and depth, MaxZ will decrease much faster than MinZ as the disparity shift is increased. Therefore, it is **advised not to use a larger than necessary disparity shift**.
- Tradeoff in reducing the MinZ this way is that objects at **distances farther away than MaxZ will not be seen**.
- Because of the point above, **we only recommend using disparity shift when MaxZ is known**, such as having a depth camera mounted above a table pointing down at the table surface.
- Output disparity map is not expanded, only the depth map. So if disparity shift is set to 50, and disparity value obtained is 90, the real disparity is 140.

**Compared to Extended disparity**, disparity shift:

- **(+)** Is faster, as it doesn't require an extra computation, which means there's also no extra latency
- **(-)** Reduces the MaxZ (significantly), while extended disparity only reduces MinZ.

Disparity shift can be combined with extended disparity.

.. doxygenfunction:: dai::StereoDepthConfig::setDisparityShift
  :project: depthai-core
  :no-link:

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

Depth perception accuracy
=========================

Disparity depth works by matching features from one image to the other and its accuracy is based on multiple parameters:

* Texture of objects / backgrounds

Backgrounds may interfere with the object detection, since backgrounds are objects too, which will make depth perception less accurate. So disparity depth works very well outdoors as there are very rarely perfectly-clean/blank surfaces there - but these are relatively commonplace indoors (in clean buildings at least).

* Lighting

If the illumination is low, the diparity map will be of low confidence, which will result in a noisy depth map.

* Baseline / distance to objects

Lower baseline enables us to detect the depth at a closer distance as long as the object is visible in both the frames. However, this reduces the accuracy for large distances due to less pixels representing the object and disparity decreasing towards 0 much faster.
So the common norm is to adjust the baseline according to how far/close we want to be able to detect objects.

Limitation
==========

Since depth is calculated from disparity, which requires the pixels to overlap, there is inherently a vertical
band on the left side of the left mono camera and on the right side of the right mono camera, where depth
cannot be calculated, since it is seen by only 1 camera. That band is marked with :code:`B`
on the following picture.

.. image:: https://user-images.githubusercontent.com/59799831/135310921-67726c28-07e7-4ffa-bc8d-74861049517e.png

Meaning of variables on the picture:

- ``BL [cm]`` - Baseline of stereo cameras.
- ``Dv [cm]`` - Minimum distace where both cameras see an object (thus where depth can be calculated).
- ``B [pixels]`` - Width of the band where depth cannot be calculated.
- ``W [pixels]`` - Width of mono in pixels camera or amount of horizontal pixels, also noted as :code:`HPixels` in other formulas.
- ``D [cm]`` - Distance from the **camera plane** to an object (see image :ref:`here <Measuring real-world object dimensions>`).
- ``F [cm]`` - Width of image at the distance ``D``.

.. image:: https://user-images.githubusercontent.com/59799831/135310972-c37ba40b-20ad-4967-92a7-c71078bcef99.png

With the use of the :code:`tan` function, the following formulas can be obtained:

- :code:`F = 2 * D * tan(HFOV/2)`
- :code:`Dv = (BL/2) * tan(90 - HFOV/2)`

In order to obtain :code:`B`, we can use :code:`tan` function again (same as for :code:`F`), but this time
we must also multiply it by the ratio between :code:`W` and :code:`F` in order to convert units to pixels.
That gives the following formula:

.. code-block:: python

  B = 2 * Dv * tan(HFOV/2) * W / F
  B = 2 * Dv * tan(HFOV/2) * W / (2 * D * tan(HFOV/2))
  B = W * Dv / D  # pixels

Example: If we are using OAK-D, which has a :code:`HFOV` of 72°, a baseline (:code:`BL`) of 7.5 cm and
:code:`640x400 (400P)` resolution is used, therefore :code:`W = 640` and an object is :code:`D = 100` cm away, we can
calculate :code:`B` in the following way:

.. code-block::

  Dv = 7.5 / 2 * tan(90 - 72/2) = 3.75 * tan(54°) = 5.16 cm
  B = 640 * 5.16 / 100 = 33 # pixels

Credit for calculations and images goes to our community member gregflurry, which he made on
`this <https://discuss.luxonis.com/d/339-naive-question-regarding-stereodepth-disparity-and-depth-outputs/7>`__
forum post.

.. note::

   OAK-D-PRO will include both IR dot projector and IR LED, which will enable operation in no light.
   IR LED is used to illuminate the whole area (for mono/color frames), while IR dot projector is mostly
   for accurate disparity matching - to have good quality depth maps on blank surfaces as well. For outdoors,
   the IR laser dot projector is only relevant at night. For more information see the development progress
   `here <https://github.com/luxonis/depthai-hardware/issues/114>`__.

Measuring real-world object dimensions
======================================

Because the depth map contains the Z distance, objects in parallel with the camera are measured accurately standard. For objects not in parallel, the Euclidean distance calculation can be used. Please refer to the below:

.. image:: /_static/images/components/Euclidian_distance_fig.png

When running eg. the :ref:`RGB & MobilenetSSD with spatial data` example, you could calculate the distance to the detected object from XYZ coordinates (:ref:`SpatialImgDetections`) using the code below (after code line ``143`` of the example):

.. code-block:: python

    distance = math.sqrt(detection.spatialCoordinates.x ** 2 + detection.spatialCoordinates.y ** 2 + detection.spatialCoordinates.z ** 2) # mm


.. include::  ../../includes/footer-short.rst
