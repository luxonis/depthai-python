import depthai.global_state as global_state
import inspect
import typing
import logging

class Node:

    # TODO Use descriptors to specify and check dynamic and static values

    default_output = None
    input_desc = {}
    output_desc = {}
    side = "host"
    sync = True
    queues = {
            "*-blocking": True,
            "*-size": 64, # TODO Set reasonable default
            }

    class PutRef:
        def __init__(self, node, name):
            self.node = node
            self.name = name
        def __repr__(self):
            return f"<{self.name} of {self.node}>"

    def __init_subclass__(cls):
        if "__run__" in cls.__dict__:
            run = cls.__dict__["__run__"]
            sig = inspect.signature(run) 
            assert list(sig.parameters.keys())[0] == "self", \
                'Please use "self" as the first parameter for __run__ method'
            assert cls.input_desc == {}, \
                'Both __run__ method and "input_desc" dictionary are provided.'\
                ' Avoiding conflict.'
            assert cls.output_desc == {}, \
                'Both __run__ method and "output_desc" dictionary are provided'\
                ' Avoiding conflict.'
                
            cls.input_desc = {}
            for name, param in sig.parameters.items():
                if name == "self": continue
                annotation = param.annotation
                if annotation == inspect.Parameter.empty:
                    annotation = typing.Any
                    logging.warning(f"The __run__ method in {cls.__name__}" 
                        f" has no annotation for parameter {name}"
                        " -- assuming typing.Any")
                cls.input_desc[name] = annotation

            cls.output_desc = {}
            return_annotation = sig.return_annotation
            if return_annotation == inspect.Signature.empty:
                return_annotation = typing.Any
                logging.warning(f"The __run__ method in {cls.__name__}" 
                        f" has no annotation for return value"
                        " -- assuming typing.Any")
            if not isinstance(return_annotation, dict):
                return_annotation = {"output": return_annotation}
            cls.output_desc = return_annotation

    def __init__(self, *args, **kwargs):
        self.inputs = {key : None for key in self.input_desc}

        global_state.pipeline.append(self)

        # Save relevant part of stack for identification of the node in
        # the code
        import inspect
        self.def_stack = inspect.stack()
        while self.def_stack[0].function == "__init__":
            self.def_stack.pop(0)

        # Link supplied inputs
        assert len(args) <= len(self.input_desc), "Too much inputs"
        for arg, name in zip(args, self.input_desc):
            self.link(name, arg)
        for key, value in kwargs.items():
            if key in self.input_desc:
                assert self.inputs[key] is None, \
                        f"Input {value} passed as a keyword argument {key}" \
                        " was already specified with a positional argument."
                self.link(key, value)
                continue
            assert hasattr(self.__class__, key), \
                    f"Unknown input {key}. "\
                    "Instance parameters can only overwrite class parameters."
            setattr(self, key, value)

    def __repr__(self):
        # Try block is here to avoid exception recursion with __getattr__
        # __getattr__ calls __repr__
        try:
            position = "{}, {}".format(
                self.def_stack[0].positions.lineno,
                self.def_stack[0].positions.col_offset)
        except:
            position = "unknown position"
        return f"<{self.__class__.__name__} at {position}>"

    def link(self, input_name, output_ref):
        if isinstance(output_ref, Node):
            output_ref = output_ref.get_default_output()
        assert isinstance(output_ref, Node.PutRef)
        assert output_ref.node in global_state.pipeline, \
            f"Input {input_name} of {self} is set to a node that isn't "\
            "in the pipeline. Perhaps you forgot to call super().__init__?"
        self.inputs[input_name] = output_ref

    def get_default_output(self):
        """
        Allows this syntax: NN(MonoCamera(...))
        """
        if self.default_output is None and len(self.output_desc) == 1:
            self.default_output ,= self.output_desc.keys()
        assert self.default_output, \
                f"Node {self.__class__.__name__} has no default output"
        return Node.PutRef(self, self.default_output)

    def __iter__(self):
        """
        Allows this syntax: out, raw = MonoCamera(...)
        """
        for output in self.output_desc:
            yield Node.PutRef(self, output)

    def __getattr__(self, name):
        """
        Allows this syntax: NN(MonoCamera(...).raw)
        """
        if name not in self.output_desc:
            raise AttributeError(f"{self} has no attribute '{name}'",
                                 name=name, obj=self)
        return Node.PutRef(self, name)

import typing
class Feedback(Node):
    # Type cannot be generic, the input is ignored and output-only generic
    # nodes are disallowed
    input_desc = {"input": typing.Any}
    output_desc = {"output": typing.Any}

    # Specify no arguments
    def __init__(self): super().__init__()

    def attach(self, output_ref):
        self.link("input", output_ref)
