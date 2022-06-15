#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
import argparse
from pathlib import Path

try:
    from projector_3d import PointCloudVisualizer
except ImportError as e:
    raise ImportError(
        f"\033[1;5;31mError occured when importing PCL projector: {e}")

parser = argparse.ArgumentParser()
parser.add_argument(
    "-res",
    "--resolution",
    type=str,
    default="720",
    help="Sets the resolution on mono cameras. Options: 800 | 720 | 400",
)
parser.add_argument(
    "-md",
    "--mesh_dir",
    type=str,
    default=None,
    help="Output directory for mesh files. If not specified mesh files won't be saved",
)
parser.add_argument(
    "-lm",
    "--load_mesh",
    default=False,
    action="store_true",
    help="Read camera intrinsics, generate mesh files and load them into the stereo node.",
)
parser.add_argument(
    "-rect",
    "--out_rectified",
    default=False,
    action="store_true",
    help="Generate and display rectified streams",
)
parser.add_argument(
    "-lr",
    "--lrcheck",
    default=False,
    action="store_true",
    help="Better handling for occlusions",
)
parser.add_argument(
    "-e",
    "--extended",
    default=False,
    action="store_true",
    help="Closer-in minimum depth, disparity range is doubled",
)
parser.add_argument(
    "-s",
    "--subpixel",
    default=False,
    action="store_true",
    help="Better accuracy for longer distance, fractional disparity 32-levels",
)
parser.add_argument(
    "-m",
    "--median",
    type=str,
    default="7x7",
    help="Choose the size of median filtering. Options: OFF | 3x3 | 5x5 | 7x7 (default)",
)
parser.add_argument(
    "-d",
    "--depth",
    default=False,
    action="store_true",
    help="Display depth frames",
)
args = parser.parse_args()

resolutionMap = {"800": (1280, 800), "720": (1280, 720), "400": (640, 400)}
if args.resolution not in resolutionMap:
    exit("Unsupported resolution!")

resolution = resolutionMap[args.resolution]
meshDirectory = args.mesh_dir  # Output dir for mesh files
generateMesh = args.load_mesh  # Load mesh files

outRectified = args.out_rectified  # Output and display rectified streams
lrcheck = args.lrcheck  # Better handling for occlusions
extended = args.extended  # Closer-in minimum depth, disparity range is doubled
subpixel = args.subpixel  # Better accuracy for longer distance, fractional disparity 32-levels
depth = args.depth  # Display depth frames

medianMap = {
    "OFF": dai.StereoDepthProperties.MedianFilter.MEDIAN_OFF,
    "3x3": dai.StereoDepthProperties.MedianFilter.KERNEL_3x3,
    "5x5": dai.StereoDepthProperties.MedianFilter.KERNEL_5x5,
    "7x7": dai.StereoDepthProperties.MedianFilter.KERNEL_7x7,
}
if args.median not in medianMap:
    exit("Unsupported median size!")

median = medianMap[args.median]

print("StereoDepth config options:")
print("    Resolution:  ", resolution)
print("    Left-Right check:  ", lrcheck)
print("    Extended disparity:", extended)
print("    Subpixel:          ", subpixel)
print("    Median filtering:  ", median)
print("    Generating mesh files:  ", generateMesh)
print("    Outputting mesh files to:  ", meshDirectory)

points = None
def cb(event, x, y, flags, param):
    global points
    if event == cv2.EVENT_LBUTTONUP:
        if points == (x, y):
            points = None # Clear
        else:
            points = (x, y)

cv2.namedWindow("disparity")
cv2.setMouseCallback("disparity", cb)
# cv2.setMouseCallback("depth", cb)
colormap = cv2.applyColorMap(np.arange(256, dtype=np.uint8), cv2.COLORMAP_JET)

def getMesh(calibData):
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.LEFT))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, resolution[0], resolution[1]))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.RIGHT))
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    tranformation = np.array(calibData.getCameraExtrinsics(dai.CameraBoardSocket.LEFT, dai.CameraBoardSocket.RIGHT))
    R = tranformation[:3, :3]
    T = tranformation[:3, 3]
    print(resolution)
    print(T)
    print(R)
    # R1, R2, P1, P2, Q, validPixROI1, validPixROI2 = cv2.stereoRectify(
    #                                                             M1,
    #                                                             d1,
    #                                                             M2,
    #                                                             d2,
    #                                                             resolution, R, T)
    # print(M1)
    # print(P1)
    # print(M2)
    # print(P2)

    P1 = np.array([[328.514646,   0.0      ,  306.177917,   0.0      ],
                   [  0.0     ,  328.514646,  251.244843,   0.0      ],
                   [  0.0     ,    0.0     ,     1.0,       0.0      ]])
    P2 = np.array([[328.514646,   0.0      ,  306.177917,   -1503.753379],
                   [  0.0     ,  328.514646,  251.244843,   0.0      ],
                   [  0.0     ,    0.0     ,     1.0,       0.0      ]])


    mapXL, mapYL = cv2.initUndistortRectifyMap(M1, d1, R1, P1, resolution, cv2.CV_32FC1)
    mapXR, mapYR = cv2.initUndistortRectifyMap(M2, d2, R2, P2, resolution, cv2.CV_32FC1)

    meshCellSize = 16
    meshLeft = []
    meshRight = []

    for y in range(mapXL.shape[0] + 1):
        if y % meshCellSize == 0:
            rowLeft = []
            rowRight = []
            for x in range(mapXL.shape[1] + 1):
                if x % meshCellSize == 0:
                    if y == mapXL.shape[0] and x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y - 1, x - 1])
                        rowLeft.append(mapXL[y - 1, x - 1])
                        rowRight.append(mapYR[y - 1, x - 1])
                        rowRight.append(mapXR[y - 1, x - 1])
                    elif y == mapXL.shape[0]:
                        rowLeft.append(mapYL[y - 1, x])
                        rowLeft.append(mapXL[y - 1, x])
                        rowRight.append(mapYR[y - 1, x])
                        rowRight.append(mapXR[y - 1, x])
                    elif x == mapXL.shape[1]:
                        rowLeft.append(mapYL[y, x - 1])
                        rowLeft.append(mapXL[y, x - 1])
                        rowRight.append(mapYR[y, x - 1])
                        rowRight.append(mapXR[y, x - 1])
                    else:
                        rowLeft.append(mapYL[y, x])
                        rowLeft.append(mapXL[y, x])
                        rowRight.append(mapYR[y, x])
                        rowRight.append(mapXR[y, x])
            if (mapXL.shape[1] % meshCellSize) % 2 != 0:
                rowLeft.append(0)
                rowLeft.append(0)
                rowRight.append(0)
                rowRight.append(0)

            meshLeft.append(rowLeft)
            meshRight.append(rowRight)

    meshLeft = np.array(meshLeft)
    meshRight = np.array(meshRight)

    return meshLeft, meshRight


def saveMeshFiles(meshLeft, meshRight, outputPath):
    print("Saving mesh to:", outputPath)
    meshLeft.tofile(outputPath + "/left_mesh.calib")
    meshRight.tofile(outputPath + "/right_mesh.calib")


def getDisparityFrame(frame):
    maxDisp = stereo.initialConfig.getMaxDisparity()
    disp = (frame * (255.0 / maxDisp)).astype(np.uint8)
    disp = cv2.applyColorMap(disp, colormap)

    return disp


print("Creating Stereo Depth pipeline")
pipeline = dai.Pipeline()

camLeft = pipeline.create(dai.node.ColorCamera)
camRight = pipeline.create(dai.node.ColorCamera)
stereo = pipeline.create(dai.node.StereoDepth)
xoutLeft = pipeline.create(dai.node.XLinkOut)
xoutRight = pipeline.create(dai.node.XLinkOut)
xoutDisparity = pipeline.create(dai.node.XLinkOut)
xoutDepth = pipeline.create(dai.node.XLinkOut)
xoutRectifLeft = pipeline.create(dai.node.XLinkOut)
xoutRectifRight = pipeline.create(dai.node.XLinkOut)

camLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
camRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
res = (
    dai.MonoCameraProperties.SensorResolution.THE_800_P
    if resolution[1] == 800
    else dai.MonoCameraProperties.SensorResolution.THE_720_P
    if resolution[1] == 720
    else dai.MonoCameraProperties.SensorResolution.THE_400_P
)
for monoCam in (camLeft, camRight):  # Common config
    monoCam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)
    monoCam.setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)
    monoCam.setIspScale(1, 3)

# monoRight.setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)
    # monoCam.setFps(20.0)

stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.initialConfig.setMedianFilter(median)  # KERNEL_7x7 default
stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout
stereo.setLeftRightCheck(lrcheck)
stereo.setExtendedDisparity(extended)
stereo.setSubpixel(subpixel)

xoutLeft.setStreamName("left")
xoutRight.setStreamName("right")
xoutDisparity.setStreamName("disparity")
xoutDepth.setStreamName("depth")
xoutRectifLeft.setStreamName("rectifiedLeft")
xoutRectifRight.setStreamName("rectifiedRight")

camLeft.isp.link(stereo.left)
camRight.isp.link(stereo.right)
# stereo.syncedLeft.link(xoutLeft.input)
# stereo.syncedRight.link(xoutRight.input)

camLeft.isp.link(xoutLeft.input)
camRight.isp.link(xoutRight.input)
stereo.disparity.link(xoutDisparity.input)
if depth:
    stereo.depth.link(xoutDepth.input)
if outRectified:
    stereo.rectifiedLeft.link(xoutRectifLeft.input)
    stereo.rectifiedRight.link(xoutRectifRight.input)

streams = ["left", "right"]
if outRectified:
    streams.extend(["rectifiedLeft", "rectifiedRight"])
streams.append("disparity")
if depth:
    streams.append("depth")

calibData = dai.Device().readCalibration()
leftMesh, rightMesh = getMesh(calibData)
if generateMesh:
    meshLeft = list(leftMesh.tobytes())
    meshRight = list(rightMesh.tobytes())
    stereo.loadMeshData(meshLeft, meshRight)

if meshDirectory is not None:
    saveMeshFiles(leftMesh, rightMesh, meshDirectory)


print("Creating DepthAI device")
with dai.Device(pipeline) as device:
    # Create a receive queue for each stream
    intrinsics = calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, resolution[0], resolution[1])

    pcl_converter = PointCloudVisualizer(intrinsics, resolution[0], resolution[1])
    qList = [device.getOutputQueue(stream, 8, blocking=False) for stream in streams]
    depthFrame = None
    rectifiedRight = None
    count = 0
    while True:
        key = cv2.waitKey(1)
        if key == ord("q"):
            break
        if key == ord('c'):
            count += 1

        for q in qList:
            name = q.getName()
            frame = q.get().getCvFrame()

            if name == "depth":
                frame = frame.astype(np.uint16)
                depthFrame = frame.copy()
            elif name == "disparity":
                frame = getDisparityFrame(frame)
            elif name == "rectifiedRight":
                rectifiedRight = frame.copy()
            # print(points)
            if points is not None and (name in ["disparity"]) and depthFrame is not None:
                text = "{:.3f}mm".format(depthFrame[points[1]][points[0]]) 
                cv2.circle(frame, points, 3, (255, 255, 255), -1)
                cv2.circle(frame, points, 1, (0, 0, 0), -1)
                cv2.putText(frame, text, (points[0] + 5, points[1] + 5), cv2.FONT_HERSHEY_DUPLEX, 0.6, (255,255,255), 3, cv2.LINE_AA)
                cv2.putText(frame, text, (points[0] + 5, points[1] + 5), cv2.FONT_HERSHEY_DUPLEX, 0.6, (0,0,0), 1, cv2.LINE_AA)

            if key == ord('c') and name not in ['disparity', 'depth']:
                image_dir =  'wide_data/' + name + '/'
                Path(image_dir).mkdir(parents=True, exist_ok=True)
                cv2.imwrite(image_dir + name + str(count) + '.png', frame)
    
            cv2.imshow(name, frame)
        
        if depthFrame is not None and rectifiedRight is not None:
            pcl_converter.rgbd_to_projection(depthFrame, rectifiedRight, False)
            pcl_converter.visualize_pcd()

            