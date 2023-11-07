UVC & Color Camera
==================

This example demonstrates how to use the RGB camera on your OAK device as a UVC webcam. The UVC feature allows you to use your OAK device as a regular webcam in applications like OpenCV's :code:`cv2.VideoCapture()`, native camera apps, and more.


.. rubric:: Similar samples:

- :ref:`UVC & Disparity`
- :ref:`UVC & Mono Camera`


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

        Also `available on GitHub <https://github.com/luxonis/depthai-python/blob/main/examples/UVC/uvc_rgb.py>`__

        .. literalinclude:: ../../../../examples/UVC/uvc_rgb.py
           :language: python
           :linenos:

.. include::  /includes/footer-short.rst