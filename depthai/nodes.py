import depthai
from depthai.core_nodes import *
from depthai.xlinks import *
import typing

class Feedback(depthai.Node):
    # Type cannot be generic, the input is ignored and output-only generic
    # nodes are disallowed
    input_desc = {"input": typing.Any}
    output_desc = {"output": typing.Any}

    def attach(self, output_ref):
        self.link("input", output_ref)

T = typing.TypeVar("T")
class Identity(depthai.Node):
    def __run__(self, input: T) -> T:
        return input
