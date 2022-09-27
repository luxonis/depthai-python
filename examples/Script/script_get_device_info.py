#!/usr/bin/env python3
import depthai as dai
import json

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setScript("""
    import json
    data = json.dumps({
        'deviceId': __device_id__,
        'fwVersion': __version__
    }).encode('utf-8')

    b = Buffer(len(data))
    b.setData(data)
    node.io['info'].send(b)
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('info')
script.outputs['info'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    msg = device.getOutputQueue("info").get() # Wait for the "end" msg
    data = json.loads(msg.getData().tobytes().decode('utf-8'))
    print(json.dumps(data, indent=4))
