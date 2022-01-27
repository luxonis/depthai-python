Camera tuning
=============

Our library supports setting camera IQ (Image Quality) tuning blob, which would be used for all cameras.
By default, cameras will use a *general* tuning blob, which works great in most cases - so changing the camera
tuning blob is not needed for most cases.

.. code-block:: python

    import depthai as dai

    pipeline = dai.Pipeline()
    pipeline.setCameraTuningBlobPath('/path/to/tuning.bin')

Available tuning blobs
######################

To tune your own camera sensors, one would need Intel's software, for which a license is needed
- so the majority of people will only be able to use pre-tuned blobs.

**Currently available tuning blobs:**

- **Mono tuning for low-light environments** `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_mono_low_light.bin>`__. This allows auto-exposure to go up to 200ms (otherwise limited with default tuning to 33ms). For 200ms auto-exposure, you also need to limit the FPS (:code:`monoRight.setFps(5)`)
- **Color tuning for low-light environments** `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_color_low_light.bin>`__. Comparison below. This allows auto-exposure to go up to 100ms (otherwise limited with default tuning to 33ms). For 200ms auto-exposure, you also need to limit the FPS (:code:`rgbCam.setFps(10)`). *Known limitation*: flicker can be seen with auto-exposure over 33ms, it is caused by auto-focus working in continuous mode. A workaround is to change from CONTINUOUS_VIDEO (default) to AUTO (focusing only once at init, and on further focus trigger commands): :code:`camRgb.initialControl.setAutoFocusMode(dai.CameraControl.AutoFocusMode.AUTO)`

.. image:: https://user-images.githubusercontent.com/18037362/149826169-3b92901d-3367-460b-afbf-c33d8dc9d118.jpeg

.. include::  /includes/footer-short.rst