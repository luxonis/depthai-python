import depthai as dai
import time

enable_4k = True

# Start defining a pipeline
pipeline = dai.Pipeline()

# Define a source - color camera
cam_rgb = pipeline.createColorCamera()
cam_rgb.setBoardSocket(dai.CameraBoardSocket.RGB)
cam_rgb.setInterleaved(False)
cam_rgb.initialControl.setManualFocus(130)

if enable_4k:
    cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)
    cam_rgb.setIspScale(1, 2)
else:
    cam_rgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)

# Create an UVC (USB Video Class) output node. It needs 1920x1080, NV12 input
uvc = pipeline.createUVC()
cam_rgb.video.link(uvc.input)

(found, deviceInfo) = dai.DeviceBootloader.getFirstAvailableDevice()
bootloader = dai.DeviceBootloader(deviceInfo)
progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
bootloader.flash(progress, pipeline)

print("Pipeline flashed successfully")