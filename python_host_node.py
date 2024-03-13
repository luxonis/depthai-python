import depthai as dai
import time

class Printer(dai.HostNode):
    def __init__(self):
        dai.HostNode.__init__(self)
        print("I'm created - a new Printer object")

    def run(self):
        print("hello world")

p = dai.Pipeline()
printer = Printer()
a = p.add(printer)
p.start()
p.wait()
