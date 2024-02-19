import depthai as dai
import cv2
import numpy as np

mouse_x, mouse_y = 0, 0


def on_mouse(event, x, y, *args):
    global mouse_x, mouse_y
    mouse_x = x
    mouse_y = y


device = dai.Device()
pipeline = dai.Pipeline()

# Thermal camera
thermal_cam = pipeline.create(dai.node.Camera)
width, height = -1, -1
thermal_found = False
for features in device.getConnectedCameraFeatures():
    if dai.CameraSensorType.THERMAL in features.supportedTypes:
        thermal_found = True
        thermal_cam.setBoardSocket(features.socket)
        width, height = features.width, features.height
        break
if not thermal_found:
    raise RuntimeError("No thermal camera found!")
thermal_cam.setPreviewSize(width, height)

# Output raw: FP16 temperature data (degrees Celsius)
xout_raw = pipeline.create(dai.node.XLinkOut)
xout_raw.setStreamName("thermal_raw")
thermal_cam.raw.link(xout_raw.input)

# Output preview,video, isp: RGB or NV12 or YUV420 thermal image.
xout_image = pipeline.create(dai.node.XLinkOut)
xout_image.setStreamName("image")
thermal_cam.preview.link(xout_image.input)
device.startPipeline(pipeline)

q_raw = device.getOutputQueue("thermal_raw", 2, False)
q_image = device.getOutputQueue("image", 2, False)


RAW_WINDOW_NAME = "temperature"
IMAGE_WINDOW_NAME = "image"
# Scale 4x and position one next to another
cv2.namedWindow(RAW_WINDOW_NAME, cv2.WINDOW_NORMAL)
cv2.namedWindow(IMAGE_WINDOW_NAME, cv2.WINDOW_NORMAL)
cv2.moveWindow(RAW_WINDOW_NAME, 0, 0)
cv2.resizeWindow(RAW_WINDOW_NAME, width * 4, height * 4)
cv2.moveWindow(IMAGE_WINDOW_NAME, width * 4, 0)
cv2.resizeWindow(IMAGE_WINDOW_NAME, width * 4, height * 4)
cv2.setMouseCallback(RAW_WINDOW_NAME, on_mouse)
cv2.setMouseCallback(IMAGE_WINDOW_NAME, on_mouse)

while True:
    in_raw = q_raw.get()
    in_img = q_image.get()

    # Retrieve one point of fp16 data
    frame = in_raw.getCvFrame().astype(np.float32)
    colormapped_frame = cv2.normalize(frame, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    colormapped_frame = cv2.applyColorMap(colormapped_frame, cv2.COLORMAP_MAGMA)
    if (
        mouse_x < 0
        or mouse_y < 0
        or mouse_x >= frame.shape[1]
        or mouse_y >= frame.shape[0]
    ):
        mouse_x = max(0, min(mouse_x, frame.shape[1] - 1))
        mouse_y = max(0, min(mouse_y, frame.shape[0] - 1))
    text_color = (255, 255, 255)
    # Draw crosshair
    cv2.line(
        colormapped_frame,
        (mouse_x - 10, mouse_y),
        (mouse_x + 10, mouse_y),
        text_color,
        1,
    )
    cv2.line(
        colormapped_frame,
        (mouse_x, mouse_y - 10),
        (mouse_x, mouse_y + 10),
        text_color,
        1,
    )
    # Draw deg C
    text = f"{frame[mouse_y, mouse_x]:.2f} deg C"
    put_text_left = mouse_x > colormapped_frame.shape[1] / 2
    cv2.putText(
        colormapped_frame,
        text,
        (mouse_x - 100 if put_text_left else mouse_x + 10, mouse_y - 10),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.5,
        text_color,
        1,
    )
    cv2.imshow(RAW_WINDOW_NAME, colormapped_frame)

    cv2.imshow(IMAGE_WINDOW_NAME, in_img.getCvFrame())

    if cv2.waitKey(1) == ord("q"):
        break
