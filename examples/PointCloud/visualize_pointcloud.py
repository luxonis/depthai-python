import depthai as dai
import open3d as o3d
from time import sleep

pipeline = dai.Pipeline()
monoLeft = pipeline.create(dai.node.MonoCamera);
monoRight = pipeline.create(dai.node.MonoCamera);
depth = pipeline.create(dai.node.StereoDepth);
pointcloud = pipeline.create(dai.node.PointCloud);
xout = pipeline.create(dai.node.XLinkOut);
xoutDepth = pipeline.create(dai.node.XLinkOut);

monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)

depth.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
depth.initialConfig.setMedianFilter(dai.MedianFilter.KERNEL_7x7)
depth.setLeftRightCheck(True)
depth.setExtendedDisparity(False)
depth.setSubpixel(True)

xout.setStreamName("out")
xoutDepth.setStreamName("depth")

monoLeft.out.link(depth.left)
monoRight.out.link(depth.right)
depth.depth.link(pointcloud.inputDepth)
pointcloud.passthroughDepth.link(xoutDepth.input)
pointcloud.outputPointCloud.link(xout.input)

with dai.Device(pipeline) as device:
    isRunning = True
    def key_callback(vis, action, mods):
        global isRunning
        if action == 0:
            isRunning = False

    q = device.getOutputQueue(name="out", maxSize=4, blocking=False)
    qDepth = device.getOutputQueue(name="depth", maxSize=4, blocking=False)

    pc = o3d.geometry.PointCloud()
    vis = o3d.visualization.VisualizerWithKeyCallback()
    vis.create_window()
    vis.register_key_action_callback(81, key_callback)
    pcd = o3d.geometry.PointCloud()

    first = True
    while isRunning:
        inDepth = qDepth.get()
        inPointCloud = q.get()
        if inPointCloud:
            pcd.points = o3d.utility.Vector3dVector(inPointCloud.getPoints())
            if first:
                vis.add_geometry(pcd)
                first = False
            else:
                vis.update_geometry(pcd)
        vis.poll_events()
        vis.update_renderer()
        sleep(0.01)
    vis.destroy_window()
