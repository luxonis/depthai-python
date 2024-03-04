#!/usr/bin/env python3
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
    import time
    
    cal = Device.readCalibration2()
    left_camera_id = cal.getStereoLeftCameraId()
    right_camera_id = cal.getStereoRightCameraId()

    extrinsics = cal.getCameraExtrinsics(left_camera_id, right_camera_id)
    intrinsics_left = cal.getCameraIntrinsics(left_camera_id)

    node.info(extrinsics.__str__())
    node.info(intrinsics_left.__str__())

    time.sleep(1)
    node.io['end'].send(Buffer(32))
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('end')
script.outputs['end'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.getOutputQueue('end').get()