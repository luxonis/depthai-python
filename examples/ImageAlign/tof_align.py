import numpy as np
import cv2
import depthai as dai
import time

# This example is intended to run unchanged on an OAK-D-SR-PoE camera
FPS = 30.0

RGB_SOCKET = dai.CameraBoardSocket.CAM_C
TOF_SOCKET = dai.CameraBoardSocket.CAM_A
ALIGN_SOCKET = RGB_SOCKET

class FPSCounter:
    def __init__(self):
        self.frameTimes = []

    def tick(self):
        now = time.time()
        self.frameTimes.append(now)
        self.frameTimes = self.frameTimes[-100:]

    def get_fps(self):
        if len(self.frameTimes) <= 1:
            return 0
        # Calculate the FPS
        return (len(self.frameTimes) - 1) / (self.frameTimes[-1] - self.frameTimes[0])



pipeline = dai.Pipeline()
# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
tof = pipeline.create(dai.node.ToF)
camTof = pipeline.create(dai.node.Camera)
sync = pipeline.create(dai.node.Sync)
align = pipeline.create(dai.node.ImageAlign)
out = pipeline.create(dai.node.XLinkOut)

# ToF settings
camTof.setFps(FPS)
camTof.setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)
camTof.setBoardSocket(TOF_SOCKET)

# rgb settings
camRgb.setBoardSocket(RGB_SOCKET)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_800_P)
camRgb.setFps(FPS)
camRgb.setIspScale(1, 2)

out.setStreamName("out")

sync.setSyncThreshold((1 / FPS))

# Linking
camRgb.isp.link(sync.inputs["rgb"])
camTof.raw.link(tof.input)
tof.depth.link(align.input)
align.outputAligned.link(sync.inputs["depth_aligned"])
sync.inputs["rgb"].setBlocking(False)
camRgb.isp.link(align.inputAlignTo)
sync.out.link(out.input)

def colorizeDepth(frameDepth):
    invalidMask = frameDepth == 0
    # Log the depth, minDepth and maxDepth
    try:
        minDepth = np.percentile(frameDepth[frameDepth != 0], 3)
        maxDepth = np.percentile(frameDepth[frameDepth != 0], 95)
        logDepth = np.log(frameDepth, where=frameDepth != 0)
        logMinDepth = np.log(minDepth)
        logMaxDepth = np.log(maxDepth)
        np.nan_to_num(logDepth, copy=False, nan=logMinDepth)
        # Clip the values to be in the 0-255 range
        logDepth = np.clip(logDepth, logMinDepth, logMaxDepth)

        # Interpolate only valid logDepth values, setting the rest based on the mask
        depthFrameColor = np.interp(logDepth, (logMinDepth, logMaxDepth), (0, 255))
        depthFrameColor = np.nan_to_num(depthFrameColor)
        depthFrameColor = depthFrameColor.astype(np.uint8)
        depthFrameColor = cv2.applyColorMap(depthFrameColor, cv2.COLORMAP_JET)
        # Set invalid depth pixels to black
        depthFrameColor[invalidMask] = 0
    except IndexError:
        # Frame is likely empty
        depthFrameColor = np.zeros((frameDepth.shape[0], frameDepth.shape[1], 3), dtype=np.uint8)
    except Exception as e:
        raise e
    return depthFrameColor


rgbWeight = 0.4
depthWeight = 0.6


def updateBlendWeights(percentRgb):
    """
    Update the rgb and depth weights used to blend depth/rgb image

    @param[in] percent_rgb The rgb weight expressed as a percentage (0..100)
    """
    global depthWeight
    global rgbWeight
    rgbWeight = float(percentRgb) / 100.0
    depthWeight = 1.0 - rgbWeight



# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    queue = device.getOutputQueue("out", 8, False)

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    rgbDepthWindowName = "rgb-depth"

    cv2.namedWindow(rgbDepthWindowName)
    cv2.createTrackbar(
        "RGB Weight %",
        rgbDepthWindowName,
        int(rgbWeight * 100),
        100,
        updateBlendWeights,
    )
    fpsCounter = FPSCounter()
    while True:
        messageGroup = queue.get()
        fpsCounter.tick()
        assert isinstance(messageGroup, dai.MessageGroup)
        frameRgb = messageGroup["rgb"]
        assert isinstance(frameRgb, dai.ImgFrame)
        frameDepth = messageGroup["depth_aligned"]
        assert isinstance(frameDepth, dai.ImgFrame)

        sizeRgb = frameRgb.getData().size
        sizeDepth = frameDepth.getData().size
        # Blend when both received
        if frameDepth is not None:
            cvFrame = frameRgb.getCvFrame()
            # Colorize the aligned depth
            alignedDepthColorized = colorizeDepth(frameDepth.getFrame())
            # Resize depth to match the rgb frame
            cv2.putText(
                alignedDepthColorized,
                f"FPS: {fpsCounter.get_fps():.2f}",
                (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (255, 255, 255),
                2,
            )
            cv2.imshow("depth", alignedDepthColorized)

            blended = cv2.addWeighted(
                cvFrame, rgbWeight, alignedDepthColorized, depthWeight, 0
            )
            cv2.imshow(rgbDepthWindowName, blended)

        key = cv2.waitKey(1)
        if key == ord("q"):
            break