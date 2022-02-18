#!/usr/bin/env python3
import depthai as dai
import json

pipeline = dai.Pipeline()

xin = pipeline.create(dai.node.XLinkIn)
xin.setStreamName('in')

script = pipeline.create(dai.node.Script)
xin.out.link(script.inputs['in'])
script.setScript("""
    import json

    # Receive bytes from the host
    data = node.io['in'].get().getData()
    jsonStr = str(data, 'utf-8')
    dict = json.loads(jsonStr)

    # Change initial dictionary a bit
    dict['one'] += 1
    dict['foo'] = "baz"

    b = Buffer(30)
    b.setData(json.dumps(dict).encode('utf-8'))
    node.io['out'].send(b)
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('out')
script.outputs['out'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    # This dict will be serialized (JSON), sent to device (Script node),
    # edited a bit and sent back to the host
    dict = {'one':1, 'foo': 'bar'}
    print('dict', dict)
    data = json.dumps(dict).encode('utf-8')
    buffer = dai.Buffer()
    buffer.setData(list(data))
    device.getInputQueue("in").send(buffer)

    # Wait for the script to send the changed dictionary back
    jsonData = device.getOutputQueue("out").get()
    jsonText = str(jsonData.getData(), 'utf-8')
    changedDict = json.loads(jsonText)
    print('changedDict', changedDict)