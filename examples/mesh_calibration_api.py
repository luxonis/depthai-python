#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
import argparse
from pathlib import Path

"""
If one or more of the additional depth modes (lrcheck, extended, subpixel)
are enabled, then:
 - depth output is FP16. TODO enable U16.
 - median filtering is disabled on device. TODO enable.
 - with subpixel, either depth or disparity has valid data.

Otherwise, depth output is U16 (mm) and median is functional.
But like on Gen1, either depth or disparity has valid data. TODO enable both.
"""

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
    help="Contains mesh files, if not specified data from EEPROM will be used",
)
parser.add_argument(
    "-static",
    "--static_frames",
    default=False,
    action="store_true",
    help="Run stereo on static frames passed from host 'dataset' folder",
)
args = parser.parse_args()

resolutionMap = {"800": (1280, 800), "720": (1280, 720), "400": (640, 400)}

if args.resolution not in resolutionMap:
    print("Unsupported resolution, 720P will be used")
    args.resolution = "720"

resolution = resolutionMap[args.resolution]

mesh_directory = args.mesh_dir  # Directory which contains mesh files

# StereoDepth config options. TODO move to command line options
source_camera = not args.static_frames
out_depth = False  # Disparity by default
out_rectified = True  # Output and display rectified streams
lrcheck = True  # Better handling for occlusions
extended = False  # Closer-in minimum depth, disparity range is doubled
subpixel = True  # Better accuracy for longer distance, fractional disparity 32-levels
# Options: MEDIAN_OFF, KERNEL_3x3, KERNEL_5x5, KERNEL_7x7
median = dai.StereoDepthProperties.MedianFilter.KERNEL_7x7

# Sanitize some incompatible options
if lrcheck or extended or subpixel:
    median = dai.StereoDepthProperties.MedianFilter.MEDIAN_OFF  # TODO

print("StereoDepth config options:")
print("    Left-Right check:  ", lrcheck)
print("    Extended disparity:", extended)
print("    Subpixel:          ", subpixel)
print("    Median filtering:  ", median)


def create_stereo_pipeline(calibData, from_camera=True, mesh_directory=None):
    print("Creating Stereo Depth pipeline: ", end="")
    if from_camera:
        print("MONO CAMS -> STEREO -> XLINK OUT")
    else:
        print("XLINK IN -> STEREO -> XLINK OUT")
    pipeline = dai.Pipeline()

    if from_camera:
        cam_left = pipeline.createMonoCamera()
        cam_right = pipeline.createMonoCamera()
    else:
        cam_left = pipeline.createXLinkIn()
        cam_right = pipeline.createXLinkIn()
    stereo = pipeline.createStereoDepth()
    xout_left = pipeline.createXLinkOut()
    xout_right = pipeline.createXLinkOut()
    xout_depth = pipeline.createXLinkOut()
    xout_rectif_left = pipeline.createXLinkOut()
    xout_rectif_right = pipeline.createXLinkOut()

    if from_camera:
        cam_left.setBoardSocket(dai.CameraBoardSocket.LEFT)
        cam_right.setBoardSocket(dai.CameraBoardSocket.RIGHT)
        res = (
            dai.MonoCameraProperties.SensorResolution.THE_800_P
            if resolution[1] == 800
            else dai.MonoCameraProperties.SensorResolution.THE_720_P
            if resolution[1] == 720
            else dai.MonoCameraProperties.SensorResolution.THE_400_P
        )
        for mono_cam in (cam_left, cam_right):  # Common config
            mono_cam.setResolution(res)
            # mono_cam.setFps(20.0)
    else:
        cam_left.setStreamName("in_left")
        cam_right.setStreamName("in_right")

    stereo.setConfidenceThreshold(200)
    stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout
    stereo.setMedianFilter(median)  # KERNEL_7x7 default
    stereo.setLeftRightCheck(lrcheck)
    stereo.setExtendedDisparity(extended)
    stereo.setSubpixel(subpixel)
    if from_camera:
        # Default: EEPROM calib is used, and resolution taken from MonoCamera nodes
        # stereo.loadCalibrationFile(path)
        pass
    else:
        stereo.setEmptyCalibration()  # Set if the input frames are already rectified
        stereo.setInputResolution(1280, 720)

    xout_left.setStreamName("left")
    xout_right.setStreamName("right")
    xout_depth.setStreamName("depth")
    xout_rectif_left.setStreamName("rectified_left")
    xout_rectif_right.setStreamName("rectified_right")

    cam_left.out.link(stereo.left)
    cam_right.out.link(stereo.right)
    stereo.syncedLeft.link(xout_left.input)
    stereo.syncedRight.link(xout_right.input)
    stereo.depth.link(xout_depth.input)
    if out_rectified:
        stereo.rectifiedLeft.link(xout_rectif_left.input)
        stereo.rectifiedRight.link(xout_rectif_right.input)

    streams = ["depth"]
    if out_rectified:
        streams.extend(["rectified_left", "rectified_right"])
    streams.extend(["left", "right"])

    parent_path = Path(__file__).resolve().parents[1]
    mesh_directory = str(parent_path) + "/generated" if mesh_directory is None else mesh_directory
    generate_mesh(calibData, mesh_directory)

    # Load mesh files
    left_mesh = mesh_directory + "/left_mesh.calib"
    right_mesh = mesh_directory + "/right_mesh.calib"
    stereo.loadMeshFiles(left_mesh, right_mesh)

    return pipeline, streams


def generate_mesh(calibData, output_path):
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.LEFT))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    R2 = np.array(calibData.getStereoRightRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.RIGHT))
    map_x_l, map_y_l = cv2.initUndistortRectifyMap(M1, d1, R1, M2, resolution, cv2.CV_32FC1)
    map_x_r, map_y_r = cv2.initUndistortRectifyMap(M2, d2, R2, M2, resolution, cv2.CV_32FC1)

    meshCellSize = 16
    mesh_left = []
    mesh_right = []

    for y in range(map_x_l.shape[0] + 1):
        if y % meshCellSize == 0:
            row_left = []
            row_right = []
            for x in range(map_x_l.shape[1] + 1):
                if x % meshCellSize == 0:
                    if y == map_x_l.shape[0] and x == map_x_l.shape[1]:
                        row_left.append(map_y_l[y - 1, x - 1])
                        row_left.append(map_x_l[y - 1, x - 1])
                        row_right.append(map_y_r[y - 1, x - 1])
                        row_right.append(map_x_r[y - 1, x - 1])
                    elif y == map_x_l.shape[0]:
                        row_left.append(map_y_l[y - 1, x])
                        row_left.append(map_x_l[y - 1, x])
                        row_right.append(map_y_r[y - 1, x])
                        row_right.append(map_x_r[y - 1, x])
                    elif x == map_x_l.shape[1]:
                        row_left.append(map_y_l[y, x - 1])
                        row_left.append(map_x_l[y, x - 1])
                        row_right.append(map_y_r[y, x - 1])
                        row_right.append(map_x_r[y, x - 1])
                    else:
                        row_left.append(map_y_l[y, x])
                        row_left.append(map_x_l[y, x])
                        row_right.append(map_y_r[y, x])
                        row_right.append(map_x_r[y, x])
            if (map_x_l.shape[1] % meshCellSize) % 2 != 0:
                row_left.append(0)
                row_left.append(0)
                row_right.append(0)
                row_right.append(0)

            mesh_left.append(row_left)
            mesh_right.append(row_right)

    mesh_left = np.array(mesh_left)
    mesh_right = np.array(mesh_right)
    print("Saving mesh to:", output_path)
    mesh_left.tofile(output_path + "/left_mesh.calib")
    mesh_right.tofile(output_path + "/right_mesh.calib")


def get_depth_frame(image):
    max_disp = 95
    disp_type = np.uint8
    if extended:
        max_disp *= 2
    if subpixel:
        max_disp *= 32
        disp_type = np.uint16  # 5 bits fractional disparity
    
    data, w, h = image.getData(), image.getWidth(), image.getHeight()
    disp = np.array(data).astype(np.uint8).view(disp_type).reshape((h, w))

    frame = (disp * 255. / max_disp).astype(np.uint8)
    frame = cv2.applyColorMap(frame, cv2.COLORMAP_HOT)

    return frame


def test_pipeline():
    calibData = dai.Device().readCalibration()
    pipeline, streams = create_stereo_pipeline(calibData, source_camera)  # Also creates mesh files

    print("Creating DepthAI device")
    with dai.Device(pipeline) as device:
        # Create a receive queue for each stream
        q_list = []
        for s in streams:
            q = device.getOutputQueue(s, 8, blocking=False)
            q_list.append(q)

        while True:
            for q in q_list:
                name = q.getName()
                image = q.get()
                frame = get_depth_frame(image) if name == "depth" else image.getCvFrame()
                cv2.imshow(name, frame)
            if cv2.waitKey(1) == ord("q"):
                break


test_pipeline()
