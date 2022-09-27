Maximizing FOV
==============

In this tutorial we will look at how you can use the full FOV of the image sensor.

When you are using ``preview`` output from :ref:`ColorCamera`, DepthAI will crop the
frames by default to get the desired aspect ratio. ``preview`` stream derives from the ``video`` stream, which is cropped
(16:9 aspect ratio, max 4k resolution) from the ``isp`` stream, which has the full FOV.

**To get the full FOV** of a sensor you need to use its **max resolution** (or 1/N of it, if supported).
So for `OV9282 <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/sensors/ov9282.html#ov9282>`__ (800P)
you can use either 800P or 400P resolution to get full FOV. Meanwhile, for `IMX378 <https://docs.luxonis.com/projects/hardware/en/latest/pages/articles/sensors/imx378.html#imx378>`__,
you need to set full 12MP resolution (as there's no eg. 6MP support). You can use :ref:`ColorCamera`'s' ``.setIspScale()``
to downscale the 12MP if you don't need such large frames.

A challenge occurs when your **NN model expects a different aspect ratio** (eg. 1:1) compared to the **sensors aspect ratio**
(eg. 4:3), and we want to run NN inference on the full FOV of the sensor. Let's say we have a MobileNet-SSD that
requires 300x300 frames (1:1 aspect ratio) - we have a few options:

#. :ref:`Stretch the ISP frame <Change aspect ratio>` to 1:1 aspect ratio of the NN
#. :ref:`Apply letterboxing <Letterboxing>` to the ISP frame to get 1:1 aspect ratio frame
#. :ref:`Crop the ISP frame <Cropping>` to 1:1 aspect ratio and lose some FOV

.. image:: /_static/images/tutorials/isp.jpg

The image above is the ``isp`` output from the :ref:`ColorCamera` (12MP resolution from IMX378). If you aren't downscaling ISP,
the ``video`` output is cropped to 4k (max 3840x2160 due to the limitation of the ``video`` output) as represented by
the blue rectangle. The Yellow rectangle represents a cropped ``preview`` output when the preview size is set to a 1:1 aspect
ratio (eg. when using a 300x300 preview size for the MobileNet-SSD NN model) because the ``preview`` output is derived from
the ``video`` output (see :ref:`ColorCamera` for more information).

Change aspect ratio
*******************

**Pros: Preserves full FOV. Cons: Due to stretched frames, NNs accuracy might decrease.**

Changing aspect ratio (**stretching**) can be used Use :code:`camRgb.setPreviewKeepAspectRatio(False)`. This means the aspect
ratio will not be preserved and the image will be "stretched". This might be problematic for some off-the-shelf NN models, so some fine-tuning might be required.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-full-fov-nn/stretching.py>`__.

.. image:: https://user-images.githubusercontent.com/18037362/180607962-e616cdc7-fcad-4bc8-a15f-617b89a2c047.jpg

Letterboxing
************

**Pros: Preserves full FOV. Cons: Smaller "frame" means less features might decrease NN accuracy.**

`Letterboxing <https://en.wikipedia.org/wiki/Letterboxing_%28filming%29>`__ approach will apply "black bars" above and below
the image to the full FOV (isp) frames, so the aspect ratio will be preserved. You can
achieve this by using :ref:`ImageManip` with :code:`manip.setResizeThumbnail(x,y)` (for Mobilenet :code:`x=300,y=300`).
The downside of using this method is that your actual image will be smaller, so some features might not be preserved,
which can mean the NN accuracy could decrease.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-full-fov-nn/letterboxing.py>`__.

.. image:: https://user-images.githubusercontent.com/18037362/180607958-0db7fb34-1221-42a1-b889-10d1f9793912.jpg

Cropping
********

**Pros: No NN accuracy decrease. Cons: Frame is cropped, so it's not full FOV.**

Cropping the full FOV (isp) frames to match the NN aspect ratio can be used to get the best NN accuracy, but this
decreases FOV.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-full-fov-nn/cropping.py>`__.

.. image:: https://user-images.githubusercontent.com/18037362/180607873-6a476ea4-55e0-4557-a93e-a7cadcd80725.jpg

.. include::  /includes/footer-short.rst
