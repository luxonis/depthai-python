from depthai.core_node import *

class ColorCamera(CoreNode):
    preview: Output(...)
    preview_size: tuple[int, int] = (300, 300)
    interleaved: bool = False

class MobileNetDetectionNetwork(CoreNode):
    input: Input(...)
    out: Output(...)
    blob_path: ...
    confidence_threshold: float = 0.5

class XLinkOut(CoreNode):
    input: Input(...)
    stream_name: str
