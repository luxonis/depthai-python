#!/usr/bin/env python3
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
    import http.client
    import time

    node.warn('Sending http GET request...')
    h1 = http.client.HTTPConnection('api.ipify.org', 80)
    h1.request("GET", "/")
    r1 = h1.getresponse()
    node.warn(f'{r1.status} {r1.reason}')
    data1 = r1.read()
    node.warn(f'Public IP: {data1}')

    node.io['end'].send(Buffer(32))
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('end')
script.outputs['end'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.getOutputQueue("end").get() # Wait for the "end" msg