import numpy as np
import cv2
import depthai as dai
import reprojection
from projector_3d import PointCloudVisualizer
from datetime import timedelta

FPS = 20

RGB_SOCKET = dai.CameraBoardSocket.CAM_C
LEFT_SOCKET = dai.CameraBoardSocket.CAM_A

COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_800_P
tofSize = (640, 480)
rgbSize = (1280, 800)

FPPN_ENABLE = True
WIGGLE_ENABLE = True
TEMPERATURE_ENABLE = False
OPTICAL_ENABLE = True

# Used for colorizing the depth map
MIN_DEPTH = 500  # mm
MAX_DEPTH = 10000  # mm


device = dai.Device()

try:
    calibData = device.readCalibration2()
    M1 = np.array(calibData.getCameraIntrinsics(LEFT_SOCKET, *tofSize))
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
tofIn = pipeline.create(dai.node.Camera)
tof = pipeline.create(dai.node.ToF)
sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)


tofIn.setBoardSocket(LEFT_SOCKET)
tofIn.properties.numFramesPoolRaw = 5
tofIn.setFps(FPS)
tofIn.setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)

tofConfig = tof.initialConfig.get()
# tofConfig.depthParams.freqModUsed = dai.RawToFConfig.DepthParams.TypeFMod.MIN
# tofConfig.depthParams.freqModUsed = dai.RawToFConfig.DepthParams.TypeFMod.MAX
# tofConfig.depthParams.avgPhaseShuffle = False
# tofConfig.depthParams.minimumAmplitude = 3.0
tofConfig.enableFPPNCorrection = FPPN_ENABLE
tofConfig.enableOpticalCorrection = OPTICAL_ENABLE
tofConfig.enableWiggleCorrection = WIGGLE_ENABLE
tofConfig.enableTemperatureCorrection = TEMPERATURE_ENABLE
tofConfig.median = dai.MedianFilter.KERNEL_5x5

tofConfig.enablePhaseUnwrapping = True
tofConfig.phaseUnwrappingLevel = 4
tof.initialConfig.set(tofConfig)

camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(COLOR_RESOLUTION)
camRgb.setFps(FPS)

out.setStreamName("out")

sync.setSyncThreshold(timedelta(milliseconds=20))

# Linking
camRgb.isp.link(sync.inputs["rgb"])
tofIn.raw.link(tof.input)
tof.depth.link(sync.inputs["depth"])
sync.out.link(out.input)


def colorizeDepth(frameDepth, minDepth=MIN_DEPTH, maxDepth=MAX_DEPTH):
    invalidMask = frameDepth == 0
    depthFrameColor = np.interp(frameDepth, (minDepth, maxDepth), (0, 255)).astype(
        np.uint8
    )
    depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_JET)
    # Set invalid depth pixels to black
    depthFrameColor[invalidMask] = 0
    return depthFrameColor


def getAlignedDepth(frameDepth):
    R1, R2, _, _, _, _, _ = cv2.stereoRectify(M1, D1, M2, D2, (100, 100), R, T)  # The (100,100) doesn't matter as it is not used for calculating the rotation matrices
    leftMapX, leftMapY = cv2.initUndistortRectifyMap(M1, D1, R1, TARGET_MATRIX, tofSize, cv2.CV_32FC1)
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
            # visualise Point Cloud
            rgb = cv2.cvtColor(frameRgb, cv2.COLOR_BGR2RGB)
            vis.rgbd_to_projection(alignedDepth, rgb)
            vis.visualize_pcd()

            # Colorize the aligned depth
            alignedDepth = colorizeDepth(alignedDepth)

            # Undistort the RGB frame
            mapX, mapY = cv2.initUndistortRectifyMap(
                M2, D2, None, M2, rgbSize, cv2.CV_32FC1
            )
            frameRgb = cv2.remap(frameRgb, mapX, mapY, cv2.INTER_LINEAR)
            blended = cv2.addWeighted(frameRgb, rgbWeight, alignedDepth, depthWeight, 0)
            cv2.imshow(rgb_depth_window_name, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break
