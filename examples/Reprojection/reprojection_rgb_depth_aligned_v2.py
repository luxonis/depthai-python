import numpy as np
import cv2
import depthai as dai
from projector_3d import PointCloudVisualizer
import reprojection

FPS = 10

RGB_SOCKET = dai.CameraBoardSocket.RGB
LEFT_SOCKET = dai.CameraBoardSocket.LEFT
RIGHT_SOCKET = dai.CameraBoardSocket.RIGHT

COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_1080_P
LEFT_RIGHT_RESOLUTION = dai.MonoCameraProperties.SensorResolution.THE_800_P
depthSize = (1280, 800)
rgbSize = (1920, 1080)

# Used for colorizing the depth map
MIN_DEPTH = 500  # mm
MAX_DEPTH = 10000  # mm

device = dai.Device()

try:
    calibData = device.readCalibration2()
    M1 = np.array(calibData.getCameraIntrinsics(LEFT_SOCKET, *depthSize))
    D1 = np.array(calibData.getDistortionCoefficients(LEFT_SOCKET))
    M2 = np.array(calibData.getCameraIntrinsics(RGB_SOCKET, *rgbSize))
    D2 = np.array(calibData.getDistortionCoefficients(RGB_SOCKET))

    T = (
        np.array(calibData.getCameraTranslationVector(LEFT_SOCKET, RGB_SOCKET, False))
        * 10
    )  # to mm for matching the depth
    R = np.array(calibData.getCameraExtrinsics(LEFT_SOCKET, RGB_SOCKET, False))[
        0:3, 0:3
    ]
    TARGET_MATRIX = M1

    lensPosition = calibData.getLensPosition(RGB_SOCKET)
except:
    raise


pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
right = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)
sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)

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
stereo.setDepthAlign(LEFT_SOCKET)
stereo.enableDistortionCorrection(True)
stereo.setDepthAlignmentUseSpecTranslation(False)
stereo.setRectificationUseSpecTranslation(False)
stereo.setFocalLengthFromCalibration(True)
stereo.setDisparityToDepthUseSpecTranslation(False)

out.setStreamName("out")

# Linking
camRgb.isp.link(sync.inputs["rgb"])
left.out.link(stereo.left)
right.out.link(stereo.right)
left.out.link(sync.inputs["left"])
right.out.link(sync.inputs["right"])
stereo.depth.link(sync.inputs["depth"])
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


def getAlignedDepth(frameDepth):
    R1, R2, _, _, _, _, _ = cv2.stereoRectify(M1, D1, M2, D2, (100, 100), R, T)  # The (100,100) doesn't matter as it is not used for calculating the rotation matrices
    leftMapX, leftMapY = cv2.initUndistortRectifyMap(M1, None, R1, TARGET_MATRIX, depthSize, cv2.CV_32FC1)
    depthRect = cv2.remap(frameDepth, leftMapX, leftMapY, cv2.INTER_NEAREST)
    newR = np.dot(R2, np.dot(R, R1.T))  # Should be very close to identity
    newT = np.dot(R2, T)
    combinedExtrinsics = np.eye(4)
    combinedExtrinsics[0:3, 0:3] = newR
    combinedExtrinsics[0:3, 3] = newT
    depthAligned = reprojection.reprojection(depthRect, TARGET_MATRIX, combinedExtrinsics, TARGET_MATRIX)
    # Rotate the depth to the RGB frame
    R_back = R2.T
    mapX, mapY = cv2.initUndistortRectifyMap(TARGET_MATRIX, None, R_back, M2, rgbSize, cv2.CV_32FC1)
    outputAligned = cv2.remap(depthAligned, mapX, mapY, cv2.INTER_NEAREST)
    return outputAligned


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
    vis = PointCloudVisualizer(M2, *rgbSize)
    while True:
        messageGroup: dai.MessageGroup = queue.get()
        frameRgb: dai.ImgFrame = messageGroup["rgb"]
        frameDepth: dai.ImgFrame = messageGroup["depth"]
        # Blend when both received
        if frameRgb is not None and frameDepth is not None:
            frameRgb = frameRgb.getCvFrame()
            cv2.imshow("rgb", frameRgb)
            cv2.imshow("depth", colorizeDepth(frameDepth.getCvFrame()))

            alignedDepth = getAlignedDepth(frameDepth.getFrame())
            pclDepth = alignedDepth.copy()
            rgb = cv2.cvtColor(frameRgb, cv2.COLOR_BGR2RGB)
            vis.rgbd_to_projection(pclDepth, rgb)
            vis.visualize_pcd()
            # Colorize the aligned depth
            alignedDepthColorized = colorizeDepth(alignedDepth)

            # Undistort the RGB frame
            mapX, mapY = cv2.initUndistortRectifyMap(
                M2, D2, None, M2, rgbSize, cv2.CV_32FC1
            )
            frameRgb = cv2.remap(frameRgb, mapX, mapY, cv2.INTER_LINEAR)

            blended = cv2.addWeighted(frameRgb, rgbWeight, alignedDepthColorized, depthWeight, 0)
            cv2.imshow(rgb_depth_window_name, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break
