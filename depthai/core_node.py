import depthai_bind
import typeguard
from depthai.node import Node
from depthai.device import DeviceRef

class CoreNode(Node):
    """
    Parameters are automatically set using relevant setter methods.  Setter
    method with the same common representation is used.  Common representation
    is found by removing underscores and coverting to lowercase.
    """

    device = DeviceRef()

    def __init__(self, *args, **kwargs):
        self.params = {}
        super_kwargs = {}
        for key, value in kwargs.items():
            if key in self.param_desc.keys():
                self.params[key] = value
            else:
                super_kwargs[key] = value
        super().__init__(*args, **super_kwargs)

    def get_setter_method(self, name):
        common_repr = name.replace("_", "").lower()
        candidates = set()
        for core_name in dir(self.core_node):
            if not core_name.startswith("set"): continue
            if common_repr != core_name[3:].lower(): continue
            candidates.add(core_name)
        assert len(candidates) == 1, \
                f"Cannot find a setter method for parameter {name}"
        return list(candidates)[0]

    def emit_core_node(self, core_pipeline):
        self.core_node = core_pipeline.create(
                            getattr(depthai_bind.node, self.__class__.__name__))

        for name, desc in self.param_desc.items():
            if isinstance(desc, tuple):
                type, value = desc
            else:
                type = desc
                if name not in self.params: continue
            if name in self.params:
                value = self.params[name]
            
            try:
                typeguard.check_type(value, type)
            except typeguard.TypeCheckError as e:
                raise TypeError(f"Parameter '{name}' type error") from e
            if not isinstance(value, tuple): value = (value,)

            getattr(self.core_node, self.get_setter_method(name))(*value)

    def create_links(self, core_pipeline):
        for name, output in self.inputs.items():
            if output is None: continue
            if isinstance(output.node.core_node, depthai_bind.node.Script):
                src = output.node.core_node.outputs[output.name]
            else:
                src = getattr(output.node.core_node, output.name)
            if isinstance(self.core_node, depthai_bind.node.Script):
                dst = self.core_node.inputs[name]
            else:
                dst = getattr(self.core_node, name)
            core_pipeline.link(src, dst)
                    
