#!/usr/bin/env python3

import cv2
import numpy as np
import depthai as dai
import argparse

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
mesh_directory = args.mesh_dir  # Output dir for mesh files
generate_mesh = args.load_mesh  # Load mesh files

out_rectified = args.out_rectified  # Output and display rectified streams
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

# Sanitize some incompatible options
if lrcheck or extended or subpixel:
    print(
        "Median filtering will be set to OFF, since",
        "lrcheck and" * lrcheck,
        "extended and" * extended,
        "subpixel" * subpixel,
        "is enabled.",
    )
    median = dai.StereoDepthProperties.MedianFilter.MEDIAN_OFF

print("StereoDepth config options:")
print("    Resolution:  ", resolution)
print("    Left-Right check:  ", lrcheck)
print("    Extended disparity:", extended)
print("    Subpixel:          ", subpixel)
print("    Median filtering:  ", median)
print("    Generating mesh files:  ", generate_mesh)
print("    Outputting mesh files to:  ", mesh_directory)


def create_stereo_pipeline(calibData, mesh_directory):
    print("Creating Stereo Depth pipeline")
    pipeline = dai.Pipeline()

    cam_left = pipeline.createMonoCamera()
    cam_right = pipeline.createMonoCamera()
    stereo = pipeline.createStereoDepth()
    xout_left = pipeline.createXLinkOut()
    xout_right = pipeline.createXLinkOut()
    xout_disparity = pipeline.createXLinkOut()
    xout_depth = pipeline.createXLinkOut()
    xout_rectif_left = pipeline.createXLinkOut()
    xout_rectif_right = pipeline.createXLinkOut()

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

    stereo.initialConfig.setConfidenceThreshold(200)
    stereo.initialConfig.setMedianFilter(median)  # KERNEL_7x7 default
    stereo.setRectifyEdgeFillColor(0)  # Black, to better see the cutout
    stereo.setLeftRightCheck(lrcheck)
    stereo.setExtendedDisparity(extended)
    stereo.setSubpixel(subpixel)

    xout_left.setStreamName("left")
    xout_right.setStreamName("right")
    xout_disparity.setStreamName("disparity")
    xout_depth.setStreamName("depth")
    xout_rectif_left.setStreamName("rectified_left")
    xout_rectif_right.setStreamName("rectified_right")

    cam_left.out.link(stereo.left)
    cam_right.out.link(stereo.right)
    stereo.syncedLeft.link(xout_left.input)
    stereo.syncedRight.link(xout_right.input)
    stereo.disparity.link(xout_disparity.input)
    if depth:
        stereo.depth.link(xout_depth.input)
    if out_rectified:
        stereo.rectifiedLeft.link(xout_rectif_left.input)
        stereo.rectifiedRight.link(xout_rectif_right.input)

    streams = ["disparity"]
    if depth:
        streams.append("depth")
    if out_rectified:
        streams.extend(["rectified_left", "rectified_right"])
    streams.extend(["left", "right"])

    left_mesh, right_mesh = get_lr_mesh(calibData)
    if generate_mesh:
        mesh_left = list(left_mesh.tobytes())
        mesh_right = list(right_mesh.tobytes())
        stereo.loadMeshData(mesh_left, mesh_right)

    if mesh_directory is not None:
        save_mesh_files(left_mesh, right_mesh, mesh_directory)

    return pipeline, streams


def get_lr_mesh(calibData):
    M1 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.LEFT, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.LEFT))
    R1 = np.array(calibData.getStereoLeftRectificationRotation())
    M2 = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, resolution[0], resolution[1]))
    d2 = np.array(calibData.getDistortionCoefficients(dai.CameraBoardSocket.RIGHT))
    R2 = np.array(calibData.getStereoRightRectificationRotation())
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

    return mesh_left, mesh_right


def save_mesh_files(mesh_left, mesh_right, output_path):
    print("Saving mesh to:", output_path)
    mesh_left.tofile(output_path + "/left_mesh.calib")
    mesh_right.tofile(output_path + "/right_mesh.calib")


def get_disparity_frame(frame):
    max_disp = 95
    if extended:
        max_disp *= 2
    if subpixel:
        max_disp *= 32

    disp = (frame * (255.0 / max_disp)).astype(np.uint8)
    disp = cv2.applyColorMap(disp, cv2.COLORMAP_JET)

    return disp


def test_pipeline():
    calibData = dai.Device().readCalibration()
    pipeline, streams = create_stereo_pipeline(calibData, mesh_directory)

    print("Creating DepthAI device")
    with dai.Device(pipeline) as device:
        # Create a receive queue for each stream
        q_list = [device.getOutputQueue(stream, 8, blocking=False) for stream in streams]

        while True:
            for q in q_list:
                name = q.getName()
                frame = q.get().getCvFrame()
                if name == "depth":
                    frame = frame.astype(np.uint16 if subpixel else np.uint8)
                elif name == "disparity":
                    frame = get_disparity_frame(frame)
                
                cv2.imshow(name, frame)
            if cv2.waitKey(1) == ord("q"):
                break


test_pipeline()
