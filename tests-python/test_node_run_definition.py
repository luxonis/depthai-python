import unittest
import depthai as dai
import typing

# Different names are used for inherited

class TestCustomNodes(unittest.TestCase):
    def test_initialization(self):
        with self.assertNoLogs(level="WARNING"):
            class Node(dai.Node):
                def __run__(self, input: int) -> str: pass
        with dai.Pipeline():
            Node()

    def test_no_self(self):
        with self.assertRaises(AssertionError):
            class Node(dai.Node):
                def __run__(this): pass

    def test_io(self):
        class Node(dai.Node):
            # The argument name is intentionally swapped to test
            # that Python doesn't reorder them
            def __run__(self, input1: int, input0: bool) -> {
                              "output1": str, "output0": float}: pass
        inputs = {
            "input1" : int,
            "input0" : bool}
        self.assertEqual(Node.input_desc, inputs)
        self.assertEqual(list(Node.input_desc.keys()), list(inputs.keys()))

        outputs = {
            "output1" : str,
            "output0" : float}
        self.assertEqual(Node.output_desc, outputs)
        self.assertEqual(list(Node.output_desc.keys()), list(outputs.keys()))

    def test_nondict(self):
        class Node(dai.Node):
            def __run__(self) -> int: pass
        self.assertIn("output", Node.output_desc)
        self.assertEqual(Node.output_desc["output"], int)

    def test_default(self):
        with self.assertLogs(level="WARNING"):
            class Node(dai.Node):
                def __run__(self, input): pass
            self.assertEqual(Node.input_desc["input"], typing.Any)
            self.assertEqual(Node.output_desc["output"], typing.Any)

    def test_forget_superinit(self):
        with self.assertRaises(AssertionError):
            class Src(dai.Node):
                def __init__(self):
                    #Forgot to call super().__init__()
                    pass
                def __run__(self) -> str: pass
            class Dst(dai.Node):
                def __run__(self, input: str) -> None: pass
            with dai.Pipeline():
                Dst(Src())

