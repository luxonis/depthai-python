from depthai.core_node import *
import typing

T = typing.TypeVar('T')

class ColorCamera(CoreNode):
    preview: Output(typing.Any)
    preview_size: tuple[int, int] = (300, 300)
    interleaved: bool = False

class MobileNetDetectionNetwork(CoreNode):
    input: Input(typing.Any)
    out: Output(typing.Any)
    blob_path: ...
    confidence_threshold: float = 0.5

class XLinkOut(CoreNode):
    input: Input(typing.Any)
    stream_name: str
XLinkDevOut = XLinkOut

class XLinkIn(CoreNode):
    out: Output(typing.Any) # TODO Rename out to output?
    stream_name: str
XLinkDevIn = XLinkIn

