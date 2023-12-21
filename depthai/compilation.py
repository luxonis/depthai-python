import logging
import depthai.type_checker as type_checker
from depthai.runtime import init_host_nodes
from depthai.xlinks import create_xlinks
from depthai.device import resolve_devices, start_devices

#TODO Consider context a class instead of dict
#TODO Fix (or check) __eq__ and __hash__ on DeviceInfo

def default_main_loop(pipeline, _):
    while True:
        for node in pipeline:
            node.dispatch()

# This list is part of read-only interface 
default_compilation = [
    type_checker.check_pipeline,
    resolve_devices,
    create_xlinks,
    start_devices,
    init_host_nodes]

def run(pipeline, 
        compilation = default_compilation,
        preprocessor = [],
        main_loop = default_main_loop,
        **context):
    if not isinstance(preprocessor, list): preprocessor = [preprocessor]
    process = preprocessor + compilation
    for step in process:
        logging.info(f"Running step {step}")
        rv = step(pipeline, context)
        pipeline = pipeline if rv is None else rv
    class DeviceHandler:
        def __enter__(self): pass
        def __exit__(self, *args):
            for device in context["running_devices"].values():
                device.__exit__(*args)
    with DeviceHandler():
        main_loop(pipeline, context)
