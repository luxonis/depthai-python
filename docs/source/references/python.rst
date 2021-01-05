Python API Reference
--------------------

.. class:: Device
  :canonical: depthai.Device

  Represents the DepthAI device with the methods to interact with it.


  **Constructors**

  .. function:: __init__(pipeline: Pipeline) -> Device

  .. function:: __init__(pipeline: Pipeline, usb2_mode: bool) -> Device

  .. function:: __init__(pipeline: Pipeline, cmd_path: str) -> Device

  .. function:: __init__(pipeline: Pipeline, device_info: DeviceInfo, usb2_mode: bool) -> Device

  .. function:: __init__(pipeline: Pipeline, device_info: DeviceInfo, cmd_path: str) -> Device

    **pipeline** is a :class:`Pipeline` object with all nodes and connections defined

    **usb2_mode**, being :code:`True/False`, allows the DepthAI to communicate using USB2 protocol, not USB3.
    This lowers the throughput of the pipeline, but allows to use >1m USB cables for connection

    **cmd_path** is a path to custom firmware to be loaded into the DepthAI device, instead of the default one.
    Mostly used in firmware development and debugging

    **device_info** is a :class:`DeviceInfo` object, obtained from :class:`XLinkConnection`. Stores information
    about single DepthAI device, allowing to target a specific one which should run the pipeline


  **Static methods**

  .. function:: getFirstAvailableDevice() -> bool, DeviceInfo

    Returns first device found and available to run the pipeline

  .. function:: getAllAvailableDevices() -> List[DeviceInfo]

    Return the list with all discovered DepthAI devices

  .. function:: getEmbeddedDeviceBinary(usb2_mode: bool) -> List[int]

    Returns the firmware which is used to boot the device

  .. function:: getDeviceByMxId(mx_id: str) -> bool, DeviceInfo

    Returns the device based on the device serial number as a hexadecimal string (e.g. :code:`41440410B1933CBC00`)


  **Member methods**

  .. function:: startPipeline()

    Starts executing the pipeline, provided during initialization

  .. function:: isPipelineRunning() -> bool

    Returns :code:`True` if the :func:`startPipeline` method was called, otherwise returns :code:`False`

  .. function:: getOutputQueue(name: str, mazSize: int = 120, overwrite: bool = False) -> DataOutputQueue

    Creates :class:`DataOutputQueue` object that consumes the data from the queue with specified name

  .. function:: getInputQueue(name: str, mazSize: int = 120, overwrite: bool = False) -> DataInputQueue

    Creates :class:`DataInputQueue` object that sends the data into the queue with specified name

  .. function:: setLogLevel(level: LogLevel)

    Sets the device logging level, which determines how many and how detailed the printed logs will be

  .. function:: getLogLevel() -> LogLevel

    Returns the current device logging level


.. class:: Pipeline
  :canonical: depthai.Pipeline

  Represents the pipeline, being a set of nodes and connections between them, that represents the desired flow of data

  **General methods**

  .. function:: getAssetManager() -> AssetManager

    Returns :class:`AssetManager` instance assigned to the current pipeline

  .. function:: getGlobalProperties() -> GlobalProperties

    Returns :class:`GlobalProperties` instance assigned to the current pipeline

  .. function:: getAllAssets()

    Returns all assets that were added to the current pipeline

  .. function:: remove()

  .. function:: getAllNodes() -> List[object]

    Returns all nodes defined in the current pipeline

  .. function:: getNode(idx: int) -> object

    Returns the pipeline node with specified index or :code:`None` otherwise

  .. function:: getConnections() -> List[Connection]

    Returns a list of :class:`Connection` objects, representing interconnections between nodes

  .. function:: link(out: object, inp: object)

    Allows to link one node's output to another node's input

  .. function:: unlink(out: object, inp: object)

    Allows to remove a previously created link between two nodes

  **Node creation methods**

  .. function:: createXLinkIn()

    Creates :class:

  .. function:: createXLinkOut()

  .. function:: createNeuralNetwork()

  .. function:: createColorCamera()

  .. function:: createVideoEncoder()

  .. function:: createSPIOut()

  .. function:: createImageManip()

  .. function:: createMonoCamera()

  .. function:: createStereoDepth()


.. include::  /references/python_nodes.rst





..
  Below you can see ?dummy=http://, this is a workaround for a Sphinx, see here - https://github.com/sphinx-doc/sphinx/issues/701#issuecomment-697116337

.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Contents:

   depthai.Device <?dummy=http://#Device>
   depthai.Pipeline <?dummy=http://#Pipeline>