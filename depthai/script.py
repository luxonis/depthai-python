from depthai.core_node import CoreNode
import inspect
import logging

class Script(CoreNode):
    param_desc = {"script": str}
    def __init_subclass__(cls):
        assert hasattr(cls, "input_desc") and hasattr(cls, "output_desc"), \
                "Please provide description of inputs and output using"\
                "'input_desc' and 'output_desc'. Both are dictionaries"\
                "mapping input/output name to its message type."
        script = getattr(cls, "__script__", None)
        assert script is not None, \
                "Please provide the script in __sciprt__ function"
        signature = inspect.signature(script)
        assert tuple(signature.parameters) == ("node",), \
                "The __script__ function must have exactly one parameter"\
                " and it's name must be 'node'"
        closure = inspect.getclosurevars(script)
        for varname in closure.nonlocals | closure.globals: 
            logging.warning(
                    f"Definition of script {cls} contains variable from"
                    "outer scope. If you are trying to use variable from"
                    "outer scope, it will probably fail.")
        source = inspect.getsourcelines(script)[0][1:]
        indent = ""
        for c in source[0]:
            if c.isspace(): indent += c
            else: break
        cls.source = "".join(line[len(indent):] for line in source)
    def __init__(self, *args, **kwargs):
        kwargs |= {"script": self.source}
        super().__init__(*args, **kwargs)
