import depthai_bind
import typeguard
from depthai.node import Node
from depthai.device import DefaultDevice

class CoreNode(Node):
    device = DefaultDevice()

    # TODO Is this fancy syntax worth it?
    def __init_subclass__(cls):
        """
        Use annotations to construct input_desc and output_desc for Node.
        Also use the same mechanism to handle parameters.

        Parameters are automatically set using relevant setter methods.
        Setter method with the same common representation is used.
        Common representation is found by removing underscores
        and coverting to lowercase.
        """
        cls.core_node = getattr(depthai_bind.node, cls.__name__)
        cls.param_name_map = {}

        cls.input_desc = {}
        cls.output_desc = {}
        cls.default_output = None
        cls.param_desc = {}
        # FIXME This is fragile but not worth fixing for now
        # https://docs.python.org/3/howto/annotations.html
        for name, type in cls.__annotations__.items():
            # Order can be relied on
            # https://peps.python.org/pep-0526/
            match type:
                case Input(type=message_type):
                    cls.input_desc[name] = message_type
                    getattr(cls.core_node, name)
                case Output(type=message_type):
                    cls.output_desc[name] = message_type
                    getattr(cls.core_node, name)
                    if isinstance(type, DefaultOutput):
                        assert cls.default_output is None, \
                                "Multiple default outputs aren't allowed"
                        cls.default_output = name
                case _:
                    cls.param_desc[name] = type
                    common_repr = name.replace("_", "").lower()
                    for core_name in dir(cls.core_node):
                        if not core_name.startswith("set"): continue
                        if common_repr != core_name[3:].lower(): continue
                        assert name not in cls.param_name_map, \
                                f"Ambiguous setter method for parameter {name}"
                        cls.param_name_map[name] = core_name
                    assert name in cls.param_name_map, \
                            f"Unknown parameter {name}"

    def __init__(self, *args, **kwargs):
        self.params = {}
        input_kwargs = {}
        for key, value in kwargs.items():
            if key in self.param_desc.keys():
                self.params[key] = value
            else:
                input_kwargs[key] = value
        super().__init__(*args, **input_kwargs)

    def emit_core_node(self, core_pipeline):
        self.core_node = core_pipeline.create(self.__class__.core_node)

        for name, value in self.param_desc.items():
            try: value = self.params[name]
            except KeyError:
                try: value = getattr(self.__class__, name)
                except AttributeError:
                    raise LookupError(
                          f"Parameter {name} is missing for node {self}.")
            
            try:
                typeguard.check_type(value, self.param_desc[name])
            except typeguard.TypeCheckError as e:
                raise TypeError(f"Parameter '{name}' type error") from e
            if not isinstance(value, tuple): value = (value,)
            getattr(self.core_node, self.param_name_map[name])(*value)

    def create_links(self, core_pipeline):
        for name, output in self.inputs.items():
            if output is None: continue
            core_pipeline.link(
                    getattr(output.node.core_node, output.name),
                    getattr(self.core_node, name))

class PutType:
    """
    Put is a linguistic pun -- it is the common part of 'inPUT' and 'outPUT'

    It is used to annotate inputs and outputs of CoreNodes.
    """
    def __init__(self, type):
        self.type = type
class Input(PutType): pass
class Output(PutType): pass
class DefaultOutput(Output): pass
