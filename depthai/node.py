import depthai.global_state as global_state

class Node:

    class OutputRef:
        def __init__(self, node, name):
            self.node = node
            self.name = name
        def __repr__(self):
            return f"<{self.name} of {self.node}>"

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
        for arg, name in zip(args, self.input_desc):
            self.link(name, arg)
        for key, value in kwargs.items():
            assert key in self.input_desc, f"Unknown input {key}"
            assert self.inputs[key] is None, \
                    f"Input {value} passed as a keyword argument {key}" \
                    " was already specified with a positional argument."
            self.link(key, value)

    def __repr__(self):
        return "<{} at {}, {}>".format(
            self.__class__.__name__,
            self.def_stack[0].positions.lineno,
            self.def_stack[0].positions.col_offset,
            )

    def link(self, input_name, output_ref):
        if isinstance(output_ref, Node):
            output_ref = output_ref.get_default_output()
        assert isinstance(output_ref, Node.OutputRef)
        #TODO Check type
        self.inputs[input_name] = output_ref

    def get_default_output(self):
        """
        Allows this syntax: NN(MonoCamera(...))
        """
        assert self.default_output, \
                f"Node {self.__class__.__name__} has no default output"
        return Node.OutputRef(self, self.default_output)

    def __iter__(self):
        """
        Allows this syntax: out, raw = MonoCamera(...)
        """
        for output in self.output_desc:
            yield Node.OutputRef(self, output)

    def __getattr__(self, name):
        """
        Allows this syntax: NN(MonoCamera(...).raw)
        """
        return Node.OutputRef(self, name)
