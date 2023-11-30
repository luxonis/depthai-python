import depthai.global_state

from depthai_bind import *
from depthai.node import Feedback
import depthai.core_nodes as node

def pipeline_enter(self):
    global_state.pipeline = []
    global_state.core_pipeline = self
    return self
Pipeline.__enter__ = pipeline_enter

def pipeline_exit(self, *args):
    if args == (None, None, None):
        for node in global_state.pipeline:
            node.create_links()
Pipeline.__exit__ = pipeline_exit
