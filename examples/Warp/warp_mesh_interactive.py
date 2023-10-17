#!/usr/bin/env python3
import cv2
import depthai as dai
import numpy as np
import argparse
import re
import sys
from random import randint

parser = argparse.ArgumentParser()
parser.add_argument("-m", "--mesh_dims", type=str, default="4x4", help="mesh dimensions widthxheight (default=%(default)s)")
parser.add_argument("-r", "--resolution", type=str, default="512x512", help="preview resolution (default=%(default)s)")
parser.add_argument("-rnd", "--random", action="store_true", help="Generate random initial mesh")
args = parser.parse_args()

# mesh dimensions
match = re.search(r'.*?(\d+)x(\d+).*', args.mesh_dims)
if not match:
    raise Exception(f"Mesh dimensions format incorrect '{args.resolution}'!")
mesh_w = int(match.group(1))
mesh_h = int(match.group(2))

# Preview resolution
match = re.search(r'.*?(\d+)x(\d+).*', args.resolution)
if not match:
    raise Exception(f"Resolution format incorrect '{args.resolution}'!")
preview_w = int(match.group(1))
preview_h = int(match.group(2))
if preview_w % 16 != 0:
    raise Exception(f"Preview width must be a multiple of 16!")

# Create an initial mesh (optionally random) of dimension mesh_w x mesh_h
first_point_x = int(preview_w / 10)
between_points_x = int(4 * preview_w / (5 * (mesh_w - 1)))
first_point_y = int(preview_h / 10)
between_points_y = int(4 * preview_h / (5 * (mesh_h - 1)))
if args.random:
    max_rnd_x = int(between_points_x / 4)
    max_rnd_y = int(between_points_y / 4)
mesh = []
for i in range(mesh_h):
    for j in range(mesh_w):
        x = first_point_x + j * between_points_x
        y = first_point_y + i * between_points_y
        if args.random:
            rnd_x = randint(-max_rnd_x, max_rnd_x)
            if x + rnd_x > 0 and x + rnd_x < preview_w:
                x += rnd_x
            rnd_y = randint(-max_rnd_y, max_rnd_y)
            if y + rnd_y > 0 and y + rnd_y < preview_h:
                y += rnd_y
        mesh.append((x, y))

def create_pipeline(mesh):
    print(mesh)
    # Create pipeline
    pipeline = dai.Pipeline()

    camRgb = pipeline.create(dai.node.ColorCamera)
    camRgb.setPreviewSize(preview_w, preview_h)
    camRgb.setInterleaved(False)
    width = camRgb.getPreviewWidth()
    height = camRgb.getPreviewHeight()

    # Output source
    xout_source = pipeline.create(dai.node.XLinkOut)
    xout_source.setStreamName('source')
    camRgb.preview.link(xout_source.input)
    # Warp source frame
    warp = pipeline.create(dai.node.Warp)
    warp.setWarpMesh(mesh, mesh_w, mesh_h)
    warp.setOutputSize(width, height)
    warp.setMaxOutputFrameSize(width * height * 3)
    camRgb.preview.link(warp.inputImage)

    warp.setHwIds([1])
    warp.setInterpolation(dai.node.Warp.Properties.Interpolation.BYPASS)
    # Output warped
    xout_warped = pipeline.create(dai.node.XLinkOut)
    xout_warped.setStreamName('warped')
    warp.out.link(xout_warped.input)
    return pipeline

point_selected = None

def mouse_callback(event, x, y, flags, param):
    global mesh, point_selected, mesh_changed
    if event == cv2.EVENT_LBUTTONDOWN:
        if point_selected is None:
            # Which point is selected ?
            min_dist = 100

            for i in range(len(mesh)):
                dist = np.linalg.norm((x - mesh[i][0], y - mesh[i][1]))
                if dist < 20 and dist < min_dist:
                    min_dist = dist
                    point_selected = i
            if point_selected is not None:
                mesh[point_selected] = (x, y)
                mesh_changed = True

    elif event == cv2.EVENT_LBUTTONUP:
        point_selected = None
    elif event == cv2.EVENT_MOUSEMOVE:
        if point_selected is not None:
            mesh[point_selected] = (x, y)
            mesh_changed = True


cv2.namedWindow("Source")
cv2.setMouseCallback("Source", mouse_callback)

running = True

print("Use your mouse to modify the mesh by clicking/moving points of the mesh in the Source window")
print("Then press 'r' key to restart the device/pipeline")
while running:
    pipeline = create_pipeline(mesh)
    # Connect to device and start pipeline
    with dai.Device(pipeline) as device:
        print("Starting device")
        # Output queue will be used to get the rgb frames from the output defined above
        q_source = device.getOutputQueue(name="source", maxSize=4, blocking=False)
        q_warped = device.getOutputQueue(name="warped", maxSize=4, blocking=False)

        restart_device = False
        mesh_changed = False
        while not restart_device:
            in0 = q_source.get()
            if in0 is not None:
                source = in0.getCvFrame()
                color = (0, 0,255) if mesh_changed else (0,255,0)
                for i in range(len(mesh)):
                    cv2.circle(source, (mesh[i][0], mesh[i][1]), 4, color, -1)
                    if i % mesh_w != mesh_w -1:
                        cv2.line(source, (mesh[i][0], mesh[i][1]), (mesh[i+1][0], mesh[i+1][1]), color, 2)
                    if i + mesh_w < len(mesh):
                        cv2.line(source, (mesh[i][0], mesh[i][1]), (mesh[i+mesh_w][0], mesh[i+mesh_w][1]), color, 2)
                cv2.imshow("Source", source)

            in1 = q_warped.get()
            if in1 is not None:
                cv2.imshow("Warped", in1.getCvFrame())

            key = cv2.waitKey(1)
            if key == ord('r'): # Restart the device if mesh has changed
                if mesh_changed:
                    print("Restart requested...")
                    mesh_changed = False
                    restart_device = True
            elif key == 27 or key == ord('q'): # Exit
                running = False
                break