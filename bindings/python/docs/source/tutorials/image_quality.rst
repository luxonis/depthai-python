Improving Image Quality
=======================

There are a few ways to improve Image Quality (IQ) on OAK cameras. A few examples:

#. Changing :ref:`Color camera ISP configuration`
#. Try keeping camera sensitivity low - :ref:`Low-light increased sensitivity`
#. :ref:`Camera tuning` with custom tuning blobs
#. Ways to reduce :ref:`Motion blur` effects

Note that the `Series 3 OAK cameras <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/oak-s3.html>`__ will
also have **temporal noise filter**, which will improve IQ.

For best IQ, **we suggest testing it yourself for your specific application**. You can use :ref:`RGB Camera Control` to try out
different ISP configurations and exposure/sensitivity values dynamically (live).

Color camera ISP configuration
##############################

You can **configure** :ref:`ColorCamera` **ISP values** such as ``sharpness``, ``luma denoise`` and ``chroma denoise``, which
can improve IQ. We have noticed that sometimes these values provide better results:

.. code-block:: python

    camRgb = pipeline.create(dai.node.ColorCamera)
    camRgb.initialControl.setSharpness(0)     # range: 0..4, default: 1
    camRgb.initialControl.setLumaDenoise(0)   # range: 0..4, default: 1
    camRgb.initialControl.setChromaDenoise(4) # range: 0..4, default: 1

.. image:: https://user-images.githubusercontent.com/18037362/181258871-f0c47fd8-e12b-4d33-89e5-15b48ffd5a3d.png

The zoomed-in image above showcases the IQ difference between ISP configuration (`discussion here <https://discuss.luxonis.com/d/554-image-pipelines-compression-and-filtering/8>`__).
Note that for best IQ, you would need to test and evaluate these values for your specific application.

On Wide FOV `Series 2 cameras <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/oak-s2.html>`__,
you can select between wide FOV IMX378 and OV9782. In general, the **IQ of
OV9782 can't be as good as say IMX378**, as the resolution is much lower, and it's harder to deal with sharpness/noise at
low resolutions. With high resolution the image can be downscaled and noise would be less visible.
And even though OV9782 has quite large pixels, in general the noise levels of global shutters are more significant than for
rolling shutter.

Low-light increased sensitivity
###############################

On the image below you can see how different sensitivity values affect the IQ. Sensitivity will only add analog gain, which
will increase the image noise. When in low-light environment, one should always increase exposure, not sensitivity. Note
that by default, depthai will always do so - but when using 30FPS, max exposure is 33ms. For the right image below, we
have set ColorCamera to 10 FPS, so we were able to increase exposure to 100ms.

.. image:: https://user-images.githubusercontent.com/18037362/181298535-085d135e-5817-48fa-8392-f711ade69a77.png

About **15x digitally zoomed-in** image of a standard A4 camera tuning target at 420cm (40 lux). We have used 12MP IMX378 (on OAK-D) for this image.

Camera tuning
#############

Our library supports setting camera IQ tuning blob, which would be used for all cameras.
By default, cameras will use a *general* tuning blob, which works great in most cases - so changing the camera
tuning blob is not needed for most cases.

.. code-block:: python

    import depthai as dai

    pipeline = dai.Pipeline()
    pipeline.setCameraTuningBlobPath('/path/to/tuning.bin')

**Available tuning blobs**

To tune your own camera sensors, one would need Intel's software, for which a license is needed
- so the majority of people will only be able to use pre-tuned blobs. Currently available tuning blobs:

- **Mono tuning for low-light environments** `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_mono_low_light.bin>`__. This allows auto-exposure to go up to 200ms (otherwise limited with default tuning to 33ms). For 200ms auto-exposure, you also need to limit the FPS (:code:`monoRight.setFps(5)`)
- **Color tuning for low-light environments** `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_color_low_light.bin>`__. Comparison below. This allows auto-exposure to go up to 100ms (otherwise limited with default tuning to 33ms). For 200ms auto-exposure, you also need to limit the FPS (:code:`rgbCam.setFps(10)`). *Known limitation*: flicker can be seen with auto-exposure over 33ms, it is caused by auto-focus working in continuous mode. A workaround is to change from CONTINUOUS_VIDEO (default) to AUTO (focusing only once at init, and on further focus trigger commands): :code:`camRgb.initialControl.setAutoFocusMode(dai.CameraControl.AutoFocusMode.AUTO)`
- **OV9782 Wide FOV color tuning for sunlight environments** `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_color_ov9782_wide_fov.bin>`__.  Fixes lens color filtering on direct sunglight, see `blog post here <https://www.luxonis.com/blog/lens_color_filtering_enhancement>`__. It also improves LSC (Lens Shading Correction). Currently doesn't work for OV9282, so when used on eg. Series 2 OAK with Wide FOV cams, mono cameras shouldn't be enabled.
- **Camera exposure limit**: `max 500us <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_exp_limit_500us.bin>`__, `max 8300us <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_exp_limit_8300us.bin>`__. These tuning blobs will limit the maximum exposure time, and instead start increasing ISO (sensitivity) after max exposure time is reached. This is a useful approach to reduce the :ref:`Motion blur`.

.. image:: https://user-images.githubusercontent.com/18037362/149826169-3b92901d-3367-460b-afbf-c33d8dc9d118.jpeg

Motion blur
###########

`Motion blur <https://en.wikipedia.org/wiki/Motion_blur>`__ appears when the camera shutter is opened for a longer time, and the object is moving during that time.

.. image:: https://user-images.githubusercontent.com/18037362/209683640-2a640794-8422-4119-9d78-6c23690418a1.jpg

In the image above the right foot moved about 50 pixels during the exposure time, which results in a blurry image in that region.
The left foot was on the ground the whole time of the exposure, so it's not blurry.

In **high-vibration environments** we recommend **using Fixed-Focus** color camera, as otherwise the Auto-Focus lens will be shaking and
cause blurry images (`docs here <https://docs.luxonis.com/projects/hardware/en/latest/pages/guides/af_ff.html#trade-offs>`__).

**Potential workarounds:**

1. Decrease the shutter (exposure) time - this will decrease the motion blur, but will also decrease the light that reaches the sensor, so the image will be darker. You could either use a larger sensor (so more photons hit the sensor) or use a higher ISO (sensitivity) value. One option to limit max exposure time is by using a :reF:`Camera tuning` blob.
2. If the motion blur negatively affects your model's accuracy, you could fine-tune it to be more robust to motion blur by including motion blur images in your training dataset. Example video:

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/q9r8IJBTev0" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

.. include::  /includes/footer-short.rst