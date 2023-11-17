import depthai as dai
import time

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
sync.setSyncThresholdMs(100)

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("xout")

sync.out.link(xout.input)

script1.outputs["out"].link(sync.inputs["s1"])
script2.outputs["out"].link(sync.inputs["s2"])

# script1.outputs["out"].link(xout.input)

with dai.Device(pipeline) as device:
    print("Start")
    q = device.getOutputQueue("xout", maxSize=10, blocking=True)
    while True:
        grp = q.get()
        for name, msg in grp:
            print(f"Received {name} with timestamp {msg.getTimestamp()}")
        print(f"Time interval between messages: {grp.getIntervalNs() / 1e6}ms")
        print("----------")
        time.sleep(0.2)
