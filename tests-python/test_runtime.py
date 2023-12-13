import unittest
import depthai
import depthai.runtime

class Src(depthai.Node):
    actions = []
    def __init__(self):
        self.i = 0
        super().__init__()
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
        class Node(depthai.Node):
            def __init__(node_self, *args, **kwargs):
                self.node = node_self
                super().__init__(*args, **kwargs)
            def __run__(self, input: int) -> None: pass
        with depthai.Pipeline(): 
            Node(Src())
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, lambda pipeline: None)
        self.node.is_queue_blocking("input")
        self.node.get_queue_size("input")
        
    def test_class_settings(self):
        class Node(depthai.Node):
            queues = {"input-size": 42}
            def __init__(node_self, *args, **kwargs):
                self.node = node_self
                super().__init__(*args, **kwargs)
            def __run__(self, input: int) -> None: pass
        with depthai.Pipeline(): 
            Node(Src())
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, lambda pipeline: None)
        self.assertEqual(42, self.node.get_queue_size("input"))

    def test_instance(self):
        class Node(depthai.Node):
            queues = {"input-size": 100}
            def __init__(node_self, *args, **kwargs):
                self.node = node_self
                super().__init__(*args, **kwargs)
            def __run__(self, input: int) -> None: pass
        with depthai.Pipeline(): 
            Node(Src(), queues={"input-size": 42})
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, lambda pipeline: None)
        self.assertEqual(42, self.node.get_queue_size("input"))
 
    def test_star(self):
        class Node(depthai.Node):
            queues = {"input1-size": 43, "*-size": 42}
            def __init__(node_self, *args, **kwargs):
                self.node = node_self
                super().__init__(*args, **kwargs)
            def __run__(self, input0: int, input1: int) -> None: pass
        with depthai.Pipeline(): 
            Node(Src())
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, lambda pipeline: None)
        self.assertEqual(42, self.node.get_queue_size("input0"))       
        self.assertEqual(43, self.node.get_queue_size("input1"))       
        
class Dst(depthai.Node):
    received = []
    def __run__(self, input: int) -> None:
        Dst.received.append(input)

class StuckDst(depthai.Node):
    def __run__(self, input: int) -> None:
        raise depthai.Abort()

def finite_main_loop(pipeline):
    for _ in range(1000):
        for node in pipeline:
            node.dispatch()

class TestRuntime(unittest.TestCase):
    def setUp(self):
        Src.actions = []
        Dst.received = []

    def test_simple(self):
        with depthai.Pipeline():
            Dst(Src(),
                queues={"input-blocking": True, "input-size": 50})
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, finite_main_loop)
        self.assertEqual(Dst.received, list(range(100)))
        self.assertEqual(Src.actions[:100], [f"run {i}" for i in range(100)])

    def test_blocking(self):
        with depthai.Pipeline():
            StuckDst(Src(),
                queues={"input-blocking": True, "input-size": 50})
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, finite_main_loop)
        self.assertGreater(len(Src.actions), 25)
        self.assertLess(len(Src.actions), 75)

    def test_queue_size(self):
        with depthai.Pipeline():
            StuckDst(Src(),
                queues={"input-blocking": True, "input-size": 5})
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, finite_main_loop)
        self.assertGreater(len(Src.actions), 2)
        self.assertLess(len(Src.actions), 10)

    def test_non_blocking(self):
        with depthai.Pipeline():
            StuckDst(Src(),
                queues={"input-blocking": False, "input-size": 50})
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, finite_main_loop)
        self.assertEqual(Src.actions[:100], [f"run {i}" for i in range(100)])

    def test_abort(self):
        class AbortDst(depthai.Node):
            def __init__(self, *args, **kwargs):
                self.test = kwargs["test"]
                del kwargs["test"]
                self.last = None
                super().__init__(*args, **kwargs)

            def __run__(self, input: int) -> None:
                if self.last == None:
                    self.last = input
                    raise depthai.Abort()
                self.test.assertEqual(self.last, input)
                self.last = None

        with depthai.Pipeline():
            AbortDst(Src(),
                queues={"input-blocking": True, "input-size": 50},
                test=self)
            pipeline = depthai.global_state.pipeline # TODO Rewrite
        depthai.runtime.run(pipeline, finite_main_loop)

