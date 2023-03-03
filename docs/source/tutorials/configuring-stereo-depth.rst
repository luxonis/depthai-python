Configuring Stereo Depth
########################

Our :ref:`StereoDepth node <StereoDepth>` is very configurable and with this tutorial we will go over some **configurations and troubleshooting**
you can do to get the best results.

This documentation is divided into 6 chapters:

- :ref:`1. Stereo Depth Basics`
- :ref:`2. Fixing noisy depth`
- :ref:`3. Improving depth accuracy`
- :ref:`4. Short range stereo depth`
- :ref:`5. Long range stereo depth`
- :ref:`6. Fixing noisy pointcloud`

1. Stereo Depth Basics
**********************

`Stereo depth vision <https://en.wikipedia.org/wiki/Computer_stereo_vision>`__ works by calculating the disparity between two images taken from
slightly different points.

Stereo vision works a lot like our eyes. Our brains (subconsciously) estimate the depth of objects and scenes based on the difference of what our left eye sees
versus what our right eye sees. On OAK-D cameras, it's exactly the same; we have left and right camera (of the stereo camera pair)
and the OAK does on-device disparity matching to estimate the depth of objects and scenes.

Disparity refers to the distance between two corresponding points in the left and right image of a stereo pair.

.. image:: /_static/images/components/disparity_explanation.jpeg

Depth from disparity
--------------------

Let's first look at how the depth is calculated:

.. math::

   depth [mm] = focal_length [pixels] * baseline [mm] / disparity [pixels]


`RVC2 <https://docs.luxonis.com/projects/hardware/en/latest/pages/rvc/rvc2.html>`__-based cameras have a **0..95 disparity search** range,
which limits the minimal depth perception. Baseline is the distance between two cameras of the
stereo camera pair. You can read camera's focal length (in pixels) from calibration, see :ref:`tutorial here <Camera intrinsics>`

Here's a graph showing disparity vs depth for OAK-D (7.5cm baseline distance) at 800P:

.. figure:: /_static/images/components/disp_to_depth.jpg

    `Full chart here <https://docs.google.com/spreadsheets/d/1ymn-0D4HcCbzYP-iPycj_PIdSwmrLenlGryuZDyA4rQ/edit#gid=0>`__

Note the value of disparity depth data is stored in *uint16*, where 0 means that the distance is invalid/unknown.

2. Fixing noisy depth
*********************

A few topics we have noticed that are relevant for stereo depth quality are:

- :ref:`Scene Texture`
- :ref:`Stereo depth confidence threshold`
- :ref:`Stereo camera pair noise`
- :ref:`Stereo postprocessing filters`

Scene Texture
-------------

Due to the way the stereo matching algorithm works, **passive stereo depth requires** to have a **good texture** in the scene, otherwise the depth will be noisy/invalid.
low-visual-interest surfaces (blank surfaces with little to no texture), such as a wall or floor.

**Solution**

Our `Pro version <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/oak-s2.html#pro-version>`__ of OAK cameras have onboard **IR laser dot projector**,
which projects thousands of little dots on the scene, which helps the stereo matching algorithm as it provides more texture to the scene.

..
    .. image:: dot projector vs no dot projector gif

The technique that we use is called ASV (`Conventional Active Stereo Vision <https://en.wikipedia.org/wiki/Computer_stereo_vision#Conventional_active_stereo_vision_(ASV)>`__)
as stereo matching is performed on the device the same way as on a passive stereo OAK-D.


Stereo depth confidence threshold
---------------------------------

When calculating the disparity, each pixel in the disparity map gets assigned a confidence value :code:`0..255` by the stereo matching algorithm.
This confidence score is kind-of inverted (if say comparing with NN):

- **0** - maximum confidence that it holds a valid value
- **255** - minimum confidence, so there is more chance that the value is incorrect

For the final disparity map, a filtering is applied based on the confidence threshold value: the pixels that have their confidence score larger than
the threshold get invalidated, i.e. their disparity value is set to zero. You can set the confidence threshold via the API below.

This means that with the confidence threshold users can prioritize **fill-rate or accuracy**.

.. code-block:: python

    stereo_depth = pipeline.create(dai.node.StereoDepth)
    stereo_depth.initialConfig.setConfidenceThreshold(int)

    # Or, alternatively, set the Stereo Preset Mode:
    # Prioritize fill-rate, sets Confidence threshold to 245
    stereo_depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
    # Prioritize accuracy, sets Confidence threshold to 200
    stereo_depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_ACCURACY)


..
    .. image:: gif of changing threshold, and how fill-rate/accuracy changes

Stereo camera pair noise
------------------------

If input left/right images are noisy, the disparity map will be noisy as well. So prerequisite for good depth are high IQ (see :ref:`Image Quality <Improving Image Quality>` docs)
left/right stereo images. Active stereo (`Pro version <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/oak-s2.html#pro-version>`__ of OAK cameras)
mostly alleviates this issue, but for passive stereo cameras, there are a few things you can do to improve the quality of the stereo camera pair.

It is preferred to use mono (grayscale) cameras for the stereo camera pair as they
have better quantum efficiency (QE) as they don't have color (Bayer) filter. Higher QE means more signal will be generated for the same amount of light (photons),
which leads to better SNR (signal-to-noise ratio).

For better low-light performance, it's advised to use longer exposure times instead of higher gain (ISO) as it will improve SNR. Sometimes this means lowering
camera FPS - at 30 FPS, you can use 1/30s exposure time, at 15 FPS, you can use 1/15s exposure time, etc. For more information, see :ref:`Low-light increased sensitivity`.

Another potential improvement is to tweak sensor's ISP settings, like chroma & luma denoise, and sharpness. For more information, see :ref:`Color camera ISP configuration`.

Stereo postprocessing filters
-----------------------------

Stereo depth node has a few postprocessing filters that **run on-device**, which can be enabled to improve the quality of the disparity map. For **implementation
(API) details**, see :ref:`StereoDepth configurable blocks <Currently configurable blocks>`. For an example, see :ref:`Depth Post-Processing` example.

As these filters run on device, it has a **decent performance cost**, which mean that at high-resolution frames (1MP) these might bottleneck the FPS. To improve 
the cost, one should consider using lower-resolution frames (eg. 400P) or use :ref:`Decimation filter`. Due to additional processing, these filters also introduce
:ref:`additional latency <Low Latency>`.

Median filter
~~~~~~~~~~~~~

This is a non-edge preserving Median filter, which can be used to reduce noise and smoothen the depth map. Median filter is implemented in hardware, so it's the
fastest filter.

Speckle filter
~~~~~~~~~~~~~~

Speckle Filter is used to reduce the speckle noise. Speckle noise is a region with huge variance between neighboring disparity/depth pixels, and speckle
filter tries to filter this region.

Temporal filter
~~~~~~~~~~~~~~~

Temporal Filter is intended to improve the depth data persistency by manipulating per-pixel values based on previous frames. The filter performs a single pass on
the data, adjusting the depth values while also updating the tracking history.

In cases where the pixel data is missing or invalid, the filter uses a user-defined persistency mode to decide whether the missing value should be improved
with stored data. Note that due to its reliance on historic data the filter may introduce
visible motion blurring/smearing artifacts, and therefore is best-suited for **static scenes**.

Spatial filter
~~~~~~~~~~~~~~

Spatial Edge-Preserving Filter will fill invalid depth pixels with valid neighboring depth pixels. It performs a series of 1D horizontal and vertical passes or
iterations, to enhance the smoothness of the reconstructed data. It is based on `this research paper <https://www.inf.ufrgs.br/~eslgastal/DomainTransform/>`__.

Threshold filter
~~~~~~~~~~~~~~~~

Threshold filter will filter out all depth pixels outside the configured min/max threshold values. In controlled environment, where you know exactly how far the scene
can be (eg. 30cm - 2m) it's advised to use this filter.

Decimation filter
~~~~~~~~~~~~~~~~~

Decimation Filter will sub-samples the depth map, which means it reduces the depth scene complexity and allows other filters to run faster. Setting
*decimationFactor* to 2 will downscale 1280x800 depth map to 640x400.

3. Improving depth accuracy
***************************

The above chapter () was focused on noise, which isn't necessary the only reason for
inaccurate depth.

There are a few ways to improve depth accuracy:

- (mentioned above) :ref:`Fixing noisy depth <2. Fixing noisy depth>` - depth should be high quality in order to be accurate
- (mentioned above) :ref:`Stereo depth confidence threshold` should we low(er) in order to get the best accuracy
- 



Looking at :ref:`Depth from disparity` section, from the graph it's clear that at the 95 disparity pixels (close distance, about 70cm),
depth change between disparity pixels (eg. 95->94) is the lowest, so the depth accuracy is the highest.


.. figure:: /_static/images/components/theoretical_error.jpg

It's clear that depth accuracy decreases exponentially with the distance from the camera. Note that With :ref:`Stereo Subpixel mode`
you can have a better depth accuracy at a longer distance but it only works to some extent.



Stereo Subpixel mode
~~~~~~~~~~~~~~~~~~~~

(see `What's subpixel? <https://dsp.stackexchange.com/questions/34103/subpixel-what-is-it>`__)



Disparity and depth are inversely related. As disparity decreases, depth increases exponentially depending on baseline and focal length. Meaning, if the disparity value is close to zero, then a small change in disparity generates a large change in depth. Similarly, if the disparity value is big, then large changes in disparity do not lead to a large change in depth.

* Baseline / distance to objects

Lower baseline enables us to detect the depth at a closer distance as long as the object is visible in both the frames. However, this reduces the accuracy for large distances due to less pixels representing the object and disparity decreasing towards 0 much faster.
So the common norm is to adjust the baseline according to how far/close we want to be able to detect objects.


4. Short range stereo depth
***************************

To get an accurate short-range 

- Extended mode
- Lower resolution
- Disparity shift
- Closer baseline distance (OAK-D-SR)

Long range stereo depth
***********************

- Subpixel mode
- Narrow FOV lenses/wider baseline distance -> OAK-D-LR



Fixing noisy pointcloud
***********************

- First check Depth is noisy
- Voxalization + remove statistical outliers
- Invalidation a few pixels around the corner of depth image, invalidating left part (eg. 30 pixels)
- Brightness filter (remove black part from rectification)
- Decimation filter?

Decimation filter - for PCL, you don't really want 1 million points (Although it sounds good from marketing POV), as it's too much data to process.
Decimation filter helps here, I'd go as far as must have. It has capabilities of filtering also, if you change it to median mode vs default pixel skipping,
not just reducing image size. It also makes the other filters faster, since there will be less data to process.

Best practices in certain environments
**************************************

- In high dynamic range env (like outside), use brightness filter (img above)
- In more static env, temporal filter







.. include::  /includes/footer-short.rst