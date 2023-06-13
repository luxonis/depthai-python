import numpy as np
import cv2
import depthai as dai
from numba import jit, prange

@jit(nopython=True, parallel=True)
def reprojection(depth_image, depth_camera_intrinsics, camera_extrinsics, color_camera_intrinsics, hardware_rectify=False):
    height = len(depth_image)
    width = len(depth_image[0])
    
    image = np.zeros((height, width), np.uint8)

    for i in prange(0, height):
        for j in prange(0, width):
            d = depth_image[i][j]

            # converte pixel to 3d point
            x = (j - depth_camera_intrinsics[0][2]) * d / depth_camera_intrinsics[0][0]
            y = (i - depth_camera_intrinsics[1][2]) * d / depth_camera_intrinsics[1][1]
            z = d

            # apply transformation
            if hardware_rectify:
                x1 = x + camera_extrinsics[0][3]
                y1 = y 
                z1 = z 
            else:
                x1 = camera_extrinsics[0][0] * x + camera_extrinsics[0][1] * y + camera_extrinsics[0][2] * z + camera_extrinsics[0][3]
                y1 = camera_extrinsics[1][0] * x + camera_extrinsics[1][1] * y + camera_extrinsics[1][2] * z + camera_extrinsics[1][3]
                z1 = camera_extrinsics[2][0] * x + camera_extrinsics[2][1] * y + camera_extrinsics[2][2] * z + camera_extrinsics[2][3]
            
            # project 3d point to pixel
            if z1 == 0:
                continue

            u = color_camera_intrinsics[0][0] * (x1  / z1) + color_camera_intrinsics[0][2]
            v = color_camera_intrinsics[1][1] * (y1  / z1) + color_camera_intrinsics[1][2]
            if u >= 0 and u < len(image[0]) and v >= 0 and v < len(image):
                image[int(v)][int(u)] = z1

    return image

rgbWeight = 0.4
depthWeight = 0.6

def updateBlendWeights(percent_rgb):
    """
    Update the rgb and depth weights used to blend depth/rgb image

    @param[in] percent_rgb The rgb weight expressed as a percentage (0..100)
    """
    global depthWeight
    global rgbWeight
    rgbWeight = float(percent_rgb)/100.0
    depthWeight = 1.0 - rgbWeight


pipeline = dai.Pipeline()
device = dai.Device()
queueNames = []

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
left = pipeline.create(dai.node.MonoCamera)
right = pipeline.create(dai.node.MonoCamera)
stereo = pipeline.create(dai.node.StereoDepth)

rgbOut = pipeline.create(dai.node.XLinkOut)
depthOut = pipeline.create(dai.node.XLinkOut)

rgbOut.setStreamName("rgb")
queueNames.append("rgb")
depthOut.setStreamName("disp")
queueNames.append("disp")

hardware_rectify = True 

fps = 30

#Properties
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setFps(fps)
camRgb.setIspScale(2, 3)

try:    
    calibData = device.readCalibration2()
    rgb_intrinsics = calibData.getCameraIntrinsics(dai.CameraBoardSocket.CAM_A, 1280, 720)
    depth_intrinsics = calibData.getCameraIntrinsics(calibData.getStereoRightCameraId(), 1280, 720)
    rgb_extrinsics = calibData.getCameraExtrinsics(calibData.getStereoRightCameraId(), dai.CameraBoardSocket.CAM_A)

    depth_intrinsics = np.asarray(depth_intrinsics).reshape(3, 3)
    rgb_extrinsics = np.asarray(rgb_extrinsics).reshape(4, 4)
    rgb_extrinsics[:,3] *= 10
    rgb_intrinsics = np.asarray(rgb_intrinsics).reshape(3, 3)

    lensPosition = calibData.getLensPosition(dai.CameraBoardSocket.CAM_A)
    if lensPosition:
        camRgb.initialControl.setManualFocus(lensPosition)
except:
    raise

left.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
left.setCamera("left")
left.setFps(fps)
right.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
right.setCamera("right")
right.setFps(fps)

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)

# Linking
camRgb.isp.link(rgbOut.input)
left.out.link(stereo.left)
right.out.link(stereo.right)
stereo.depth.link(depthOut.input)

# Connect to device and start pipeline
with device:
    device.startPipeline(pipeline)

    frameRgb = None
    frameDisp = None

    # Configure windows; trackbar adjusts blending ratio of rgb/depth
    rgb_depth_window_name = 'rgb-depth'

    cv2.namedWindow(rgb_depth_window_name)
    cv2.createTrackbar('RGB Weight %', rgb_depth_window_name, int(rgbWeight*100), 100, updateBlendWeights)

    while True:
        latestPacket = {}
        latestPacket["rgb"] = None
        latestPacket["disp"] = None

        queueEvents = device.getQueueEvents(("rgb", "disp"))
        for queueName in queueEvents:
            packets = device.getOutputQueue(queueName).tryGetAll()
            if len(packets) > 0:
                latestPacket[queueName] = packets[-1]

        if latestPacket["rgb"] is not None:
            frameRgb = latestPacket["rgb"].getCvFrame()

        if latestPacket["disp"] is not None:
            frameDisp = latestPacket["disp"].getFrame()
        
        # Blend when both received
        if frameRgb is not None and frameDisp is not None:

            if hardware_rectify:
                rectification_map = cv2.initUndistortRectifyMap(depth_intrinsics, None, rgb_extrinsics[0:3, 0:3], depth_intrinsics, (1280, 720), cv2.CV_16SC2)
                frameDisp = cv2.remap(frameDisp, rectification_map[0], rectification_map[1], cv2.INTER_LINEAR)

            frameDisp = reprojection(frameDisp, depth_intrinsics, rgb_extrinsics, rgb_intrinsics, hardware_rectify)

            frameDisp = cv2.cvtColor(frameDisp, cv2.COLOR_GRAY2BGR)

            blended = cv2.addWeighted(frameRgb, rgbWeight, frameDisp, depthWeight, 0)
            cv2.imshow(rgb_depth_window_name, blended)
            frameRgb = None
            frameDisp = None

        if cv2.waitKey(1) == ord('q'):
            break
