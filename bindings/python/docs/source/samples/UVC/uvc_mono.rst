UVC & Mono Camera
=================

This example demonstrates how to use a mono camera on your OAK device to function as a webcam. The UVC feature allows you to use your OAK device as a regular webcam in applications like OpenCV's :code:`cv2.VideoCapture()`, native camera apps, and more.

.. rubric:: How It Works:

The :ref:`MonoCamera` node outputs image data in the GRAY8 format. However, the :ref:`UVC` node expects the data in NV12 format. To bridge this gap, an intermediary :ref:`ImageManip` node is used to convert the GRAY8 output from the MonoCamera node to NV12 format, which is then passed to the UVC node for streaming.


.. rubric:: Similar samples:

- :ref:`UVC & Color Camera`
- :ref:`UVC & Disparity`


Setup
#####

.. include::  /includes/install_from_pypi.rst

Code used for testing
#####################

.. code-block:: python

    import cv2

    # Initialize the VideoCapture object to use the default camera (camera index 0 is webcam)
    cap = cv2.VideoCapture(1)

    # Check if the camera opened successfully
    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    # Loop to continuously get frames from the camera
    while True:
        ret, frame = cap.read()

        if not ret:
            print("Error: Could not read frame.")
            break

        cv2.imshow('Video Feed', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()



Source code
###########

.. tabs::

    .. tab:: Python

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/UVC/uvc_mono.py>`__

        .. literalinclude:: ../../../../examples/UVC/uvc_mono.py
           :language: python
           :linenos:


.. include::  /includes/footer-short.rst