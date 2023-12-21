import collections

import depthai_bind
from depthai.pipeline import Pipeline
import depthai.global_state as global_state
import depthai.device
import logging

class DeviceRef:
    # TODO This interface seems bloated
    def __enter__(self):
        self.old = global_state.device
        global_state.device = self
    def __exit__(self, *args):
        assert global_state.device == self
        global_state.device = self.old

class DeviceInfo(DeviceRef, depthai_bind.DeviceInfo):
    # TODO Check that this should be used
    # I still don't fully understand difference between this and DeviceDesc
    def __eq__(this, other):
        # TODO Double check
        #return this.name == other.name or this.mxid == other.mxid # FIXME
        return this.mxid == other.mxid
    def __hash__(self):
        # TODO Double check that hash collision aren't a problem
        return hash(self.mxid)

class DefaultDevice(DeviceRef):
    pass

class NamedDevice(DeviceRef):
    def __init__(self, name):
        self.name = name

def resolve_devices(pipeline, context):
    for node in pipeline:
        match node.device:
            case None: #Host
                pass
            case depthai.device.DeviceInfo():
                pass
            case depthai_bind.DeviceInfo():
                pass
            case depthai.device.DefaultDevice():
                if "default_device" in context:
                    node.device = context["default_device"]
                    continue
                logging.info("Default device was not specified. "
                             "Looking for one.")
                devs = depthai_bind.Device.getAllAvailableDevices() 
                if len(devs) == 0:
                    raise RuntimeError("Could not resolve DefaultDevice. "
                                       "No device available.")
                if len(devs) > 1:
                    raise RuntimeError("Could not resolve DefaultDevice. "
                                       "Multiple devices available.")
                node.device ,= devs
            case depthai.device.NamedDevice(name=name):
                node.device = context["devices"][name]
            case _:
                print(node.device)
                raise TypeError(f"Node {node} device is set to unknown type. "
                                "Supported types: DeviceInfo, DefaultDevice, "
                                f"NamedDevice. Got {node.device}")
        if isinstance(node.device, depthai_bind.DeviceInfo) \
        and not isinstance(node.device, depthai.device.DeviceInfo):
            node.device = depthai.device.DeviceInfo(
                    node.device.name,
                    node.device.mxid,
                    node.device.state,
                    node.device.protocol,
                    node.device.platform,
                    node.device.status)


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

    context["running_devices"] = {device_info: depthai_bind.Device(
                                                    pipeline)#, #FIXME
                                                    #device_info)
                        for device_info, pipeline in device_pipelines.items()}
    return host_pipeline


