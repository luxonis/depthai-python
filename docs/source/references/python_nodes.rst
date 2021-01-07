.. class:: XLinkIn
  :canonical: depthai.XLinkIn

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: out

    Output where the data received from the host is being sent

  .. function:: setStreamName()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setMaxDataSize()

  .. function:: setNumFrames()


.. class:: XLinkOut
  :canonical: depthai.XLinkOut

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: input

    Output where the data received from the host is being sent

  .. function:: setStreamName()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setFpsLimit()


.. class:: ColorCamera
  :canonical: depthai.ColorCamera

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: video

    Output where the data received from the host is being sent

  .. attribute:: preview

    Output where the data received from the host is being sent

  .. attribute:: still

    Output where the data received from the host is being sent

  .. function:: setCamId()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: getCamId()

  .. function:: setColorOrder()

  .. function:: setInterleaved()

  .. function:: setFp16()

  .. function:: setPreviewSize()

  .. function:: setResolution()

  .. function:: setFps()


.. class:: MonoCamera
  :canonical: depthai.MonoCamera

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: out

    Output where the data received from the host is being sent

  .. function:: setCamId()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: getCamId()

  .. function:: setResolution()

  .. function:: setFps()


.. class:: NeuralNetwork
  :canonical: depthai.NeuralNetwork

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: input

    Output where the data received from the host is being sent

  .. attribute:: out

    Output where the data received from the host is being sent

  .. function:: setBlobPath()

    Sets the stream name, needed for :func:`getInputQueue` to transfer data to it.

  .. function:: setNumPoolFrames()


.. class:: ImageManip
  :canonical: depthai.ImageManip

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: inputConfig

    Output where the data received from the host is being sent

  .. attribute:: inputImage

    Output where the data received from the host is being sent

  .. attribute:: out

    Output where the data received from the host is being sent

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

  .. function:: setWidth()

  .. function:: getWidth()

  .. function:: setHeight()

  .. function:: getHeight()

  .. function:: setFrameRate()

  .. function:: getFrameRate()


.. class:: ColorCameraProperties
  :canonical: depthai.ColorCameraProperties

  Represents an XLink stream of data from host into device

  **Outputs**

  .. attribute:: camId

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

  .. attribute:: camId

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


