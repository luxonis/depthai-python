#!/usr/bin/env python3
import depthai as dai
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setScript("""
    buf = NNData(150)
    buf.setLayer("fp16", [1.0, 1.2, 3.9, 5.5])
    buf.setLayer("uint8", [6, 9, 4, 2, 0])
    node.warn("Names of layers: " + str(buf.getAllLayerNames()))
    node.io['host'].send(buf)
""")

# XLinkOut
xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('host')
script.outputs['host'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.setLogLevel(dai.LogLevel.WARN)
    device.setLogOutputLevel(dai.LogLevel.WARN)

    nndata = device.getOutputQueue("host").get()
    time.sleep(0.5)

    print(f"NNData size: {len(nndata.getData())}")
    print("FP16 values:", nndata.getLayerFp16("fp16"))
    print("UINT8 values:",nndata.getLayerUInt8("uint8"))