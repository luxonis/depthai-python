#!/usr/bin/env python3
import depthai as dai

# Start defining a pipeline
pipeline = dai.Pipeline()

# Script node
script = pipeline.create(dai.node.Script)
script.setProcessor(dai.ProcessorType.LEON_CSS)
script.setScript("""
import socket
import fcntl
import struct

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        -1071617759,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15].encode())
    )[20:24])

ip = get_ip_address('re0')  # '192.168.0.110'
node.warn(f'IP of the device: {ip}')
node.io['end'].send(Buffer(32))
""")

xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName('end')
script.outputs['end'].link(xout.input)

# Connect to device with pipeline
with dai.Device(pipeline) as device:
    device.getOutputQueue("end").get() # Wait for the "end" msg