Hello World
===========

Learn how to use the DepthAI Python API to display a color video stream.

.. _hello_world_dependencies:

Dependencies
############

Let's get your development environment setup first. This tutorial uses:

- Python 3.6 (Ubuntu) or Python 3.7 (Raspbian).
- The DepthAI :ref:`Python API`
- The :code:`cv2` and :code:`numpy` Python modules.


Code Overview
#############

The :code:`depthai` Python module provides access to your board's 4K 60 Hz color camera.
We'll display a video stream from this camera to your desktop.
You can find the `complete source code for this tutorial on GitHub <https://github.com/luxonis/depthai-tutorials/tree/master/1-hello-world>`__.

File Setup
##########

Setup the following file structure on your computer:

.. code-block:: bash

  cd ~
  mkdir -p depthai-tutorials-practice/1-hello-world
  touch depthai-tutorials-practice/1-hello-world/hello_world.py
  cd depthai-tutorials-practice/1-hello-world

What's with the :code:`-practice` suffix in parent directory name? Our tutorials are available on GitHub
via the `depthai-tutorials <https://github.com/luxonis/depthai-tutorials>`__ repository.
We're appending :code:`-practice` so you can distinguish between your work and our finished
tutorials (should you choose to download those).


Install pip dependencies
########################

To display the DepthAI color video stream we need to import a small number of packages.
Download and install the requirements for this tutorial:

.. code-block:: bash

  python3 -m pip install numpy opencv-python depthai --user


Test your environment
#####################

Let's verify we're able to load all of our dependencies. Open the :code:`hello_world.py` file you
:ref:`created earlier <File Setup>` in your code editor. Copy and paste the following into :code:`hello_world.py`:


.. code-block:: python

  import numpy as np # numpy - manipulate the packet data returned by depthai
  import cv2 # opencv - display the video stream
  import depthai # access the camera and its data packets

Try running the script and ensure it executes without error:

.. code-block:: bash

  python3 hello_world.py

If you see the following error:

.. code-block::

  ModuleNotFoundError: No module named 'depthai'

...follow :ref:`these steps in our troubleshooting section <ImportError: No module named 'depthai'>`.

Initialize the DepthAI Device
#############################

Start the DepthAI device:

.. code-block:: python

  device = depthai.Device('', False)

Try running the script. You should see output similar to:

.. code-block::

  No calibration file. Using Calibration Defaults.
  XLink initialized.
  Sending device firmware "cmd_file": /home/pi/Desktop/depthai/depthai.cmd
  Successfully connected to device.
  Loading config file
  Attempting to open stream config_d2h
  watchdog started 6000
  Successfully opened stream config_d2h with ID #0!

If instead you see an error, please :ref:`reset your DepthAI device <'depthai: Error initalizing xlink' errors and DepthAI fails to run.>`, then try again.

Create the DepthAI Pipeline
###########################

Now we'll create our data pipeline using the :code:`previewout` stream. This stream contains the data from the color camera.
The model used in :code:`ai` section is a MobileNetSSD with 20 different classes, see
`here <https://github.com/luxonis/depthai/blob/master/resources/nn/mobilenet-ssd/mobilenet-ssd.json>`__ for details

.. code-block:: python

  # Create the pipeline using the 'previewout' stream, establishing the first connection to the device.
  pipeline = device.create_pipeline(config={
      'streams': ['previewout', 'metaout'],
      'ai': {
          'blob_file': "/path/to/mobilenet-ssd.blob",
          'blob_file_config': "/path/to/mobilenet-ssd.json"
      }
  })

  if pipeline is None:
      raise RuntimeError('Pipeline creation failed!')

Display the video stream
########################

A DepthAI Pipeline generates a stream of data packets. Each :code:`previewout` data packet contains a
3D array representing an image frame.
We change the shape of the frame into a :code:`cv2`-compatible format and display it.

.. code-block:: python

  detections = []

  while True:
      # Retrieve data packets from the device.
      # A data packet contains the video frame data.
      nnet_packets, data_packets = pipeline.get_available_nnet_and_data_packets()

      for nnet_packet in nnet_packets:
          detections = list(nnet_packet.getDetectedObjects())

      for packet in data_packets:
          # By default, DepthAI adds other streams (notably 'meta_2dh'). Only process `previewout`.
          if packet.stream_name == 'previewout':
              data = packet.getData()
              # the format of previewout image is CHW (Channel, Height, Width), but OpenCV needs HWC, so we
              # change shape (3, 300, 300) -> (300, 300, 3)
              data0 = data[0,:,:]
              data1 = data[1,:,:]
              data2 = data[2,:,:]
              frame = cv2.merge([data0, data1, data2])

              img_h = frame.shape[0]
              img_w = frame.shape[1]

              for detection in detections:
                  pt1 = int(detection.x_min * img_w), int(detection.y_min * img_h)
                  pt2 = int(detection.x_max * img_w), int(detection.y_max * img_h)

              cv2.imshow('previewout', frame)

      if cv2.waitKey(1) == ord('q'):
          break

  # The pipeline object should be deleted after exiting the loop. Otherwise device will continue working.
  # This is required if you are going to add code after exiting the loop.
  del pipeline
  del device

Run the script. Press the :code:`Q` key with focus on the video stream (not your terminal) to exit:

.. code-block:: bash

  python3 hello_world.py

You're on your way! You can find the `complete code for this tutorial on GitHub <https://github.com/luxonis/depthai-tutorials/blob/master/1-hello-world/hello_world.py>`__.

.. include::  /pages/includes/footer-short.rst
