import unittest
import depthai.core_nodes
from depthai.core_node import CoreNode
import depthai_bind
import depthai

class TestNodes(unittest.TestCase):
    def test_names(self):
        with depthai.Pipeline():
            pipeline = depthai_bind.Pipeline()
            for name in dir(depthai.core_nodes):
                node_class = getattr(depthai.core_nodes, name)
                if not isinstance(node_class, type): continue
                if not issubclass(node_class, CoreNode): continue
                if node_class == CoreNode: continue
                node = node_class()
                node.emit_core_node(pipeline)
                for param in node_class.param_desc.keys():
                    node.get_setter_method(param)
