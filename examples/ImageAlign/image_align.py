import cv2
import depthai as dai

# This is an interactive example that shows how two frame sources without depth information.

FPS = 30.0

RGB_SOCKET = dai.CameraBoardSocket.CAM_A
LEFT_SOCKET = dai.CameraBoardSocket.CAM_B
RIGHT_SOCKET = dai.CameraBoardSocket.CAM_C
ALIGN_SOCKET = LEFT_SOCKET

COLOR_RESOLUTION = dai.ColorCameraProperties.SensorResolution.THE_1080_P
LEFT_RIGHT_RESOLUTION = dai.MonoCameraProperties.SensorResolution.THE_720_P

device = dai.Device()
pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
right = pipeline.create(dai.node.MonoCamera)
sync = pipeline.create(dai.node.Sync)
out = pipeline.create(dai.node.XLinkOut)
align = pipeline.create(dai.node.ImageAlign)
cfgIn = pipeline.create(dai.node.XLinkIn)

left.setResolution(LEFT_RIGHT_RESOLUTION)
left.setBoardSocket(LEFT_SOCKET)
left.setFps(FPS)

right.setResolution(LEFT_RIGHT_RESOLUTION)
right.setBoardSocket(RIGHT_SOCKET)
right.setFps(FPS)

camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(COLOR_RESOLUTION)
camRgb.setFps(FPS)
camRgb.setIspScale(1, 3)

out.setStreamName("out")

sync.setSyncThreshold(1/FPS)

align.setOutputSize(1920, 1080)

cfgIn.setStreamName("config")

cfg = align.initialConfig.get()
staticDepthPlane = cfg.staticDepthPlane

# Linking
align.outputAligned.link(sync.inputs["aligned"])
camRgb.isp.link(sync.inputs["rgb"])
camRgb.isp.link(align.inputAlignTo)
left.out.link(align.input)
sync.out.link(out.input)
cfgIn.out.link(align.inputConfig)


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

def updateDepthPlane(depth):
    global staticDepthPlane
    staticDepthPlane = depth

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)
    queue = device.getOutputQueue("out", 8, False)
    cfgQ = device.getInputQueue("config")

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    rgb_depth_window_name = "rgb-left"

    # Set the window to be resizable and the initial size
    cv2.namedWindow(rgb_depth_window_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(rgb_depth_window_name, 1280, 720)
    cv2.createTrackbar(
        "RGB Weight %",
        rgb_depth_window_name,
        int(rgbWeight * 100),
        100,
        updateBlendWeights,
    )
    cv2.createTrackbar(
        "Static Depth Plane",
        rgb_depth_window_name,
        0,
        2000,
        updateDepthPlane,
    )
    while True:
        messageGroup = queue.get()
        assert isinstance(messageGroup, dai.MessageGroup)
        frameRgb = messageGroup["rgb"]
        assert isinstance(frameRgb, dai.ImgFrame)
        leftAligned = messageGroup["aligned"]
        assert isinstance(leftAligned, dai.ImgFrame)

        frameRgb = frameRgb.getCvFrame()
        # Colorize the aligned depth
        left = leftAligned.getCvFrame()

        if len(left.shape) == 2:
            left = cv2.cvtColor(left, cv2.COLOR_GRAY2BGR)
        if left.shape != frameRgb.shape:
            left = cv2.resize(left, (frameRgb.shape[1], frameRgb.shape[0]))

        blended = cv2.addWeighted(frameRgb, rgbWeight, left, depthWeight, 0)
        cv2.imshow(rgb_depth_window_name, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break

        cfg.staticDepthPlane = staticDepthPlane
        cfgQ.send(cfg)