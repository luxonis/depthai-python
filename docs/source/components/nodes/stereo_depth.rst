StereoDepth
===========

Stereo depth node calculates the disparity/depth from two :ref:`mono cameras <MonoCamera>`.

How to place it
###############

.. tabs::

  .. code-tab:: py

    pipeline = dai.Pipeline()
    stereo = pipeline.create(dai.node.StereoDepth)

  .. code-tab:: c++

    dai::Pipeline pipeline;
    auto stereo = pipeline.create<dai::node::StereoDepth>();


Inputs and Outputs
##################

.. code-block::

                 ┌───────────────────┐
                 │                   │rectifiedLeft
                 │                   ├─────────────►
  left           │                   │   syncedLeft
  ──────────────►│                   ├─────────────►
                 │                   │        depth
                 │                   ├─────────────►
                 │    StereoDepth    │    disparity
                 │                   ├─────────────►
  right          │                   │rectifiedRight
  ──────────────►│                   ├─────────────►
                 │                   │   syncedRight
                 │                   ├─────────────►
                 └───────────────────┘

**Message types**

- :code:`left` - :ref:`ImgFrame` from the left :ref:`MonoCamera`
- :code:`right` - :ref:`ImgFrame` from the right :ref:`MonoCamera`
- :code:`rectifiedLeft` - :ref:`ImgFrame`
- :code:`syncedLeft` - :ref:`ImgFrame`
- :code:`depth` - :ref:`ImgFrame`
- :code:`disparity` - :ref:`ImgFrame`
- :code:`rectifiedRight` - :ref:`ImgFrame`
- :code:`syncedRight` - :ref:`ImgFrame`

Disparity
#########

When calculating the disparity, each pixel in the disparity map gets assigned a confidence value :code:`0..255` by the stereo matching algorithm,
as:

- :code:`0` - maximum confidence that it holds a valid value
- :code:`255` - minimum confidence, so there is more chance that the value is incorrect

(this confidence score is kind-of inverted, if say comparing with NN)

For the final disparity map, a filtering is applied based on the confidence threshold value: the pixels that have their confidence score larger than
the threshold get invalidated, i.e. their disparity value is set to zero. You can set the confidence threshold with :code:`stereo.initialConfig.setConfidenceThreshold()`.

Max Stereo Disparity Depth Resolution
#####################################

DepthAI has either a 96-pixel (default) or 191-pixel disparity search (when Extended Disparity is enabled) and either a full-pixel (default) or sub-pixel matching with precision of 32 sub-pixel steps (when Sub-Pixel Disparity is enabled), resulting in a maximum theoretical depth precision of:
:code:`191 * 32 = 6,112`.
However sub-pixel and extended disparity are not yet supported simultaneously, but should be available in the near future (`Pull Request <https://github.com/luxonis/depthai-python/pull/347>`__), so the maximum depth precision is currently 3,072 depth steps.  More information on the disparity depth modes are below:

#. Default (96-pixel disparity search): 96 depth steps
#. Extended Disparity (191-pixel disparity search): 191 depth steps
#. Subpixel Disparity (32 sub-pixel steps): 96 depth steps * 32 subpixel depth steps = 3,072 depth steps.
#. LR-Check Disparity: with disparity run in both directions for allowing recentering of the depth.

See :ref:`this <Currently configurable blocks>` table for more information about stereo modes.

Enable depthai to perceive closer distances
###########################################

If the depth results for close-in objects look weird, this is likely because they are below the minimum depth-perception distance of the device.

Minimum depth-perception distance can be cut in 1/2 and 1/4 by enabling `Extended Disparity <https://docs.luxonis.com/en/latest/pages/faq/#extended-disparity>`__ , which extends the disparity search to 191 pixels from the standard 96 pixels.

Note that minimum depth-perception distance may vary depending on the device, so please refer to documentation of your specific device for more information. For OAK-D, the standard-settings minimum depth is around 70cm at 800P resolution of mono cameras.

See `these examples <https://github.com/luxonis/depthai-experiments/tree/master/gen2-camera-demo#real-time-depth-from-depthai-stereo-pair>`__ for how to enable Extended Disparity.

Current limitations
###################

If one or more of the additional depth modes (:code:`lrcheck`, :code:`extended`, :code:`subpixel`) are enabled, then:

- median filtering is disabled on device
- with subpixel, if both :code:`depth` and :code:`disparity` are used, only :code:`depth` will have valid output

Otherwise, :code:`depth` output is **U16** (in millimeters) and median is functional.

Stereo depth FPS
################

.. list-table::
   :header-rows: 1

   * - Stereo depth mode
     - FPS for 720P
   * - Standard mode
     - 150
   * - Left-Right Check
     - 60
   * - Subpixel Disparity
     - 30
   * - Extended Disparity
     - 60
   * - Subpixel + LR check
     - 15
   * - Extended + LR check
     - 30

Internal block diagram of StereoDepth node
##########################################

.. image:: /_static/images/components/depth_diagram.jpeg
   :target: https://whimsical.com/stereo-node-EKcfcXGjGpNL6cwRPV6NPv

On the diagram, red rectangle are firmware settings that are configurable via the API. Gray rectangles are settings that that are not yet
exposed to the API. We plan on exposing as much configurability as possible, but please inform us if you would like to see these settings
configurable sooner.

If you click on the image, you will be redirected to the webapp. Some blocks have notes that provide additional technical information.

Currently configurable blocks
*****************************

.. tabs::

  .. tab:: Stereo Mode

    .. tabs::

      .. tab:: Left-Right Check

        Left-Right Check or LR-Check is used to remove incorrectly calculated disparity pixels due to occlusions at object borders (Left and Right camera views
        are slightly different).

        #. Computes disparity by matching in R->L direction
        #. Computes disparity by matching in L->R direction
        #. Combines results from 1 and 2, running on Shave: each pixel d = disparity_LR(x,y) is compared with disparity_RL(x-d,y). If the difference is above a threshold, the pixel at (x,y) in the final disparity map is invalidated.

      .. tab:: Extended Disparity

        The :code:`extended disparity` allows detecting closer distance objects for the given baseline. This increases the maximum disparity search from 96 to 191.
        So this cuts the minimum perceivable distance in half, given that the minimum distance is now :code:`focal_length * base_line_dist / 190` instead
        of :code:`focal_length * base_line_dist / 95`.

        #. Computes disparity on the original size images (e.g. 1280x720)
        #. Computes disparity on 2x downscaled images (e.g. 640x360)
        #. Combines the two level disparities on Shave, effectively covering a total disparity range of 191 pixels (in relation to the original resolution).

      .. tab:: Subpixel Disparity

        Subpixel improves the precision and is especially useful for long range measurements. It also helps for better estimating surface normals

        Besides the integer disparity output, the Stereo engine is programmed to dump to memory the cost volume, that is 96 levels (disparities) per pixel,
        then software interpolation is done on Shave, resulting a final disparity with 5 fractional bits, resulting in significantly more granular depth
        steps (32 additional steps between the integer-pixel depth steps), and also theoretically, longer-distance depth viewing - as the maximum depth
        is no longer limited by a feature being a full integer pixel-step apart, but rather 1/32 of a pixel.

        For comparison of normal disparity vs. subpixel disparity images, click `here <https://github.com/luxonis/depthai/issues/184>`__.

  .. tab:: Mesh file / Homography matrix

    Mesh files are generated using the camera intrinsics, distortion coeffs, and rectification rotations.
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

    - **Confidence threshold**: Stereo depth algorithm searches for the matching feature from right camera point to the left image (along the 96 dispairty levels). During this process it computes the cost for each disparity level and choses the minimal cost between two disparities and uses it to compute the confidence at each pixel. Stereo node will output disparity/depth pixels only where depth confidence is below the **confidence threshold** (lower the confidence value means better depth accuracy). Note: This threshold only applies to Normal stereo mode as of now.
    - **LR check threshold**: Disparity is considered for the output when the difference between LR and RL disparities is smaller than the LR check threshold.

    .. doxygenfunction:: dai::StereoDepthConfig::setConfidenceThreshold
        :project: depthai-core
        :no-link:

    .. doxygenfunction:: dai::StereoDepthConfig::setLeftRightCheckThreshold
        :project: depthai-core
        :no-link:


Usage
#####

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
#########################

- :ref:`Depth Preview`
- :ref:`Mono & MobilenetSSD & Depth`
- :ref:`RGB & MobilenetSSD with spatial data`

Reference
#########

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

.. include::  ../../includes/footer-short.rst
