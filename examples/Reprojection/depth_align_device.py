import numpy as np
import cv2
import depthai as dai
# from projector_3d import PointCloudVisualizer
import reprojection

FPS = 10

RGB_SOCKET = dai.CameraBoardSocket.RGB
LEFT_SOCKET = dai.CameraBoardSocket.LEFT
RIGHT_SOCKET = dai.CameraBoardSocket.RIGHT
ALIGN_SOCKET = LEFT_SOCKET

COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_1080_P
LEFT_RIGHT_RESOLUTION = dai.MonoCameraProperties.SensorResolution.THE_480_P
depthSize = (640, 400)
rgbSize = (1920, 1080)
# rgbSize = depthSize

# Used for colorizing the depth map
MIN_DEPTH = 500  # mm
MAX_DEPTH = 10000  # mm

device = dai.Device()

pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
right = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)
align = pipeline.create(dai.node.ImageAlign)

left.setResolution(LEFT_RIGHT_RESOLUTION)
left.setBoardSocket(LEFT_SOCKET)
left.setFps(FPS)

right.setResolution(LEFT_RIGHT_RESOLUTION)
right.setBoardSocket(RIGHT_SOCKET)
right.setFps(FPS)

camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(COLOR_RESOLUTION)
camRgb.setFps(FPS)

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.setExtendedDisparity(True)
stereo.setLeftRightCheck(True)
stereo.setDepthAlign(ALIGN_SOCKET)
stereo.enableDistortionCorrection(True)
stereo.setDepthAlignmentUseSpecTranslation(False)
stereo.setRectificationUseSpecTranslation(False)
stereo.setFocalLengthFromCalibration(True)
stereo.setDisparityToDepthUseSpecTranslation(False)

out.setStreamName("out")

sync.setSyncThreshold(0.05)

# Linking
camRgb.isp.link(sync.inputs["rgb"])
left.out.link(stereo.left)
right.out.link(stereo.right)
left.out.link(sync.inputs["left"])
right.out.link(sync.inputs["right"])
stereo.depth.link(align.input)
align.outputAligned.link(sync.inputs["depth"])
camRgb.isp.link(align.inputAlignTo)
sync.out.link(out.input)


def colorizeDepth(frameDepth, minDepth=MIN_DEPTH, maxDepth=MAX_DEPTH):
    invalidMask = frameDepth == 0
    # Log the depth, minDepth and maxDepth
    logDepth = np.log(frameDepth, where=frameDepth != 0)
    logMinDepth = np.log(minDepth)
    logMaxDepth = np.log(maxDepth)
    depthFrameColor = np.interp(logDepth, (logMinDepth, logMaxDepth), (0, 255)).astype(
        np.uint8
    )
    depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_JET)
    # Set invalid depth pixels to black
    depthFrameColor[invalidMask] = 0
    return depthFrameColor


rgbWeight = 0.4
depthWeight = 0.6


def updateBlendWeights(percent_rgb):
    """
    Update the rgb and depth weights used to blend depth/rgb image

    @param[in] percent_rgb The rgb weight expressed as a percentage (0..100)
    """
    global depthWeight
    global rgbWeight
    rgbWeight = float(percent_rgb) / 100.0
    depthWeight = 1.0 - rgbWeight


# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)
    queue = device.getOutputQueue("out", 8, False)

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    rgb_depth_window_name = "rgb-depth"

    cv2.namedWindow(rgb_depth_window_name)
    cv2.createTrackbar(
        "RGB Weight %",
        rgb_depth_window_name,
        int(rgbWeight * 100),
        100,
        updateBlendWeights,
    )

    try:
        calibData = device.readCalibration2()
        M1 = np.array(calibData.getCameraIntrinsics(ALIGN_SOCKET, *depthSize))
        D1 = np.array(calibData.getDistortionCoefficients(ALIGN_SOCKET))
        M2 = np.array(calibData.getCameraIntrinsics(RGB_SOCKET, *rgbSize))
        D2 = np.array(calibData.getDistortionCoefficients(RGB_SOCKET))

        T = (
                np.array(calibData.getCameraTranslationVector(ALIGN_SOCKET, RGB_SOCKET, False))
                * 10
        )  # to mm for matching the depth
        R = np.array(calibData.getCameraExtrinsics(ALIGN_SOCKET, RGB_SOCKET, False))[
            0:3, 0:3
            ]
        TARGET_MATRIX = M1

        lensPosition = calibData.getLensPosition(RGB_SOCKET)
    except:
        raise
    mapX, mapY = cv2.initUndistortRectifyMap(
        M2, D2, None, M2, rgbSize, cv2.CV_32FC1
    )

    while True:
        messageGroup: dai.MessageGroup = queue.get()
        frameRgb: dai.ImgFrame = messageGroup["rgb"]
        frameDepth: dai.ImgFrame = messageGroup["depth"]
        # Blend when both received
        if frameRgb is not None and frameDepth is not None:
            frameRgb = frameRgb.getCvFrame()
            # Undistort RGB frame
            frameRgb = cv2.remap(frameRgb, mapX, mapY, cv2.INTER_LINEAR)
            # Colorize the aligned depth
            alignedDepthColorized = colorizeDepth(frameDepth.getFrame())
            cv2.imshow("depth", alignedDepthColorized)

            blended = cv2.addWeighted(frameRgb, rgbWeight, alignedDepthColorized, depthWeight, 0)
            cv2.imshow(rgb_depth_window_name, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break
