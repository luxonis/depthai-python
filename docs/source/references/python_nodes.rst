.. class:: XLinkIn
  :canonical: depthai.XLinkIn

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: out

    Output where the data received from the host is being sent

  **Methods**

  .. function:: setStreamName(name: string)

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setMaxDataSize(maxDataSize: int)

  .. function:: setNumFrames(numFrames: int)


.. class:: XLinkOut
  :canonical: depthai.XLinkOut

  Represents an XLink stream of data from device into host

  **Inputs**

  .. attribute:: input

    Input which receives the data and send it to host

  .. function:: setStreamName(name: string)

    Sets the stream name, needed for :func:`getOutputQueue` to transfer data from it.

  .. function:: setFpsLimit(fps: float)

    Limits the stream throughput


.. class:: ColorCamera
  :canonical: depthai.ColorCamera

  Represents an XLink stream of data from host into device

  **Inputs**

  .. attribute:: inputConfig

    Output where the data received from the host is being sent

  .. attribute:: inputControl

    Output where the data received from the host is being sent

  **Outputs**

  .. attribute:: video

    Output where the data received from the host is being sent

  .. attribute:: preview

    Color camera video stream scaled to the preview size, set using :func:`setPreviewSize`

  .. attribute:: still

    Output where the data received from the host is being sent

  **Methods**

  .. function:: setBoardSocket(boardSocket: CameraBoardSocket)

    Assigns the camera socket to the node

  .. function:: getBoardSocket() -> CameraBoardSocket

    Returns the assigned board socket

  .. function:: setColorOrder(colorOrder: ColorCameraProperties.ColorOrder)

    Sets the color order of the frame

  .. function:: getColorOrder() -> ColorCameraProperties.ColorOrder

    Returns the color order of the frame

  .. function:: setFp16(fp16: bool)

    Forces the frame values to be in FP16 format

  .. function:: getFp16() -> bool

    Returns information if the output values were forced to be in FP16 format

  .. function:: setFps(fps: float)

    Limits the FPS of the node, allowing e.g. to decrease the amount of frames being produced by this node

  .. function:: getFps() -> float

    Returns the current FPS limit

  .. function:: setPreviewSize(width: int, height: int)

    Sets the desired preview dimensions, available as :attr:`preview` output

  .. function:: getPreviewSize() -> Tuple[int, int]

    Returns the preview dimensions previously set using :func:`setPreviewSize`

  .. function:: getPreviewWidth() -> int

    Returns width of the preview previously set using :func:`setPreviewSize`

  .. function:: getPreviewHeight() -> int

    Returns height of the preview previously set using :func:`setPreviewSize`

  .. function:: setVideoSize(width: int, height: int)

    Sets the desired video dimensions, available as :attr:`video` output

  .. function:: getVideoSize() -> Tuple[int, int]

    Returns the video dimensions previously set using :func:`getVideoSize`

  .. function:: getVideoWidth() -> int

    Returns width of the video previously set using :func:`getVideoSize`

  .. function:: getVideoHeight() -> int

    Returns height of the video previously set using :func:`getVideoSize`

  .. function:: setStillSize()

  .. function:: getStillSize()

  .. function:: getStillWidth()

  .. function:: setResolution(resolution: ColorCameraProperties.SensorResolution)

    Sets the camera sensor resolution, determining the maximum resolution that can be obtained from this node

  .. function:: getResolution() -> ColorCameraProperties.SensorResolution

    Returns camera resolution previously set using :func:`setResolution`

  .. function:: getResolutionSize() -> Tuple[int, int]

    Returns the sensor resolution dimensions

  .. function:: getResolutionWidth() -> int

    Returns the sensor resolution width

  .. function:: getResolutionHeight() -> int

    Returns the sensor resolution height

  .. function:: sensorCenterCrop()

  .. function:: setSensorCrop()

  .. function:: getSensorCrop()

  .. function:: getSensorCropX()

  .. function:: getSensorCropY()

  .. function:: setWaitForConfigInput()

  .. function:: getWaitForConfigInput()

  .. function:: setPreviewKeepAspectRatio(keep: bool)

  .. function:: getPreviewKeepAspectRatio() -> bool


.. class:: MonoCamera
  :canonical: depthai.MonoCamera

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: out

    Mono camera video stream, sending grayscale frames

  **Methods**

  .. function:: setBoardSocket()

    Assigns the camera socket to the node

  .. function:: getBoardSocket()

    Returns the assigned board socket

  .. function:: setResolution(resolution: MonoCameraProperties.SensorResolution)

    Sets the camera sensor resolution, determining the maximum resolution that can be obtained from this node

  .. function:: getResolution() -> MonoCameraProperties.SensorResolution

    Returns camera resolution previously set using :func:`setResolution`

  .. function:: setFps(fps: float)

    Limits the FPS of the node, allowing e.g. to decrease the amount of frames being produced by this node

  .. function:: getFps() -> float

    Returns the current FPS limit

  .. function:: getResolutionSize() -> Tuple[int, int]

    Returns the sensor resolution dimensions

  .. function:: getResolutionWidth() -> int

    Returns the sensor resolution width

  .. function:: getResolutionHeight() -> int

    Returns the sensor resolution height


.. class:: NeuralNetwork
  :canonical: depthai.NeuralNetwork

  Represents an XLink stream of data from host into device

  **Inputs**

  .. attribute:: input

    Neural network input, on which the inference will be ran

  **Outputs**

  .. attribute:: out

    Inference results output, all neural network outputs will be send through this output

  .. function:: setBlobPath(path: str)

    Sets the neural network blob path, being the actual neural network to be ran

  .. function:: setNumPoolFrames()


.. class:: ImageManip
  :canonical: depthai.ImageManip

  Represents an XLink stream of data from host into device

  **Inputs**

  .. attribute:: inputConfig

    Output where the data received from the host is being sent

  .. attribute:: inputImage

    Output where the data received from the host is being sent

  **Outputs**

  .. attribute:: out

    Output where the data received from the host is being sent

  **Methods**

  .. function:: setCropRect()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setCenterCrop()

  .. function:: setResize()

  .. function:: setResizeThumbnail()

  .. function:: setFrameType()

  .. function:: setHorizontalFlip()

  .. function:: setWaitForConfigInput()

  .. function:: setNumFramesPool()

  .. function:: setMaxOutputFrameSize()


.. class:: StereoDepth
  :canonical: depthai.StereoDepth

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: left

    Output where the data received from the host is being sent

  .. attribute:: right

    Output where the data received from the host is being sent

  .. attribute:: depth

  .. attribute:: disparity

  .. attribute:: syncedLeft

  .. attribute:: syncedRight

  .. attribute:: rectifiedLeft

  .. attribute:: rectifiedRight

  .. function:: loadCalibrationFile()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: loadCalibrationData()

  .. function:: setEmptyCalibration()

  .. function:: setInputResolution()

  .. function:: setMedianFilter()

  .. function:: setConfidenceThreshold()

  .. function:: setLeftRightCheck()

  .. function:: setSubpixel()

  .. function:: setExtendedDisparity()

  .. function:: setRectifyEdgeFillColor()

  .. function:: setRectifyMirrorFrame()

  .. function:: setOutputRectified()

  .. function:: setOutputDepth()


.. class:: VideoEncoder
  :canonical: depthai.VideoEncoder

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: input

    Output where the data received from the host is being sent

  .. attribute:: bitstream

    Output where the data received from the host is being sent

  .. function:: setDefaultProfilePreset()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setNumFramesPool()

  .. function:: getNumFramesPool()

  .. function:: setRateControlMode()

  .. function:: getRateControlMode()

  .. function:: setProfile()

  .. function:: getProfile()

  .. function:: setBitrate()

  .. function:: getBitrate()

  .. function:: setKeyframeFrequency()

  .. function:: getKeyframeFrequency()

  .. function:: setNumBFrames()

  .. function:: getNumBFrames()

  .. function:: setQuality()

  .. function:: getQuality()

  .. function:: getWidth()

  .. function:: getHeight()

  .. function:: getSize() -> Tuple[int, int]

  .. function:: setFrameRate()

  .. function:: getFrameRate()


.. class:: CameraBoardSocket
  :canonical: depthai.CameraBoardSocket

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: AUTO

  .. attribute:: RGB

  .. attribute:: LEFT

  .. attribute:: RIGHT


.. class:: ColorCameraProperties
  :canonical: depthai.ColorCameraProperties

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: boardSocket

    Output where the data received from the host is being sent

  .. attribute:: colorOrder

    Output where the data received from the host is being sent

  .. attribute:: interleaved

    Output where the data received from the host is being sent

  .. attribute:: previewHeight

    Output where the data received from the host is being sent

  .. attribute:: previewWidth

    Output where the data received from the host is being sent

  .. attribute:: resolution

    Output where the data received from the host is being sent

  .. attribute:: fps

    Output where the data received from the host is being sent

  .. attribute:: videoHeight

  .. attribute:: videoWidth

  .. attribute:: stillHeight

  .. attribute:: stillWidth

  .. attribute:: sensorCropX

  .. attribute:: sensorCropY


.. class:: ColorCameraProperties.SensorResolution
  :canonical: depthai.ColorCameraProperties.SensorResolution

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: THE_1080_P

    Output where the data received from the host is being sent

  .. attribute:: THE_4_K

    Output where the data received from the host is being sent

  .. attribute:: THE_12_MP

    Output where the data received from the host is being sent


.. class:: ColorCameraProperties.ColorOrder
  :canonical: depthai.ColorCameraProperties.ColorOrder

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: BGR

    Output where the data received from the host is being sent

  .. attribute:: RGB

    Output where the data received from the host is being sent


.. class:: MonoCameraProperties
  :canonical: depthai.MonoCameraProperties

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: boardSocket

    Output where the data received from the host is being sent

  .. attribute:: resolution

    Output where the data received from the host is being sent

  .. attribute:: fps

    Output where the data received from the host is being sent


.. class:: MonoCameraProperties.SensorResolution
  :canonical: depthai.MonoCameraProperties.SensorResolution

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: THE_720_P

    Output where the data received from the host is being sent

  .. attribute:: THE_800_P

    Output where the data received from the host is being sent

  .. attribute:: THE_400_P

    Output where the data received from the host is being sent


.. class:: StereoDepthProperties
  :canonical: depthai.StereoDepthProperties

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: calibration

    Output where the data received from the host is being sent

  .. attribute:: median

    Output where the data received from the host is being sent

  .. attribute:: confidenceThreshold

    Output where the data received from the host is being sent

  .. attribute:: enableLeftRightCheck

  .. attribute:: enableSubpixel

  .. attribute:: enableExtendedDisparity

  .. attribute:: rectifyMirrorFrame

  .. attribute:: rectifyEdgeFillColor

  .. attribute:: enableOutputRectified

  .. attribute:: enableOutputDepth

  .. attribute:: width

  .. attribute:: height


.. class:: StereoDepthProperties.MedianFilter
  :canonical: depthai.StereoDepthProperties.MedianFilter

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: MEDIAN_OFF

    Output where the data received from the host is being sent

  .. attribute:: KERNEL_3x3

    Output where the data received from the host is being sent

  .. attribute:: KERNEL_5x5

    Output where the data received from the host is being sent

  .. attribute:: KERNEL_7x7


.. class:: VideoEncoderProperties
  :canonical: depthai.VideoEncoderProperties

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: bitrate

    Output where the data received from the host is being sent

  .. attribute:: keyframeFrequency

    Output where the data received from the host is being sent

  .. attribute:: maxBitrate

    Output where the data received from the host is being sent

  .. attribute:: numBFrames

  .. attribute:: numFramesPool

  .. attribute:: profile

  .. attribute:: quality

  .. attribute:: rateCtrlMode

  .. attribute:: width

  .. attribute:: height


.. class:: VideoEncoderProperties.Profile
  :canonical: depthai.VideoEncoderProperties.Profile

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: H264_BASELINE

    Output where the data received from the host is being sent

  .. attribute:: H264_HIGH

    Output where the data received from the host is being sent

  .. attribute:: H264_MAIN

    Output where the data received from the host is being sent

  .. attribute:: H265_MAIN

  .. attribute:: MJPEG


.. class:: VideoEncoderProperties.RateControlMode
  :canonical: depthai.VideoEncoderProperties.RateControlMode

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: CBR

    Output where the data received from the host is being sent

  .. attribute:: VBR


.. class:: SPIOut
  :canonical: depthai.SPIOut

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: input

  .. function:: setStreamName()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setBusId()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.



.. toctree::
   :maxdepth: 1
   :hidden:
   :caption: Contents:

   depthai.SPIOut <?dummy=http://#SPIOut>