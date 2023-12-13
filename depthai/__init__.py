import depthai.global_state
import depthai.type_checker

# FIXME Conside import .node notation
from depthai_bind import *
from depthai.node import Feedback, Node
from depthai.runtime import Abort
import depthai.core_nodes as node

def pipeline_enter(self):
    global_state.pipeline = []
    global_state.core_pipeline = self
    return self
Pipeline.__enter__ = pipeline_enter

def pipeline_exit(self, *args):
    if args == (None, None, None):
        # TODO Check that no input type is None (breaks with dispatch)
        depthai.type_checker.check_pipeline(global_state.pipeline)
        for node in global_state.pipeline:
            if isinstance(node, depthai.core_node.CoreNode):
                node.create_links()
Pipeline.__exit__ = pipeline_exit
