import depthai as dai
import cv2
import numpy as np

mouse_x, mouse_y = 0, 0


def mouse_callback(event, x, y, flags, param):
    if event == cv2.EVENT_MOUSEMOVE:
        global mouse_x, mouse_y
        mouse_x, mouse_y = x, y


WINDOW_NAME_IMAGE = "thermal_image"
WINDOW_NAME_TEMP = "thermal_temperature"

with dai.Device() as device:
    pipeline = dai.Pipeline()
    thermal = pipeline.createColorCamera()
    thermal.setBoardSocket(dai.CameraBoardSocket.CAM_B)
    xlink = pipeline.createXLinkOut()
    xlink.setStreamName("thermal_image")
    thermal.preview.link(xlink.input)

    xlink_raw = pipeline.createXLinkOut()
    xlink_raw.setStreamName("thermal_temperature")
    thermal.raw.link(xlink_raw.input)
    device.startPipeline(pipeline)
    q = device.getOutputQueue("thermal_image", 5, False)
    q_raw = device.getOutputQueue("thermal_temperature", 5, False)
    cv2.namedWindow(WINDOW_NAME_IMAGE)
    cv2.namedWindow(WINDOW_NAME_TEMP)
    cv2.setMouseCallback(WINDOW_NAME_TEMP, mouse_callback)
    while True:
        inFrame = q.get()
        frame = inFrame.getCvFrame()
        cv2.imshow(WINDOW_NAME_IMAGE, frame)
        inFrameRaw = q_raw.tryGet()
        if inFrameRaw is not None:
            frameRaw = (
                np.array(inFrameRaw.getCvFrame()).view(np.float32).reshape((192, 256))
            )
            frameRaw += 96
            min_val, max_val = frameRaw.min(), frameRaw.max()
            normalized_frame = (frameRaw - min_val) / (max_val - min_val)
            frame_8bit = np.uint8(normalized_frame * 255)
            colored_frame = cv2.applyColorMap(frame_8bit, cv2.COLORMAP_MAGMA)
            colored_frame = cv2.circle(
                colored_frame, (mouse_x, mouse_y), 5, (255, 255, 255), -1
            )
            colored_frame = cv2.putText(
                colored_frame,
                f"{frameRaw[mouse_y, mouse_x]:.2f} deg C",
                (mouse_x + 10, mouse_y - 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (255, 255, 255),
            )
            cv2.imshow(WINDOW_NAME_TEMP, colored_frame)

        if cv2.waitKey(1) == ord("q"):
            break
