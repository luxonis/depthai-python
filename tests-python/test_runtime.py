import unittest
import depthai

class Src(depthai.Node):
    actions = []
    def __node_init__(self):
        self.i = 0
    def __run__(self) -> int:
        if self.i < 100: 
            Src.actions.append(f"run {self.i}")
            rv = self.i
            self.i += 1
            return rv
        Src.actions.append("abort")
        raise depthai.Abort()

class TestQueues(unittest.TestCase):
    def test_exists_default(self):
        test_self = self
        class Node(depthai.Node):
            def __node_init__(self):
                test_self.node = self
            def __run__(self, input: int): pass
        with (pipeline := depthai.Pipeline()): 
            Node(Src())
        depthai.run(pipeline, main_loop=lambda pipeline, _: None)
        self.node.is_queue_blocking("input")
        self.node.get_queue_size("input")
        
    def test_class_settings(self):
        test_self = self
        class Node(depthai.Node):
            queues = {"input-size": 42}
            def __node_init__(self):
                test_self.node = self
            def __run__(self, input: int) -> None: pass
        with (pipeline := depthai.Pipeline()): 
            Node(Src())
        depthai.run(pipeline, main_loop=lambda pipeline, _: None)
        self.assertEqual(42, self.node.get_queue_size("input"))

    def test_instance(self):
        test_self = self
        class Node(depthai.Node):
            queues = {"input-size": 100}
            def __node_init__(self):
                test_self.node = self
            def __run__(self, input: int) -> None: pass
        with (pipeline := depthai.Pipeline()): 
            Node(Src(), queues={"input-size": 42})
        depthai.run(pipeline, main_loop=lambda pipeline, _: None)
        self.assertEqual(42, self.node.get_queue_size("input"))
 
    def test_star(self):
        test_self = self
        class Node(depthai.Node):
            queues = {"input1-size": 43, "*-size": 42}
            def __node_init__(self):
                test_self.node = self
            def __run__(self, input0: int, input1: int) -> None: pass
        with (pipeline := depthai.Pipeline()): 
            Node(Src())
        depthai.run(pipeline, main_loop=lambda pipeline, _: None)
        self.assertEqual(42, self.node.get_queue_size("input0"))       
        self.assertEqual(43, self.node.get_queue_size("input1"))       
        
class Dst(depthai.Node):
    received = []
    def __run__(self, input: int) -> None:
        Dst.received.append(input)

class StuckDst(depthai.Node):
    def __run__(self, input: int) -> None:
        raise depthai.Abort()

def finite_main_loop(pipeline, context):
    for _ in range(1000):
        for node in pipeline:
            node.dispatch()

class TestRuntime(unittest.TestCase):
    def setUp(self):
        Src.actions = []
        Dst.received = []

    def test_simple(self):
        with (pipeline := depthai.Pipeline()): 
            Dst(Src(),
                queues={"input-blocking": True, "input-size": 50})
        depthai.run(pipeline, main_loop=finite_main_loop)
        self.assertEqual(Dst.received, list(range(100)))
        self.assertEqual(Src.actions[:100], [f"run {i}" for i in range(100)])

    def test_blocking(self):
        with (pipeline := depthai.Pipeline()): 
            StuckDst(Src(),
                queues={"input-blocking": True, "input-size": 50})
        depthai.run(pipeline, main_loop=finite_main_loop)
        self.assertGreater(len(Src.actions), 25)
        self.assertLess(len(Src.actions), 75)

    def test_queue_size(self):
        with (pipeline := depthai.Pipeline()): 
            StuckDst(Src(),
                queues={"input-blocking": True, "input-size": 5})
        depthai.run(pipeline, main_loop=finite_main_loop)
        self.assertGreater(len(Src.actions), 2)
        self.assertLess(len(Src.actions), 10)

    def test_non_blocking(self):
        with (pipeline := depthai.Pipeline()): 
            StuckDst(Src(),
                queues={"input-blocking": False, "input-size": 50})
        depthai.run(pipeline, main_loop=finite_main_loop)
        self.assertEqual(Src.actions[:100], [f"run {i}" for i in range(100)])

    def test_abort(self):
        test = self
        class AbortDst(depthai.Node):
            def __node_init__(self):
                self.last = None

            def __run__(self, input: int) -> None:
                if self.last == None:
                    self.last = input
                    raise depthai.Abort()
                test.assertEqual(self.last, input)
                self.last = None

        with (pipeline := depthai.Pipeline()): 
            AbortDst(Src(),
                queues={"input-blocking": True, "input-size": 50})
        depthai.run(pipeline, main_loop=finite_main_loop)

    def test_feedback(self):
        with (pipeline := depthai.Pipeline()):
            feedback = depthai.node.Feedback()
            Dst(feedback)
            feedback.attach(Src())
        depthai.run(pipeline, main_loop=finite_main_loop)
        self.assertEqual(Dst.received, list(range(100)))
        self.assertEqual(Src.actions[:100], [f"run {i}" for i in range(100)])
