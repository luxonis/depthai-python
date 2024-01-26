import collections

import depthai_bind
from depthai.pipeline import Pipeline
import depthai.global_state as global_state
import depthai.device
import logging

class DeviceRef(str):
    def __enter__(self):
        self.old = global_state.device
        global_state.device = self
    def __exit__(self, *args):
        assert global_state.device == self
        global_state.device = self.old

def start_devices(pipeline, context):
    host_pipeline = Pipeline()
    device_pipelines = collections.defaultdict(depthai_bind.Pipeline)
    for node in pipeline:
        if node.device is None: 
            host_pipeline.append(node)
        else:
            node.emit_core_node(device_pipelines[node.device])

    for node in pipeline:
        if node.device is not None: 
            node.create_links(device_pipelines[node.device])

    for device_ref, pipeline in device_pipelines.items():
        if device_ref in context["devices"]:
            args = [context["devices"][device_ref]]
        else:
            args = []
        device = depthai_bind.Device(pipeline, *args)
        context["running_devices"][device_ref] = device
        context["exit_routines"].append(lambda: device.__exit__(*(3*[None])))

    return host_pipeline
