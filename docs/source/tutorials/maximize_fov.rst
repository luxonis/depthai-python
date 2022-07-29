How to maximize FOV
===================

By default, when you are using :code:`preview` output from :ref:`ColorCamera`, the DepthAI will crop the
frames to get the desired aspect ratio. For example, if you are using Mobilenet-SSD model, you need
:code:`300x300` frames. DepthAI will crop 1080P frames to :code:`1080x1080` and then resize them to :code:`300x300`.
This means you will lose some part of the image.

If you would like to maximize the FOV of the image, you can either:

#. Change the aspect ratio (stretch the image)
#. Apply letterboxing to the image

Change aspect ratio
*******************

Use :code:`camRgb.setPreviewKeepAspectRatio(False)`. This means the aspect ratio will not be preserved and the image
will be "stretched". This might be problematic for some off-the-shelf NN models, so model fine-tuning it might be required.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-lossless-zooming/main.py#L19>`__.

.. image:: https://user-images.githubusercontent.com/18037362/144095838-d082040a-9716-4f8e-90e5-15bcb23115f9.gif
    :target: https://youtu.be/8X0IcnkeIf8

Letterboxing
************

`Letterboxing <https://en.wikipedia.org/wiki/Letterboxing_%28filming%29>`__ the frames. This method will decrease
the size of the image and apply "black bars" above and below the image, so the aspect ratio is preserved. You can
achieve this by using :ref:`ImageManip` with :code:`manip.setResizeThumbnail(x,y)` (for Mobilenet :code:`x=300,y=300`).
The downside of using this method is that your actual image will be smaller, so some features might not be preserved,
which can mean the NN accuracy could decrease.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-full-fov-nn/main.py#L28>`__.

.. image:: /_static/images/tutorials/fov.jpeg

