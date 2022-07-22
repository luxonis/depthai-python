Maximizing FOV
==============

Do you need to maximize the FOV of your OAK?

By default, when you are using :code:`preview` output from :ref:`ColorCamera`, DepthAI will crop the
frames to get the desired aspect ratio. For example, if you are using Mobilenet-SSD model, you need
:code:`300x300` frames. DepthAI will crop 1080P frames to :code:`1080x1080` and then resize them to :code:`300x300`.
This means you will lose some part of the image.

If you need to maximize the FOV of the image captured by an OAK-D or other OAK camera, you can either:

#. Change the aspect ratio (stretch the image)
#. Apply letterboxing to the image

Change aspect ratio
*******************

Use :code:`camRgb.setPreviewKeepAspectRatio(False)`. This means the aspect ratio will not be preserved and the image
will be "stretched". This might be problematic for some off-the-shelf NN models, so some fine-tuning might be required.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-lossless-zooming/main.py#L19>`__.

.. image:: https://user-images.githubusercontent.com/18037362/144095838-d082040a-9716-4f8e-90e5-15bcb23115f9.gif
    :target: https://youtu.be/8X0IcnkeIf8

Letterboxing
************

`Letterboxing <https://en.wikipedia.org/wiki/Letterboxing_%28filming%29>`__ the frames. This method of OAK FOV maximization will decrease
the size of the image and apply "black bars" above and below the image, so the aspect ratio is preserved. You can
achieve this by using :ref:`ImageManip` with :code:`manip.setResizeThumbnail(x,y)` (for Mobilenet :code:`x=300,y=300`).
The downside of using this method is that your actual image will be smaller, so some features might not be preserved,
which can mean the NN accuracy could decrease.
`Usage example here <https://github.com/luxonis/depthai-experiments/blob/master/gen2-full-fov-nn/main.py#L28>`__.

.. image:: /_static/images/tutorials/fov.jpeg

12MP OAK FOV
************

Do you need to get full 12MP FOV for your OAK? We can help with that too.

Check out the following `Full FOV NN Inferencing experiment <https://github.com/luxonis/depthai-experiments/tree/master/gen2-full-fov-nn>`__ to learn how.

.. image:: /_static/images/tutorials/full_fov_demo.png