import depthai as dai
import cv2
import numpy as np

mouseX, mouseY = 0, 0


def onMouse(event, x, y, *args):
    global mouseX, mouseY
    mouseX = x
    mouseY = y


device = dai.Device()
pipeline = dai.Pipeline()

# Thermal camera
thermalCam = pipeline.create(dai.node.Camera)
thermalCam.setFps(25) # Limit to 25 to match what the sensor can do, capped even if left at default, but warns.
width, height = -1, -1
thermalFound = False
for features in device.getConnectedCameraFeatures():
    if dai.CameraSensorType.THERMAL in features.supportedTypes:
        thermalFound = True
        thermalCam.setBoardSocket(features.socket)
        width, height = features.width, features.height
        break
if not thermalFound:
    raise RuntimeError("No thermal camera found!")
thermalCam.setPreviewSize(width, height)

# Output raw: FP16 temperature data (degrees Celsius)
xoutRaw = pipeline.create(dai.node.XLinkOut)
xoutRaw.setStreamName("thermal_raw")
thermalCam.raw.link(xoutRaw.input)

# Output preview,video, isp: RGB or NV12 or YUV420 thermal image.
xoutImage = pipeline.create(dai.node.XLinkOut)
xoutImage.setStreamName("image")
thermalCam.preview.link(xoutImage.input)
device.startPipeline(pipeline)

qRaw = device.getOutputQueue("thermal_raw", 2, False)
qImage = device.getOutputQueue("image", 2, False)


RAW_WINDOW_NAME = "temperature"
IMAGE_WINDOW_NAME = "image"
# Scale 4x and position one next to another
cv2.namedWindow(RAW_WINDOW_NAME, cv2.WINDOW_NORMAL)
cv2.namedWindow(IMAGE_WINDOW_NAME, cv2.WINDOW_NORMAL)
cv2.moveWindow(RAW_WINDOW_NAME, 0, 0)
cv2.resizeWindow(RAW_WINDOW_NAME, width * 4, height * 4)
cv2.moveWindow(IMAGE_WINDOW_NAME, width * 4, 0)
cv2.resizeWindow(IMAGE_WINDOW_NAME, width * 4, height * 4)
cv2.setMouseCallback(RAW_WINDOW_NAME, onMouse)
cv2.setMouseCallback(IMAGE_WINDOW_NAME, onMouse)

while True:
    inRaw = qRaw.get()
    inImg = qImage.get()

    # Retrieve one point of fp16 data
    frame = inRaw.getCvFrame().astype(np.float32)
    colormappedFrame = cv2.normalize(frame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    colormappedFrame = cv2.applyColorMap(colormappedFrame, cv2.COLORMAP_MAGMA)
    if (
        mouseX < 0
        or mouseY < 0
        or mouseX >= frame.shape[1]
        or mouseY >= frame.shape[0]
    ):
        mouseX = max(0, min(mouseX, frame.shape[1] - 1))
        mouseY = max(0, min(mouseY, frame.shape[0] - 1))
    textColor = (255, 255, 255)
    # Draw crosshair
    cv2.line(
        colormappedFrame,
        (mouseX - 10, mouseY),
        (mouseX + 10, mouseY),
        textColor,
        1,
    )
    cv2.line(
        colormappedFrame,
        (mouseX, mouseY - 10),
        (mouseX, mouseY + 10),
        textColor,
        1,
    )
    # Draw deg C
    text = f"{frame[mouseY, mouseX]:.2f} deg C"
    putTextLeft = mouseX > colormappedFrame.shape[1] / 2
    cv2.putText(
        colormappedFrame,
        text,
        (mouseX - 100 if putTextLeft else mouseX + 10, mouseY - 10),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.5,
        textColor,
        1,
    )
    cv2.imshow(RAW_WINDOW_NAME, colormappedFrame)

    cv2.imshow(IMAGE_WINDOW_NAME, inImg.getCvFrame())

    if cv2.waitKey(1) == ord("q"):
        break
