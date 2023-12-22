import depthai.global_state as global_state
import depthai.pipeline
import inspect
import typing
import logging

# TODO Access to fuctionality of Pipeline, Device and Bootloader (integration with RH)

class Node:

    default_output = None
    input_desc = None
    output_desc = None
    device = None
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
            assert cls.input_desc is None, \
                'Both __run__ method and "input_desc" dictionary are provided.'\
                ' Avoiding conflict.'
            assert cls.output_desc is None, \
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
                return_annotation = None
            if not isinstance(return_annotation, dict):
                return_annotation = {"output": return_annotation}
            cls.output_desc = return_annotation

        if "__node_init__" in cls.__dict__:
            sig = inspect.signature(cls.__dict__["__node_init__"])
            parameters = list(sig.parameters.keys())
            assert parameters[0] == "self", 'Please use "self"'\
                    ' as the first parameter for __node_init__ method'
            cls.init_kwarg_names = set(parameters[1:])
        else:
            cls.init_kwarg_names = set()


    # It would be more systematic to have pipeline as another node
    # parameter. However, this would make it accessible to the user.
    # This would be confusing since allegiance to a pipeline changes in
    # compile time.
    def __init__(self, *args, pipeline=None, **kwargs):
        # Resolve global context
        if pipeline is None: pipeline = global_state.pipeline
        pipeline.append(self)
        if global_state.device is not None and "device" not in kwargs:
            kwargs["device"] = global_state.device

        self.inputs = {key : None for key in self.input_desc}

        # Save relevant part of stack for identification of the node in
        # the code
        import inspect
        self.def_stack = inspect.stack()
        while self.def_stack[0].function == "__init__":
            self.def_stack.pop(0)

        # Link supplied inputs and set parameters
        assert len(args) <= len(self.input_desc), "Too much inputs"
        for arg, name in zip(args, self.input_desc):
            self.link(name, arg)
        self.init_kwargs = {}
        for key, value in kwargs.items():
            if key in self.input_desc:
                assert self.inputs[key] is None, \
                        f"Input {value} passed as a keyword argument {key}" \
                        " was already specified with a positional argument."
                self.link(key, value)
                continue
            if hasattr(self.__class__, key):
                setattr(self, key, value)
                continue
            if key in self.init_kwarg_names:
                self.init_kwargs[key] = value
                continue
            raise NameError(f"Unknown keyword argument {key}")

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
        if not isinstance(global_state.pipeline, depthai.pipeline.NoPipeline):
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
