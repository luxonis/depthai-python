import unittest
from collections import deque, defaultdict
import depthai.compilation
import depthai
import collections

class DeviceMock:
    """Sends everything from inputs to outputs"""
    class OutputQueue:
        def __init__(self):
            self.queue = deque()
        def tryGet(self):
            if len(self.queue) == 0: return None
            return self.queue.pop()
    def __init__(self):
        self.outs = []
    def getOutputQueue(self, _):
        out_queue = DeviceMock.OutputQueue()
        self.outs.append(out_queue)
        return out_queue
    def getInputQueue(self, _):
        class InputQueue:
            def send(_, data):
                for out_queue in self.outs:
                    out_queue.queue.appendleft(data)
        return InputQueue()
    def __enter__(self): pass
    def __exit__(self, *args): pass

def start_devices_mock(pipeline, context):
    host_pipeline = depthai.Pipeline()
    context["running_devices"] = {}
    for node in pipeline:
        if node.device is None:
            host_pipeline.append(node)
        else:
            context["running_devices"][node.device] = DeviceMock()

    return host_pipeline

mock_compilation = [start_devices_mock 
                        if step.__name__ == "start_devices" 
                        else step
                    for step in depthai.compilation.default_compilation]

class Src(depthai.Node):
    def __node_init__(self, context):
        self.i = 0
    def __run__(self) -> int:
        self.i += 1 
        return self.i

class Dst(depthai.Node):
    def __node_init__(self, test_self, context):
        self.i = 0
        self.test_self = test_self
    def __run__(self, input: int) -> None:
        self.i += 1 
        self.test_self.assertEqual(self.i, input)
        if self.i == 100: raise StopIteration()


class TestXLinks(unittest.TestCase):
    def test_simple(self):
        def main_loop(pipeline, context):
            input_queues = [device.getInputQueue(None)
                            for device in context["running_devices"].values()]
            for i in range(1, 10000):
                for node in pipeline:
                    node.dispatch()
                for input_queue in input_queues:
                    input_queue.send(i)

        with self.assertRaises(StopIteration):
            with (p := depthai.Pipeline()):
                with depthai.DeviceRef():
                    src = Src()
                Dst(src, test_self = self)
            depthai.run(p, 
                        compilation=mock_compilation, 
                        main_loop=main_loop)

    def test_reversed(self):
        self.i = 0
        def main_loop(pipeline, context):
            output_queues = [device.getOutputQueue(None)
                            for device in context["running_devices"].values()]
            for i in range(1, 10000):
                for node in pipeline:
                    node.dispatch()
                for output_queue in output_queues:
                    if (rv := output_queue.tryGet()):
                        self.i += 1
                        self.assertEqual(self.i, rv)
                        if self.i == 100: raise StopIteration()

        with self.assertRaises(StopIteration):
            with (p := depthai.Pipeline()):
                src = Src()
                with depthai.DeviceRef():
                    Dst(src, test_self = self)
            depthai.run(p, 
                        compilation=mock_compilation, 
                        main_loop=main_loop)


    def test_cross_device(self):
        def main_loop(pipeline, context):
            for i in range(1, 10000):
                for node in pipeline:
                    node.dispatch()

        with self.assertRaises(StopIteration):
            with (p := depthai.Pipeline()):
                src = Src()
                with depthai.DeviceRef("one"):
                    cross_link = depthai.node.Identity(src)
                with depthai.DeviceRef("two"):
                    dst = depthai.node.Identity(cross_link)
                Dst(dst, test_self = self)
            depthai.run(p, 
                        compilation=mock_compilation, 
                        main_loop=main_loop)
