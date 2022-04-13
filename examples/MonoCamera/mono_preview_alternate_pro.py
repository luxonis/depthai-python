#!/usr/bin/env python3

import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoL = pipeline.create(dai.node.MonoCamera)
monoR = pipeline.create(dai.node.MonoCamera)

monoL.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoL.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)
monoR.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoR.setResolution(dai.MonoCameraProperties.SensorResolution.THE_720_P)

xoutDotL = pipeline.create(dai.node.XLinkOut)
xoutDotR = pipeline.create(dai.node.XLinkOut)
xoutFloodL = pipeline.create(dai.node.XLinkOut)
xoutFloodR = pipeline.create(dai.node.XLinkOut)

xoutDotL.setStreamName('dot-left')
xoutDotR.setStreamName('dot-right')
xoutFloodL.setStreamName('flood-left')
xoutFloodR.setStreamName('flood-right')
streams = ['dot-left', 'dot-right', 'flood-left', 'flood-right']

# Script node for frame routing and IR dot/flood alternate
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
    dotBright = 500
    floodBright = 200

    node.warn(f'IR drivers detected: {str(Device.getIrDrivers())}')

    flagDot = False
    while True:
        # Wait first for a frame event, received at MIPI start-of-frame
        event = node.io['event'].get()

        # Immediately reconfigure the IR driver.
        # Note the logic is inverted, as it applies for next frame
        Device.setIrLaserDotProjectorBrightness(0 if flagDot else dotBright)
        Device.setIrFloodLightBrightness(floodBright if flagDot else 0)

        # Wait for the actual frames (after MIPI capture and ISP proc is done)
        frameL = node.io['frameL'].get()
        frameR = node.io['frameR'].get()

        # Route the frames to their respective outputs
        node.io['dotL' if flagDot else 'floodL'].send(frameL)
        node.io['dotR' if flagDot else 'floodR'].send(frameR)

        flagDot = not flagDot
""")

# Linking
monoL.frameEvent.link(script.inputs['event'])
monoL.out.link(script.inputs['frameL'])
monoR.out.link(script.inputs['frameR'])

script.outputs['dotL'].link(xoutDotL.input)
script.outputs['dotR'].link(xoutDotR.input)
script.outputs['floodL'].link(xoutFloodL.input)
script.outputs['floodR'].link(xoutFloodR.input)

# Connect to device and start pipeline
with dai.Device(pipeline) as device:
    queues = [device.getOutputQueue(name=s, maxSize=4, blocking=False) for s in streams]

    while True:
        for q in queues:
            pkt = q.tryGet()
            if pkt is not None:
                name = q.getName()
                frame = pkt.getCvFrame()
                cv2.imshow(name, frame)

        if cv2.waitKey(5) == ord('q'):
            break
