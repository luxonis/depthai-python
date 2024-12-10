#!/usr/bin/env python3
import depthai as dai
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setScript("""
    from datetime import timedelta
    while True:
        node.warn("Waiting for message")
        message = node.inputs['test'].get(timedelta(milliseconds=500))
        # message = node.inputs['test'].get()
        if message is None:
            node.warn("Timeout")
            continue
        node.warn("Received message")
        buf = NNData(150)
        node.outputs['host'].send(buf)
""")

# XLinkOut
xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('host')
script.outputs['host'].link(xout.input)

# XLinkIn
xin = pipeline.create(dai.node.XLinkIn)
xin.setStreamName('test')
xin.out.link(script.inputs['test'])

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.setLogLevel(dai.LogLevel.WARN)
    device.setLogOutputLevel(dai.LogLevel.WARN)
    inp = device.getInputQueue("test")
    inp.send(dai.NNData())
    nndata = device.getOutputQueue("host").get()
    print("Host received NNData")
    time.sleep(30)