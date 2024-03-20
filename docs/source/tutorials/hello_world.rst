Hello World
===========

Learn how to use the DepthAI Python API to display a color video stream.


Demo
####

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="//www.youtube.com/embed/puI57TaFCUM" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>

.. _hello_world_dependencies:

Dependencies
############

Let's get your development environment setup first. This tutorial uses:

- Python >=3.6
- DepthAI :ref:`Python API <Installation>`
- :code:`cv2`, :code:`blobconverter` and :code:`numpy` Python modules.


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

  python3 -m pip install numpy opencv-python depthai blobconverter --user


Test your environment
#####################

Let's verify we're able to load all of our dependencies. Open the :code:`hello_world.py` file you
:ref:`created earlier <File Setup>` in your code editor. Copy and paste the following into :code:`hello_world.py`:


.. code-block:: python

  import numpy as np  # numpy - manipulate the packet data returned by depthai
  import cv2  # opencv - display the video stream
  import depthai  # depthai - access the camera and its data packets
  import blobconverter  # blobconverter - compile and download MyriadX neural network blobs

Try running the script and ensure it executes without error:

.. code-block:: bash

  python3 hello_world.py

If you see the following error:

.. code-block::

  ModuleNotFoundError: No module named 'depthai'

...follow `these steps in our troubleshooting section <https://docs.luxonis.com/en/latest/pages/troubleshooting/#importerror-no-module-named-depthai'>`__.

Define a pipeline
#################

Any action from DepthAI, whether it's a neural inference or color camera output, require a **pipeline** to be defined,
including nodes and connections corresponding to our needs.

In this case, we want to see the frames from **color camera**, as well as a simple **neural network** to be ran on top of them.

Let's start off with an empty :class:`Pipeline` object

.. code-block:: python

  pipeline = depthai.Pipeline()

Now, first node we will add is a :class:`ColorCamera`. We will use the :code:`preview` output, resized to 300x300 to fit the
`mobilenet-ssd input size <https://docs.openvino.ai/2021.4/omz_models_model_mobilenet_ssd.html>`__ (which we will define later)

.. code-block:: python

  cam_rgb = pipeline.create(depthai.node.ColorCamera)
  cam_rgb.setPreviewSize(300, 300)
  cam_rgb.setInterleaved(False)

Up next, let's define a :class:`MobileNetDetectionNetwork` node with `mobilenet-ssd network <https://docs.openvinotoolkit.org/latest/omz_models_public_mobilenet_ssd_moblenet_ssd.html>`__.
The blob file for this example will be compiled and downloaded automatically using `blobconverter tool <https://pypi.org/project/blobconverter/>`__.
:code:`blobconverter.from_zoo()` function returns Path to the model, so we can directly put it inside the :code:`detection_nn.setBlobPath()` function.
With this node, the output from nn will be parsed on device side and we'll receive a ready to use detection objects. For this to work properly, we need also to set the confidence threshold
to filter out the incorrect results

.. code-block:: python

  detection_nn = pipeline.create(depthai.node.MobileNetDetectionNetwork)
  # Set path of the blob (NN model). We will use blobconverter to convert&download the model
  # detection_nn.setBlobPath("/path/to/model.blob")
  detection_nn.setBlobPath(blobconverter.from_zoo(name='mobilenet-ssd', shaves=6))
  detection_nn.setConfidenceThreshold(0.5)

And now, let's connect a color camera :code:`preview` output to neural network input

.. code-block:: python

  cam_rgb.preview.link(detection_nn.input)

Finally, we want to receive both color camera frames and neural network inference results - as these are produced on the
device, they need to be transported to our machine (host). The communication between device and host is handled by :code:`XLink`,
and in our case, since we want to receive data from device to host, we will use :class:`XLinkOut` node

.. code-block:: python

  xout_rgb = pipeline.create(depthai.node.XLinkOut)
  xout_rgb.setStreamName("rgb")
  cam_rgb.preview.link(xout_rgb.input)

  xout_nn = pipeline.create(depthai.node.XLinkOut)
  xout_nn.setStreamName("nn")
  detection_nn.out.link(xout_nn.input)


Initialize the DepthAI Device
#############################

Having the pipeline defined, we can now initialize a device with pipeline and start it

.. code-block:: python

  with depthai.Device(pipeline) as device:

.. note::

  By default, the DepthAI is accessed as a USB3 device. This comes with `several limitations <https://docs.luxonis.com/en/latest/pages/troubleshooting/#intermittent-connectivity-with-long-2-meter-usb3-cables>`__.

  If you'd like to communicate via USB2, being free from these but having a limited bandwidth, initialize the DepthAI with the following code

  .. code-block:: python

    device = depthai.Device(pipeline, usb2Mode=True)



From this point on, the pipeline will be running on the device, producing results we requested. Let's grab them

Adding helpers
##############

As :class:`XLinkOut` nodes has been defined in the pipeline, we'll define now a host side output queues to access the
produced results

.. code-block:: python

  q_rgb = device.getOutputQueue("rgb")
  q_nn = device.getOutputQueue("nn")

These will fill up with results, so next thing to do is consume the results. We will need two placeholders - one
for rgb frame and one for nn results

.. code-block:: python

  frame = None
  detections = []

Also, due to neural network implementation details, bounding box coordinates in inference results are represented
as floats from <0..1> range - so relative to frame width/height (e.g. if image has 200px width and nn returned x_min
coordinate equal to 0.2, this means the actual (normalised) x_min coordinate is 40px).

That's why we need to define a helper function, :code:`frameNorm`, that will convert these <0..1> values into actual
pixel positions

.. code-block:: python

    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)


Consuming the results
#####################

Having everything prepared, we are ready to start out main program loop

.. code-block:: python

  while True:
    # ...

Now, inside this loop, first thing to do is fetching latest results from both nn node and color camera

.. code-block:: python

  in_rgb = q_rgb.tryGet()
  in_nn = q_nn.tryGet()

The :code:`tryGet` method returns either the latest result or :code:`None` if the queue is empty.

Results, both from rgb camera or neural network, will be delivered as 1D arrays, so both of them will require transformations
to be useful for display (we have already defined one of the transformations needed - the :code:`frameNorm` function)

First up, if we receive a frame from rgb camera using the :code:`getCvFrame` command

.. code-block:: python

  if in_rgb is not None:
      frame = in_rgb.getCvFrame()

Second, we will receive the neural network results. Default MobileNetSSD result has 7 fields, each being respectively :code:`image_id, label, confidence, x_min, y_min, x_max, y_max`,
and by accessing the :code:`detections` array, we receive the detection objects that allow us to access these fields

.. code-block:: python

  if in_nn is not None:
      detections = in_nn.detections

Display the results
###################

Up to this point, we have all our results consumed from the DepthaI device, and only thing left is to actually display them.

.. code-block:: python

  if frame is not None:
      for detection in detections:
          bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
          cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), (255, 0, 0), 2)
      cv2.imshow("preview", frame)

You can see here the usage of :code:`frameNorm` we defined earlier for bounding box coordinates normalization.
By using :code:`cv2.rectangle` we draw a rectangle on the rgb frame as an indicator where the face position is, and then
we display the frame using :code:`cv2.imshow`

Finally, we add a way to terminate our program (as it's running inside an infinite loop). We will use :code:`cv2.waitKey`
method, that waits for a key to be pressed by user - in our case, we want to break out of the loop when user presses :code:`q` key

.. code-block:: python

    if cv2.waitKey(1) == ord('q'):
        break

Running the example
###################

Putting it all together, only thing left to do is to run the file we've prepared in this tutorial and see the results

.. code-block:: bash

  python3 hello_world.py

You're on your way! You can find the `complete code for this tutorial on GitHub <https://github.com/luxonis/depthai-tutorials/blob/master/1-hello-world/hello_world.py>`__.

.. include::  /includes/footer-short.rst
