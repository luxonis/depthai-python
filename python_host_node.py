import depthai as dai
import time

class Printer(dai.HostNode):
    def run(self):
        print("hello world")

p = dai.Pipeline()
p.create(Printer)
p.start()
p.wait()
