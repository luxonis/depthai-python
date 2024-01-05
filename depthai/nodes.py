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

def resolve_feedback_nodes(pipeline, _):
    for node in pipeline:
        for input_name, put_ref in node.inputs.items():
            if put_ref is None: continue
            if isinstance(put_ref.node, Feedback):
                node.inputs[input_name] = put_ref.node.inputs["input"]
                pipeline.remove(put_ref.node)

T = typing.TypeVar("T")
class Identity(depthai.Node):
    def __run__(self, input: T) -> T:
        return input
