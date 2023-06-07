import cv2
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
    
    cal = Device.readCalibration2()
    left_camera_id = cal.getStereoLeftCameraId()
    right_camera_id = cal.getStereoRightCameraId()

    extrinsics = cal.getCameraExtrinsics(left_camera_id, right_camera_id)
    intrinsics_left = cal.getCameraIntrinsics(left_camera_id)

    print(extrinsics)
    print(intrinsics_left)

""")

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    while True:
        pass