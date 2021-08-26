#!/usr/bin/env python3
import depthai as dai
import time

# Whether to use HTTPS instead of HTTP
SSL = False

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript(f"""
    import http.client
    import time
    while True:
        node.warn('Sending http GET request...')
        h1 = http.client.HTTP{'S' if SSL else ''}Connection('httpstat.us', {'443' if SSL else '80'})
        h1.request("GET", "/200")
        r1 = h1.getresponse()
        node.warn(r1.status + ' ' + r1.reason)
        data1 = r1.read()
        node.warn(str(data1))
        time.sleep(1)
""")

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.setLogLevel(dai.LogLevel.WARN)
    device.setLogOutputLevel(dai.LogLevel.WARN)
    while not device.isClosed():
        time.sleep(1)