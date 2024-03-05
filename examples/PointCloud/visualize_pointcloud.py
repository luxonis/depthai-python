import depthai as dai
import open3d as o3d
from time import sleep
import numpy as np
import cv2
import time

FPS = 30
class FPSCounter:
    def __init__(self):
        self.frameCount = 0
        self.fps = 0
        self.startTime = time.time()

    def tick(self):
        self.frameCount += 1
        if self.frameCount % 10 == 0:
            elapsedTime = time.time() - self.startTime
            self.fps = self.frameCount / elapsedTime
            self.frameCount = 0
            self.startTime = time.time()
        return self.fps

pipeline = dai.Pipeline()
camRgb = pipeline.create(dai.node.ColorCamera)
monoLeft = pipeline.create(dai.node.MonoCamera)
monoRight = pipeline.create(dai.node.MonoCamera)
depth = pipeline.create(dai.node.StereoDepth)
pointcloud = pipeline.create(dai.node.PointCloud)
sync = pipeline.create(dai.node.Sync)
xOut = pipeline.create(dai.node.XLinkOut)
xOut.input.setBlocking(False)


camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setIspScale(1,3)
camRgb.setFps(FPS)

monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setCamera("left")
monoLeft.setFps(FPS)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setCamera("right")
monoRight.setFps(FPS)

depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
depth.setLeftRightCheck(True)
depth.setExtendedDisparity(False)
depth.setSubpixel(True)
depth.setDepthAlign(dai.CameraBoardSocket.CAM_A)

monoLeft.out.link(depth.left)
monoRight.out.link(depth.right)
depth.depth.link(pointcloud.inputDepth)
camRgb.isp.link(sync.inputs["rgb"])
pointcloud.outputPointCloud.link(sync.inputs["pcl"])
sync.out.link(xOut.input)
xOut.setStreamName("out")




with dai.Device(pipeline) as device:
    isRunning = True
    def key_callback(vis, action, mods):
        global isRunning
        if action == 0:
            isRunning = False

    q = device.getOutputQueue(name="out", maxSize=4, blocking=False)
    pc = o3d.geometry.PointCloud()
    vis = o3d.visualization.VisualizerWithKeyCallback()
    vis.create_window()
    vis.register_key_action_callback(81, key_callback)
    pcd = o3d.geometry.PointCloud()
    coordinateFrame = o3d.geometry.TriangleMesh.create_coordinate_frame(size=1000, origin=[0,0,0])
    vis.add_geometry(coordinateFrame)

    first = True
    fpsCounter = FPSCounter()
    while isRunning:
        inMessage = q.get()
        inColor = inMessage["rgb"]
        inPointCloud = inMessage["pcl"]
        cvColorFrame = inColor.getCvFrame()
        # Convert the frame to RGB
        cvRGBFrame = cv2.cvtColor(cvColorFrame, cv2.COLOR_BGR2RGB)
        fps = fpsCounter.tick()
        # Display the FPS on the frame
        cv2.putText(cvColorFrame, f"FPS: {fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imshow("color", cvColorFrame)
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        if inPointCloud:
            t_before = time.time()
            points = inPointCloud.getPoints().astype(np.float64)
            pcd.points = o3d.utility.Vector3dVector(points)
            colors = (cvRGBFrame.reshape(-1, 3) / 255.0).astype(np.float64)
            pcd.colors = o3d.utility.Vector3dVector(colors)
            if first:
                vis.add_geometry(pcd)
                first = False
            else:
                vis.update_geometry(pcd)
        vis.poll_events()
        vis.update_renderer()
    vis.destroy_window()
