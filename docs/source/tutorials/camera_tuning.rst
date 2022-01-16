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

To tune your own camera sensors, one would need special Intel's software, which is under their IP (license needed)
- which means that the majority of people will only be able to use pre-tuned blobs.

**Currently available tuning blobs:**

- Color tuning for low-light environments `here <https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/tuning_color_low_light.bin>`__. Comparison below.

.. image:: /_static/images/tutorials/tuning-comparison.jpeg