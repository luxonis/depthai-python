#!/usr/bin/env python3
import depthai as dai
import json

pipeline = dai.Pipeline()

script = pipeline.create(dai.node.Script)
script.setScript("""
    import json
    dict = {}
    with open('/sys/class/thermal/thermal_zone0/temp') as f:
        dict['temp'] = int(f.read()) / 1000.0
    # Change initial dictionary a bit
    dict['other_sensor'] = 3
    dict['another_sensor'] = 4.12

    b = Buffer()
    b.setData(json.dumps(dict).encode('utf-8'))
    node.io['out'].send(b)
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('out')
script.outputs['out'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    # Wait for the script to send the changed dictionary back
    jsonData = device.getOutputQueue("out").get()
    jsonText = str(jsonData.getData(), 'utf-8')
    sensorDict = json.loads(jsonText)
    print('sensorDict', sensorDict)