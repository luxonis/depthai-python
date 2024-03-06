from depthai.node import Node
from depthai.runtime import Abort
import depthai.messages as message
import depthai.proto.ImgFrame_pb2
import depthai.proto.Detections_pb2
import depthai_bind

import typing
import json
import os
import os.path
import numpy as np
import queue

# TODO Check name collision

# TODO Rewrite using generic class 
# -- allows for OutNode[dai.message.ImgFrame](image, name = "...")
# use __parameters__ in __class_getitem__ to allow specialization
# change the generic variable for the give value

# TODO Double check buffering

def create_pipe(name, context):
    os.makedirs(context["pipe_path"], exist_ok=True)
    *prefix, suffix = name.split(".")
    path = os.path.join(context["pipe_path"], name)
    if not os.path.exists(path):
        os.mkfifo(path)
    # RDWR is needed https://stackoverflow.com/
    # questions/46537450/python-open-fifo-blocks-forever
    fd = os.open(path, os.O_RDWR | os.O_NONBLOCK) 
    context["exit_routines"].append(lambda: os.close(fd))
    return fd

class InOutNode(Node):
    sync = False
    def __node_init__(self, name, __context__):
        self.service_out_node = __context__["service_out_node"]
        self.pipe_name = name + self.suffix
        self.pipe = create_pipe(self.pipe_name, __context__)
        self.service_out_node.enqueue_message({
            "type": "new_pipe",
            "pipe_name": self.pipe_name,
            })
        self.buffer = bytearray()
        self.halted = False

class InNode(InOutNode):
    suffix = ".in"
    def __run__(self) -> typing.Any:
        try:
            self.buffer += os.read(self.pipe, 10000000) # TODO Value is temporary
        except OSError as e:
            if e.errno != 11:
                if not self.halted: 
                    self.service_out_node.enqueue_message({
                        "type": "pipe_error",
                        "pipe": self.pipe_name,
                        "errno": e.errno,
                        "message": e.strerror,
                        })               
                self.halted = True
        if len(self.buffer) < 4: return
        message_len = int.from_bytes(self.buffer[:4])
        if len(self.buffer) < message_len: return
        self.halted = False
        type_id = int.from_bytes(self.buffer[4:6])
        repr = bytes(self.buffer[6:message_len])
        self.buffer = self.buffer[message_len:]

        match type_id:
            case 1:
                pb2 = depthai.proto.ImgFrame_pb2.ImgFrame()
                pb2.ParseFromString(repr)
                output = depthai.message.ImgFrame()
                output.setType(message.ImgFrame.Type.BGR888p)
                output.setWidth(pb2.width)
                output.setHeight(pb2.height)
                output.setWidth(pb2.width)
                output.setFrame(np.stack([
                        np.array(color, dtype=np.uint8)
                            .reshape(pb2.height, pb2.width)
                        for color in (pb2.blue, pb2.green, pb2.red)]))
                return output
            case 2:
                pb2 = depthai.proto.Detections_pb2.Detections()
                pb2.ParseFromString(repr)
                detections = []
                for pb2_d in pb2.detections:
                    d = depthai_bind.SpatialImgDetection()
                    d.label = pb2_d.label
                    d.confidence = pb2_d.confidence
                    d.xmin = pb2_d.xmin
                    d.xmax = pb2_d.xmax
                    d.ymin = pb2_d.ymin
                    d.ymax = pb2_d.ymax
                    #d.spatialCoordinates = depthai_bind.Point3f(
                    #        pb2_d.spatial_x,
                    #        pb2_d.spatial_y,
                    #        pb2_d.spatial_z)
                    #d.boundingBoxMapping = depthai_bind.
                    detections.append(d)
                output = depthai.message.SpatialImgDetections()
                # Yes, it first needs to bo created and only the assigned
                # For some reason append doesn't work
                # Seems like an py_bind bug/limitation
                output.detections = detections
                return output


class OutNode(InOutNode):
    suffix = ".out"
    def __run__(self, input: typing.Any):
        if isinstance(input, message.ImgFrame):
            type_id = 1
            assert input.getType() == message.ImgFrame.Type.BGR888p
            def get_color(index):
                return input.getFrame()[index, ...].flatten().tolist()
            pb2 = depthai.proto.ImgFrame_pb2.ImgFrame(
                    width = input.getWidth(),
                    height = input.getHeight(),
                    blue = get_color(0),
                    green = get_color(1),
                    red = get_color(2))
        if isinstance(input, message.ImgDetections) \
        or isinstance(input, message.SpatialImgDetections):
            type_id = 2
            def convert_detection(d):
                pb2 = depthai.proto.Detections_pb2.Detection(
                        label = d.label,
                        confidence = d.confidence,
                        xmin = d.xmin,
                        xmax = d.xmax,
                        ymin = d.ymin,
                        ymax = d.ymax)
                if isinstance(input, message.SpatialImgDetections):
                    pb2.spatial_x = d.spatialCoordinates.x
                    pb2.spatial_y = d.spatialCoordinates.y
                    pb2.spatial_z = d.spatialCoordinates.z
                    pb2.roi_x = d.boundingBoxMapping.roi.x
                    pb2.roi_y = d.boundingBoxMapping.roi.x
                    pb2.roi_width = d.boundingBoxMapping.roi.width
                    pb2.roi_height = d.boundingBoxMapping.roi.height
                    pb2.low_threshold = \
                        d.boundingBoxMapping.depthThresholds.lowerThreshold
                    pb2.high_threshold = \
                        d.boundingBoxMapping.depthThresholds.higherThreshold
                return pb2

            pb2 = depthai.proto.Detections_pb2.Detections(detections = [
                convert_detection(d) for d in input.detections])

        if input is not None: 
            repr = pb2.SerializeToString()
            self.buffer += (4 + 2 + len(repr)).to_bytes(4)
            self.buffer += type_id.to_bytes(2)
            self.buffer += repr

        if len(self.buffer) == 0: return
        try:
            count = os.write(self.pipe, self.buffer)
        except OSError as e:
            if not self.halted:
                self.service_out_node.enqueue_message({
                    "type": "pipe_error",
                    "pipe": self.pipe_name,
                    "errno": e.errno,
                    "message": e.strerror,
                    })
            self.halted = True
        else:
            self.halted = False
            self.buffer = self.buffer[count:]

class ServiceOutNode(Node):
    sync = False
    def __node_init__(self):
        self.buffer = bytearray()
    def enqueue_message(self, message: dict):
        self.input_queues["input"].put(message)
    def __run__(self, input: typing.Any):
        if input is not None:
            self.buffer += (json.dumps(input) + "\n").encode("ascii")
        try:
            count = os.write(self.pipe, self.buffer)
        except OSError as e:
            pass
        self.buffer = self.buffer[count:]

class ServiceInNode(Node):
    sync = False
    def __node_init__(self):
        self.buffer = bytearray()
    def __run__(self) -> typing.Any:
        try:
            self.buffer += os.read(self.pipe, 1000)
        except OSError as e:
            pass
        if (pos := self.buffer.find(b"\n")) >= 0:
            line = self.buffer[:pos]
            self.buffer = self.buffer[pos+1:]
            return json.loads(line.decode("ascii").strip())

def manage_service_communication(pipeline, context):
    for ServiceNode, pipe_name in [(ServiceInNode, "service.in"),
                                   (ServiceOutNode, "service.out")]:
        candidates = list(filter(
                        lambda x: isinstance(x, ServiceNode), 
                        pipeline))
        match len(candidates):
            case 0:
                candidate = ServiceNode(pipeline=pipeline, id="default")
            case 1:
                candidate, = candidates
            case _:
                raise Exception(
                    "Only one service node in each direction is allowed."
                    " Found these: " + ", ".join(map(str, candidates)))
        candidate.pipe = create_pipe(pipe_name, context)
    context["service_out_node"] = candidate
