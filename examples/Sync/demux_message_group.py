import depthai as dai
import time
from datetime import timedelta

pipeline = dai.Pipeline()

script1 = pipeline.create(dai.node.Script)
script1.setScript("""
from time import sleep

while True:
    sleep(1)
    b = Buffer(512)
    b.setData(bytes(4 * [i for i in range(0, 128)]))
    b.setTimestamp(Clock.now())
    node.io['out'].send(b)
""")

script2 = pipeline.create(dai.node.Script)
script2.setScript("""
from time import sleep

while True:
    sleep(0.3)
    b = Buffer(512)
    b.setData(bytes(4 * [i for i in range(128, 256)]))
    b.setTimestamp(Clock.now())
    node.io['out'].send(b)
""")

sync = pipeline.create(dai.node.Sync)
sync.setSyncThreshold(timedelta(milliseconds=100))

demux = pipeline.create(dai.node.MessageDemux)

xout1 = pipeline.create(dai.node.XLinkOut)
xout1.setStreamName("xout1")
xout2 = pipeline.create(dai.node.XLinkOut)
xout2.setStreamName("xout2")

script1.outputs["out"].link(sync.inputs["s1"])
script2.outputs["out"].link(sync.inputs["s2"])
sync.out.link(demux.input)
demux.outputs["s1"].link(xout1.input)
demux.outputs["s2"].link(xout2.input)

with dai.Device(pipeline) as device:
    print("Start")
    q1 = device.getOutputQueue("xout1", maxSize=10, blocking=True)
    q2 = device.getOutputQueue("xout2", maxSize=10, blocking=True)
    while True:
        bufS1 = q1.get()
        bufS2 = q2.get()
        print(f"Buffer 1 timestamp: {bufS1.getTimestamp()}")
        print(f"Buffer 2 timestamp: {bufS2.getTimestamp()}")
        print("----------")
        time.sleep(0.2)
