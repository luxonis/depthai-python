from depthai.nodes import Feedback
import mypy.api
import typing
import types
import builtins
import logging
import re
import sys
import os
import os.path

"""
This module uses MyPy to check type information.
"""

MYPY_COMMAND = ["--follow-imports", "silent", "-c"]

construct_path = "MYPYPATH" not in os.environ
if construct_path: os.environ["MYPYPATH"] = ""

def construct_imports(*types):
    for type in types:
        if (subtypes := typing.get_args(type)) != ():
            yield "import typing"
            yield from construct_imports(*subtypes)
            continue
        if type is None:
            continue
        try:
            if construct_path:
                if type.__module__ in sys.stdlib_module_names: continue
                path = sys.modules[type.__module__].__file__
                if "site-packages" in path: continue
                if os.environ["MYPYPATH"] != "":
                    os.environ["MYPYPATH"] += ":"
                os.environ["MYPYPATH"] += os.path.dirname(path)
        finally:
            if type.__module__ == "__main__":
                yield "import {} as __main__".format(
                        os.path.basename(path).removesuffix(".py"))
            else:
                yield f"import {type.__module__}"

def type_repr(type):
    """
    Common __repr__ for classes doesn't follow the eval(repr(property)).
    This does. Or least better than __repr__.
    """
    if type == types.NoneType:
        # None is weird
        # hasattr(type(None).__module__, "NoneType") == False
        return "None"
    match type:
        case builtins.type() | typing.TypeVar():
            return f"{type.__module__}.{type.__name__}"
        case types.UnionType():
            prefix = "typing.Union"
        case types.GenericAlias():
            prefix = type_repr(typing.get_origin(type))
        case types.EllipsisType():
            return "..."
        case None:
            return "None"
        case _:
            raise NotImplementedError()

    return prefix + "[" + ", ".join(
            type_repr(t) for t in typing.get_args(type)
            ) + "]"

def check_type(instance, type):
    """
    This is slow. Use typeguard instead. But keep it here. 
    It is a useful for testing other functions.

    It finds all imports and then tries to assign variable of the instance type
    to a variable of the other type. It does it twice. First, it only creates
    the variables but doesn't assign them. This way we can detect that the
    error is not in the assignment itself.
    """
    program = list(construct_imports(type, builtins.type(instance))) + [
                f"_from: {type_repr(builtins.type(instance))}",
                f"_to: {type_repr(type)}"]
    result = mypy.api.run(MYPY_COMMAND + ["\n".join(program)])
    if result[2] != 0:
        logging.warning("Skipped type checking (please report)"
            f"for instance '{instance}' and type '{type}'")
        logging.debug("MyPy details:") 
        for line in result[0].splitlines(): logging.debug("\t" + line) 
        return

    program[-1] = f"_to: {type_repr(type)} = _from"
    result = mypy.api.run(MYPY_COMMAND + ["\n".join(program)])
    if result[2] == 0: return
    logging.debug("MyPy details:") 
    for line in result[0].splitlines(): logging.debug("\t" + line) 

    raise TypeError(f"Type mismatch: instance '{instance}' and type '{type}'")

def check_pipeline(pipeline, _):
    """
    Check type correctness of the give pipeline. 

    It works by creating a equvivalent program and checking it with MyPy. We
    define a function for each used node with respective inputs and outputs.
    Then the pipeline is constructed from these functions. This is passed to
    MyPy and checked.

    I don't have full trust in this. I suspect it will break when complicated
    application structure and build system is used. The import system just
    doesn't feel robust. Also, MyPy may report errors that can be mysterious to
    the user.
    """

    def get_topological_order(pipeline):
        pipeline = list(pipeline)
        while pipeline:
            for node in pipeline:
                for name, input in node.inputs.items():
                    if input is None: continue
                    if input.node in pipeline: break
                else:
                    yield node
                    pipeline.remove(node)

    def get_types(pipeline):
        for node in pipeline:
            yield from node.output_desc.values()
            yield from node.input_desc.values()

    def construct_functions(pipeline):
        def construct_return_type(node):
            if len(node.output_desc) == 0:
                return "None"
            return ("tuple["
                + ", ".join(map(type_repr, node.output_desc.values()))
                + "]")

        for node in pipeline:
            yield "def {}({}) -> {}: pass".format(
                    node.__class__.__name__,
                    ", ".join(f"{k}: {type_repr(v)}" 
                              for k, v in node.input_desc.items()),
                    construct_return_type(node)) + \
                  " # type: ignore[empty-body, no-redef]"

    pipeline = filter(lambda x: not isinstance(x, Feedback), pipeline)
    pipeline = list(get_topological_order(pipeline))
    program = list(construct_imports(*get_types(pipeline))) 
    program += list(construct_functions(pipeline))


    # Run for the first time to detect problem with program construction
    logging.debug("Representation of nodes for MyPy")
    for line in program: logging.debug("\t" + line) 
    result = mypy.api.run(MYPY_COMMAND + ["\n".join(program)])
    if result[2] != 0:
        logging.warning("Skipped type checking of pipeline (please report)")
        logging.debug("MyPy details:")
        for line in result[0].splitlines(): logging.debug("\t" + line) 
        return

    # Construct the pipeline with defined functions
    program.append("import typing")
    program.append("def NoConn() -> typing.Any: pass # type: ignore[empty-body]")
    offset = len(program)
    logging.debug(f"Offset of pipeline in representations is {offset}")
    for i, node in enumerate(pipeline):
        node.i = i

        # Every line has this form
        # first_output44, second_output44 = NodeName(NoConn(), output42)
        outputs = [out + str(i) for out in node.output_desc.keys()]
        if len(outputs) == 0: line = ""
        if len(outputs) == 1: line = outputs[0] + ",="
        if len(outputs)  > 1: line = ",".join(outputs) + "="
        line += node.__class__.__name__
        inputs = ["NoConn()" if input == None 
                             or isinstance(input.node, Feedback)
                                  else input.name + str(input.node.i)
                      for input in node.inputs.values()]
        line += "(" + ", ".join(inputs) + ")"
        program.append(line)

    # Second run
    logging.debug("Representation of pipeline for MyPy:")
    for line in program: logging.debug("\t" + line) 
    result = mypy.api.run(MYPY_COMMAND + ["\n".join(program)])
    if result[2] == 0: return

    logging.debug("MyPy details:")
    for line in result[0].splitlines(): logging.debug("\t" + line) 
    report = ["Pipeline type error: (consult doc for details)"]
    for line in result[0].splitlines():
        # Reconstruct node index from line number
        if match := re.match(r"<string>:(\d*): error: (.*)", line.strip()):
            linenum, message = match.groups()
            node_idx = int(linenum) - offset - 1
            assert node_idx >= 0
            report.append(str(pipeline[node_idx]) + ": " + message)
    raise TypeError("\n".join(report))
