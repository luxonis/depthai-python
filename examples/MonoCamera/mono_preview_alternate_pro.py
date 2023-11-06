#!/usr/bin/env python3

import cv2
import depthai as dai

if 1:  # PoE config
    fps = 30
    res = dai.MonoCameraProperties.SensorResolution.THE_400_P
    poolSize = 24  # default 3, increased to prevent desync
else:  # USB
    fps = 30
    res = dai.MonoCameraProperties.SensorResolution.THE_720_P
    poolSize = 8  # default 3, increased to prevent desync

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
monoL = pipeline.create(dai.node.MonoCamera)
monoR = pipeline.create(dai.node.MonoCamera)

monoL.setCamera("left")
monoL.setResolution(res)
monoL.setFps(fps)
monoL.setNumFramesPool(poolSize)
monoR.setCamera("right")
monoR.setResolution(res)
monoR.setFps(fps)
monoR.setNumFramesPool(poolSize)

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
    dotBright = 0.8
    floodBright = 0.1
    LOGGING = False  # Set `True` for latency/timings debugging

    node.warn(f'IR drivers detected: {str(Device.getIrDrivers())}')

    flagDot = False
    while True:
        # Wait first for a frame event, received at MIPI start-of-frame
        event = node.io['event'].get()
        if LOGGING: tEvent = Clock.now()

        # Immediately reconfigure the IR driver.
        # Note the logic is inverted, as it applies for next frame
        Device.setIrLaserDotProjectorBrightness(0 if flagDot else dotBright)
        Device.setIrFloodLightBrightness(floodBright if flagDot else 0)
        if LOGGING: tIrSet = Clock.now()

        # Wait for the actual frames (after MIPI capture and ISP proc is done)
        frameL = node.io['frameL'].get()
        if LOGGING: tLeft = Clock.now()
        frameR = node.io['frameR'].get()
        if LOGGING: tRight = Clock.now()

        if LOGGING:
            latIR      = (tIrSet - tEvent               ).total_seconds() * 1000
            latEv      = (tEvent - event.getTimestamp() ).total_seconds() * 1000
            latProcL   = (tLeft  - event.getTimestamp() ).total_seconds() * 1000
            diffRecvRL = (tRight - tLeft                ).total_seconds() * 1000
            node.warn(f'T[ms] latEv:{latEv:5.3f} latIR:{latIR:5.3f} latProcL:{latProcL:6.3f} '
                    + f' diffRecvRL:{diffRecvRL:5.3f}')

        # Sync checks
        diffSeq = frameL.getSequenceNum() - event.getSequenceNum()
        diffTsEv = (frameL.getTimestamp() - event.getTimestamp()).total_seconds() * 1000
        diffTsRL = (frameR.getTimestamp() - frameL.getTimestamp()).total_seconds() * 1000
        if diffSeq or diffTsEv or (abs(diffTsRL) > 0.8):
            node.error(f'frame/event desync! Fr-Ev: {diffSeq} frames,'
                    + f' {diffTsEv:.3f} ms; R-L: {diffTsRL:.3f} ms')

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
