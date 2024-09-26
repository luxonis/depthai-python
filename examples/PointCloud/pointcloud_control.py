#!/usr/bin/env python3

import depthai as dai
import numpy as np
import cv2
import sys

try:
    import open3d as o3d
except ImportError:
    sys.exit(f"Critical dependency missing: Open3D. Please install it using the command: '{sys.executable} -m pip install open3d' and then rerun the script.")


# Continue with the DepthAI and Open3D setup as before
pipeline = dai.Pipeline()
camRgb = pipeline.create(dai.node.ColorCamera)
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
depth = pipeline.create(dai.node.StereoDepth)
pointcloud: dai.node.PointCloud = pipeline.create(dai.node.PointCloud)
sync = pipeline.create(dai.node.Sync)
xOut = pipeline.create(dai.node.XLinkOut)
xOut.input.setBlocking(False)


camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setIspScale(1,3)

monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("left")

monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setCamera("right")


depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
depth.setLeftRightCheck(True)
depth.setExtendedDisparity(False)
depth.setSubpixel(True)
depth.setDepthAlign(dai.CameraBoardSocket.CAM_A)
config = depth.initialConfig.get()
config.postProcessing.thresholdFilter.minRange = 200
config.postProcessing.thresholdFilter.maxRange = 1000
depth.initialConfig.set(config)

monoLeft.out.link(depth.left)
monoRight.out.link(depth.right)
depth.depth.link(pointcloud.inputDepth)
camRgb.isp.link(sync.inputs["rgb"])
pointcloud.outputPointCloud.link(sync.inputs["pcl"])
pointcloud.initialConfig.setSparse(False)
sync.out.link(xOut.input)
xOut.setStreamName("out")

inConfig = pipeline.create(dai.node.XLinkIn)
inConfig.setStreamName("config")
inConfig.out.link(pointcloud.inputConfig)

with dai.Device(pipeline) as device:

    isRunning = True
    def key_callback(vis, action, mods):
        global isRunning
        if action == 0:
            isRunning = False

    q = device.getOutputQueue(name="out", maxSize=4, blocking=False)
    pclConfIn = device.getInputQueue(name="config", maxSize=4, blocking=False)

    vis = o3d.visualization.Visualizer()
    vis.create_window()
    pcd = o3d.geometry.PointCloud()
    coordinateFrame = o3d.geometry.TriangleMesh.create_coordinate_frame(size=1000, origin=[0,0,0])
    vis.add_geometry(coordinateFrame)
    vis.add_geometry(pcd)

    first = True
    rot = 0
    while device.isPipelineRunning():
        
        inMessage = q.get()
        inColor = inMessage["rgb"]
        inPointCloud = inMessage["pcl"]
        cvColorFrame = inColor.getCvFrame()
        
        cvRGBFrame = cv2.cvtColor(cvColorFrame, cv2.COLOR_BGR2RGB)

        if inPointCloud:
            points = inPointCloud.getPoints().astype(np.float64)
            pcd.points = o3d.utility.Vector3dVector(points)
            colors = (cvRGBFrame.reshape(-1, 3) / 255.0).astype(np.float64)
            pcd.colors = o3d.utility.Vector3dVector(colors)
            
            vis.update_geometry(pcd)
        vis.poll_events()
        vis.update_renderer()

        # Create a transformation matrix that rotates around all 3 axes at the same time
        rotate_z_matrix = np.array([[np.cos(rot), -np.sin(rot), 0, 0],
                                          [np.sin(rot), np.cos(rot), 0, 0],
                                          [0, 0, 1, 0],
                                          [0, 0, 0, 1]])
        
        translate_y_matrix = np.array([[1, 0, 0, 0],
                                  [0, 1, 0, 500],
                                  [0, 0, 1, 0],
                                  [0, 0, 0, 1]])
    
        
        # Combine the 3 transformation matrices into one
        transformation_matrix = rotate_z_matrix @ translate_y_matrix
        
        rot += 0.05

        pclConfig = dai.PointCloudConfig()
        pclConfig.setTransformationMatrix(transformation_matrix)
        pclConfig.setSparse(False)
        pclConfIn.send(pclConfig)

            
    vis.destroy_window()
    cv2.destroyAllWindows()
