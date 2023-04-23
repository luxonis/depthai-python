#!/usr/bin/env python3
'''
NOTE: This should only be run on OAK-FFC-4P, as other OAK cameras might have different GPIO configuration!
'''
import depthai as dai
import time

# Start defining a pipeline
pipeline = dai.Pipeline()

script = pipeline.create(dai.node.Script)
script.setScript("""
    import serial
    import time

    ser = serial.Serial("/dev/ttyS0", baudrate=115200)
    i = 0
    while True:
        i += 1
        time.sleep(0.1)
        serString = f'TEST_{i}'
        ser.write(serString.encode())
""")
# Define script for output
script.setProcessor(dai.ProcessorType.LEON_CSS)


config = dai.Device.Config()
# Get argument first
GPIO = dai.BoardConfig.GPIO
config.board.gpio[15] = GPIO(GPIO.OUTPUT, GPIO.ALT_MODE_2)
config.board.gpio[16] = GPIO(GPIO.INPUT, GPIO.ALT_MODE_2)
config.board.uart[0] = dai.BoardConfig.UART()


with dai.Device(config) as device:
    device.startPipeline(pipeline)
    print("Pipeline started")
    while True:
        time.sleep(1)
